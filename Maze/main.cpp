/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */

#include <string.h>
#include <CommonThings.h>
#include <Maze.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <Timer.h>
#include <player.h>
#include <vector>
#include <utility>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <Enemies.h>

#include <wall.h>
#include <math.h>

/* GLUT callback Handlers */

using namespace std;

const int mazeSize = 10;
const double matrixSize = 10;

int myMatrix[mazeSize][mazeSize];

const int wallAmount = mazeSize * mazeSize;
int wallCounter = 1;

const int enemyAmount = 3;
int enemyCounter = 0;

Maze *M = new Maze(mazeSize);                         // Set Maze grid size
Player *P = new Player();                       // create player

wall W[wallAmount];                             // wall with number of tile

Enemies E[enemyAmount];                         // create number of enemies
Timer *T0 = new Timer();                        // animation timer

float wWidth, wHeight;                          // display window width and Height
float xPos,yPos;                                // Viewpoar mapping

void display(void);                             // Main Display : this runs in a loop

void resize(int width, int height)              // resizing case on the window
{
    wWidth = width;
    wHeight = height;

    if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
}

void placeWall(int a, int b){
    if(wallCounter < wallAmount){
        W[wallCounter].wallInit(M->getGridSize(),"images/wall.png");// Load walls
        W[wallCounter].placeWall(a, b);
        wallCounter++;
    }
    else {
        cout << "Too many walls defined in text file!\n";
        return;
    }
}

void placeEnemy(int a, int b){
    if(enemyCounter < enemyAmount){
        E[enemyCounter].initEnm(M->getGridSize(),4,"images/e.png"); //Load enemy image
        E[enemyCounter].placeEnemy(a,b);
        enemyCounter++;
    }
    else{
        cout << "Too many enemies defined in text file!\n";
        return;
    }
}

void readFile()
{
    int a, b;
    string line, label;
    ifstream infile("maze.txt");

    if(infile.is_open()){
        while(getline(infile, line)){
            istringstream ss(line);
            ss >> label >> a >> b;

            if(label == "wall"){
                cout << "Wall found at: " << a << "," << b << endl;
                placeWall(a, b);
                myMatrix[a][b] = 1;
            }
            if(label == "enemy"){
                cout << "Enemy placed at: " << a << "," << b << endl;
                placeEnemy(a, b);
                myMatrix[a][b] = 2;
            }
            if(label == "player"){
                cout << "player placed at: " << a << "," << b << endl;
                P->initPlayer(M->getGridSize(),6,"images/p.png");   // initialize player pass grid size,image and number of frames
                P->loadArrowImage("images/arr.png");                // Load arrow image
                P->placePlayer(a, b);
            }
            if(label == "arrow"){
                cout << "Arrows placed at: " << a << "," << b << endl;
                M->loadSetOfArrowsImage("images/arrwset.png");      // load set of arrows image
                M->placeStArrws(a, b);
            }
            if(label == "chest"){
                cout << "Chest found at: " << a << "," << b << endl;
                M->loadBackgroundImage("images/bak.jpg");           // Load maze background image
                M->loadChestImage("images/chest.png");              // load chest image
                M->placeChest(a, b);
            }
        }
    }
}


void init()
{
    glEnable(GL_COLOR_MATERIAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0,0.0,0.0,0.0);
    gluOrtho2D(0, wWidth, 0, wHeight);

    T0->Start();                                        // set timer to 0

    glEnable(GL_BLEND);                                 //display images with transparent
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    readFile();

    //M->loadBackgroundImage("images/bak.jpg");           // Load maze background image
    //M->loadChestImage("images/chest.png");              // load chest image
    //M->placeChest(3,3);                                 // place chest in a grid

    //M->loadSetOfArrowsImage("images/arrwset.png");      // load set of arrows image
    //M->placeStArrws(5,3);                               // place set of arrows

    //P->initPlayer(M->getGridSize(),6,"images/p.png");   // initialize player pass grid size,image and number of frames
    //P->loadArrowImage("images/arr.png");                // Load arrow image
    //P->placePlayer(9,9);                                // Place player

    /*
    for(int i=0; i<3;i++)
    {
        E[i].initEnm(M->getGridSize(),4,"images/e.png"); //Load enemy image
        E[i].placeEnemy(float(rand()%(M->getGridSize())),float(rand()%(M->getGridSize())));
        //place enemies random x,y
    }
    */
}

void display(void)
{
  glClear (GL_COLOR_BUFFER_BIT);        // clear display screen

        glPushMatrix();
         M->drawBackground();
        glPopMatrix();


        for(int i=0; i <= wallAmount;i++)
        {
           W[i].drawWall();
        }

        glPushMatrix();
            M->drawGrid();
        glPopMatrix();

        glPushMatrix();
            P->drawplayer();
        glPopMatrix();

        for(int i=0; i < enemyAmount ;i++)
        {
        E[i].drawEnemy();
        }

        glPushMatrix();
            P->drawArrow();
        glPopMatrix();

         glPushMatrix();
           M->drawChest();
        glPopMatrix();

        glPushMatrix();
           M->drawArrows();
        glPopMatrix();

    glutSwapBuffers();
}




void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case ' ':
          // if(!M->liveSetOfArrws)      // if setof arrows were picked by player
             P->shootArrow();
        break;
        case 27 :                       // esc key to exit
        case 'q':
            exit(0);
            break;
    }

    glutPostRedisplay();
}


 void GetOGLPos(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    xPos =posX ;
    yPos =posY ;
}

 void idle(void)
{

    //Your Code here
    glutPostRedisplay();
}


void mouse(int btn, int state, int x, int y){

    switch(btn){
        case GLUT_LEFT_BUTTON:

        if(state==GLUT_DOWN){

              GetOGLPos(x,y);
            }
            break;


      case GLUT_RIGHT_BUTTON:

        if(state==GLUT_DOWN){

              GetOGLPos(x,y);
            }
            break;
    }
     glutPostRedisplay();
};

void Specialkeys(int key, int x, int y)
{

    // Your Code here
    switch(key)
    {
    case GLUT_KEY_UP:
         cout<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
         if(myMatrix[P->getPlayerLoc().x][int(P->getPlayerLoc().y + .5)] == 1)
            cout << "Wall detected at: " << P->getPlayerLoc().x << "    " << P->getPlayerLoc().y+1 << endl;
         else
            P->movePlayer("up");
         if(!(myMatrix[E[0].getEnemyLoc().x][E[0].getEnemyLoc().y + 1] == 1))
            E[0].moveEnemy("up");
         break;

    case GLUT_KEY_DOWN:
         cout<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
         if(myMatrix[P->getPlayerLoc().x][int(P->getPlayerLoc().y - .5)] == 1)
            cout << "Wall detected at: " << P->getPlayerLoc().x << "    " << P->getPlayerLoc().y-1 << endl;
         else
            P->movePlayer("down");
         if(!(myMatrix[E[0].getEnemyLoc().x][E[0].getEnemyLoc().y - 1] == 1))
            E[0].moveEnemy("down");
         break;

    case GLUT_KEY_LEFT:
         cout<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
         if(myMatrix[int(P->getPlayerLoc().x - 1)][P->getPlayerLoc().y] == 1)
            cout << "Wall detected at: " << P->getPlayerLoc().x-1 << "    " << P->getPlayerLoc().y << endl;
         else
            P->movePlayer("left");
         if(!(myMatrix[E[0].getEnemyLoc().x - 1][E[0].getEnemyLoc().y] == 1))
            E[0].moveEnemy("left");
         break;

    case GLUT_KEY_RIGHT:
         cout<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
         if(myMatrix[int(P->getPlayerLoc().x + 1)][P->getPlayerLoc().y] == 1)
            cout << "Wall detected at: " << P->getPlayerLoc().x+1 << "    " << P->getPlayerLoc().y << endl;
         else
            P->movePlayer("right");
         if(!(myMatrix[E[0].getEnemyLoc().x + 1][E[0].getEnemyLoc().y] == 1))
            E[0].moveEnemy("right");
         break;
   }
  glutPostRedisplay();
}

/* Program entry point */
int main(int argc, char *argv[])
{

   glutInit(&argc, argv);

   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize (800, 800);                //window screen
   glutInitWindowPosition (100, 100);            //window position
   glutCreateWindow ("Maze");                    //program title
   init();

   glutDisplayFunc(display);                     //callback function for display
   glutReshapeFunc(resize);                      //callback for reshape
   glutKeyboardFunc(key);                        //callback function for keyboard
   glutSpecialFunc(Specialkeys);
   glutMouseFunc(mouse);
   glutIdleFunc(idle);
   glutMainLoop();

   return EXIT_SUCCESS;
}
