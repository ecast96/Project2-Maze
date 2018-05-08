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
#include <unordered_map>
#include <stack>
#include <list>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <Enemies.h>

#include <wall.h>
#include <math.h>
#include <graph.h>

/* GLUT callback Handlers */

using namespace std;

const int mazeSize = 10;
Maze *M = new Maze(mazeSize);                         // Set Maze grid size
int **myMatrix;

const int verticesCount = 100;
graph g(verticesCount);

const int wallAmount = mazeSize * mazeSize;
int wallCounter = 1;
wall W[wallAmount];                             // wall with number of tile

const int enemyAmount = 4;
int enemyCounter = 0;
Enemies E[enemyAmount];                         // create number of enemies

Player *P = new Player();                       // create player
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
        E[enemyCounter].initEnm(M->getGridSize(),4,"images/chicken.png"); //Load enemy image
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
                P->initPlayer(M->getGridSize(),4,"images/llama.png");   // initialize player pass grid size,image and number of frames
                P->loadArrowImage("images/arr.png");                // Load arrow image
                P->placePlayer(a, b);
                myMatrix[a][b] = 3;
            }
            if(label == "arrow"){
                cout << "Arrows placed at: " << a << "," << b << endl;
                M->loadSetOfArrowsImage("images/arrwset.png");      // load set of arrows image
                M->placeStArrws(a, b);
                myMatrix[a][b] = 5;
            }
            if(label == "chest"){
                cout << "Chest found at: " << a << "," << b << endl;
                M->loadBackgroundImage("images/bak.jpg");           // Load maze background image
                M->loadChestImage("images/chest.png");              // load chest image
                M->placeChest(a, b);
                myMatrix[a][b] = 4;
            }
        }
    }
}

void initMatrix(){
	myMatrix = new int*[mazeSize];
	for (int i = 0; i < mazeSize; i++) {
		myMatrix[i] = new int[mazeSize];
		for (int j = 0; j < mazeSize; j++)
			myMatrix[i][j] = 0;
	}
}
void PrintMatrix() {
	// Display Adjacency matrix
	for (int i = mazeSize - 1; i >= 0; i--) {
		for (int j = 0; j < mazeSize; j++)
			cout << myMatrix[j][i] << " ";
		cout << endl;
	}
	cout << endl;
}

void deleteAll(){
	for (int i = 0; i < mazeSize; i++)
		delete [] myMatrix;
	delete[] myMatrix;
	delete P;
	delete T0;
	delete M;
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

    initMatrix();
	//PrintMatrix(myMat);

    readFile(); // Reads input from text file, places objects in maze
    PrintMatrix();  //Prints current values from matrix
    g.initGraph(myMatrix, mazeSize); // creates graph to be used by dfs
    g.DFS(g.returnKey(0, 3));   // runs dfs on graph created
}

void display(void)
{
    if(M->gameOver == true){
        static const auto hasWon = [] { PrintMatrix(); return true;}();

        glClear (GL_COLOR_BUFFER_BIT);        // clear display screen
        glPushMatrix();
        M->drawBackground();
        glPopMatrix();
        glutSwapBuffers();


    }else {
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
}




void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'z':
            P->shootMode = !(P->shootMode);
            if(P->shootMode == true)
                cout << "Now in shooting mode." << endl;
            else
                cout << "Now in walking mode." << endl;
        break;

        case ' ':
          // if(!M->liveSetOfArrws)      // if setof arrows were picked by player
             //P->shootArrow();
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
    //Collision with enemy and player = Lose
    for(int i  = 0; i < enemyCounter; i++){
        if(E[i].getEnemyLoc().x == P->getPlayerLoc().x && E[i].getEnemyLoc().y == P->getPlayerLoc().y){
            static const auto hasDied = [] { cout << "Player has died!" << endl; return true;}();
            P->livePlayer = false;
            M->gameOver = 1;
            myMatrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 2;
            PrintMatrix();
            deleteAll();
            exit(0);
        }
    }

    //Broken for game edges -possible fix: check raw x, y (floats)
    if(P->arrowStatus == 1){
        if(P->getArrowLoc().x - 1 >= -1 && P->getArrowLoc().x + 1 <= 10){
            //Collision with arrow and walls
            if(myMatrix[P->getArrowLoc().x][P->getArrowLoc().y] == 1)
                P->arrowStatus = 0;

            //Collisions with arrows and enemies
            for(int i = 0; i < enemyCounter; i++){
                if(E[i].getEnemyLoc().x == P->getArrowLoc().x && E[i].getEnemyLoc().y == P->getArrowLoc().y){
                    E[i].live = false;
                    myMatrix[E[i].getEnemyLoc().x][E[i].getEnemyLoc().y] = 0;
                    E[i].placeEnemy(mazeSize + 1, mazeSize + 1);
                    P->arrowStatus = false;
                }
            }
        }
    }

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

// Allows player to shoot when player reaches set of arrows
void checkArrows(int x, int y){
    if(myMatrix[x][y] == 5){
        cout << "Player has gathered arrows!\n";
        P->hasArrows = true;
        M->liveSetOfArrws = false;
    }
}

//Collision with player and chest = Win
void checkChest(int x, int y){
    if(myMatrix[x][y] == 4){
        cout << "Player has won!\n";
        M->liveChest = 0;
        M->gameOver = 1; //Going to be used for displaying new screen
        deleteAll();
        exit(0);
    }
}

void Specialkeys(int key, int x, int y)
{

    switch(key)
    {
    case GLUT_KEY_UP:
         if(P->shootMode == false){
             if(P->getPlayerLoc().y + 1 < mazeSize){
                 if(!(myMatrix[P->getPlayerLoc().x][P->getPlayerLoc().y + 1] == 1)){ //Walking mode
                    checkArrows(P->getPlayerLoc().x, P->getPlayerLoc().y + 1);
                    checkChest(P->getPlayerLoc().x, P->getArrowLoc().y + 1);

                    myMatrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 0;
                    P->movePlayer("up");
                    myMatrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 3;
                    PrintMatrix();
                 }
            }
         }
         else if(P->hasArrows == true){ //Shooting mode
            P->playerDir = "up";
            P->shootArrow();
         }

        /*
         if(!(myMatrix[E[0].getEnemyLoc().x][E[0].getEnemyLoc().y + 1] == 1
              || myMatrix[E[0].getEnemyLoc().x][E[0].getEnemyLoc().y + 1] == 2)){
            E[0].moveEnemy("up");
            }
            */
         break;


    case GLUT_KEY_DOWN:
         if(P->shootMode == false){
             if(P->getPlayerLoc().y - 1 >= 0){
                 if(!(myMatrix[P->getPlayerLoc().x][P->getPlayerLoc().y - 1] == 1)){ //Walking mode
                    checkArrows(P->getPlayerLoc().x, P->getPlayerLoc().y - 1);
                    checkChest(P->getPlayerLoc().x, P->getPlayerLoc().y - 1);

                    myMatrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 0;
                    P->movePlayer("down");
                    myMatrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 3;
                    PrintMatrix();
                 }
             }
         }
         else if(P->hasArrows == true){ //Shooting mode
            P->playerDir = "down";
            P->shootArrow();
         }

        /*
         if(!(myMatrix[E[0].getEnemyLoc().x][E[0].getEnemyLoc().y - 1] == 1
              || myMatrix[E[0].getEnemyLoc().x][E[0].getEnemyLoc().y - 1] == 2) && E[0].live){
            E[0].moveEnemy("down");
            }
            */
         break;

    case GLUT_KEY_LEFT:
        if(P->shootMode == false){
            if(P->getPlayerLoc().x -1 >= 0){
                if(!(myMatrix[P->getPlayerLoc().x - 1][P->getPlayerLoc().y] == 1)){ //Walking mode
                    checkArrows(P->getPlayerLoc().x - 1, P->getPlayerLoc().y);      //If next step is set of arrows
                    checkChest(P->getPlayerLoc().x - 1, P->getPlayerLoc().y);       //If next step is chest/goal

                    myMatrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 0;
                    P->movePlayer("left");
                    myMatrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 3;
                    PrintMatrix();
                }
            }
        }
        else if(P->hasArrows == true){ //Shooting mode
            P->playerDir = "left";
            P->shootArrow();
        }

        /*
         if(!(myMatrix[E[0].getEnemyLoc().x - 1][E[0].getEnemyLoc().y] == 1
              || myMatrix[E[0].getEnemyLoc().x - 1][E[0].getEnemyLoc().y] == 2) && E[0].live){
            E[0].moveEnemy("left");
            }
            */
         break;

    case GLUT_KEY_RIGHT:
        if(P->shootMode == false){
            if(P->getPlayerLoc().x + 1 < mazeSize){
                if(!(myMatrix[P->getPlayerLoc().x + 1][P->getPlayerLoc().y] == 1)){ //Walking mode
                    checkArrows(P->getPlayerLoc().x + 1, P->getPlayerLoc().y);
                    checkChest(P->getPlayerLoc().x + 1, P->getPlayerLoc().y);

                    myMatrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 0;
                    P->movePlayer("right");
                    myMatrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 3;
                    PrintMatrix();
                }
            }
        }
         else if(P->hasArrows){ //Shooting mode
            P->playerDir = "right";
            P->shootArrow();
         }

        /*
         if(!(myMatrix[E[0].getEnemyLoc().x + 1][E[0].getEnemyLoc().y] == 1
              || myMatrix[E[0].getEnemyLoc().x + 1][E[0].getEnemyLoc().y] == 2) && E[0].live){
            E[0].moveEnemy("right");
            }
            */
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
