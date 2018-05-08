#include<bits/stdc++.h>
#include <utility>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

unordered_map<int, pair<int,int> > nodeMap;

int mazeSize = 10;
int **myMatrix;

class Graph
{
    int V;    // No. of vertices
    list<int> *adj;    // adjacency lists
public:
    Graph(int V);  // Constructor
    void addEdge(int v, int w); // to add an edge to graph
    void DFS(int s);  // prints all vertices in DFS manner
    // from a given source.
};
 
Graph::Graph(int V)
{
    this->V = V;
    adj = new list<int>[V];
}
 
void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w); // Add w to v’s list.
}
 
void Graph::DFS(int s)
{
    // Initially mark all verices as not visited
    vector<bool> visited(V, false);
 
    // Create a stack for DFS
    stack<int> stack;
 
    // Push the current source node.
    stack.push(s);
 
    while (!stack.empty())
    {
        s = stack.top();
        stack.pop();

        if(nodeMap[s].first == P->getPlayerLoc().x && nodeMap[s].second == P->getPlayerLoc().y)
            return;

        if (!visited[s])
        {
            cout << "(" << nodeMap[s].first << ", " << nodeMap[s].second << ") - > ";
            visited[s] = true;
        }
 
        for (auto i = adj[s].begin(); i != adj[s].end(); ++i)
            if (!visited[*i])
                stack.push(*i);
    }
}

void initMatrix()
{
    myMatrix = new int*[mazeSize];
    for(int i = 0; i < mazeSize; i++){
        myMatrix[i] = new int[mazeSize];
        for(int j = 0; j < mazeSize; j++)
            myMatrix[i][j] = 0;
    }
}

void PrintMatrix() {
	for (int i = mazeSize - 1; i >= 0; i--) {
        cout << i << "  ";
		for (int j = 0; j < mazeSize; j++)
			cout << myMatrix[j][i] << " ";
		cout << endl;
	}
    cout << "                       \n";
    cout << "   0 1 2 3 4 5 6 7 8 9\n";
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
                myMatrix[a][b] = 1;
            }
            if(label == "enemy"){
                myMatrix[a][b] = 2;
            }
            if(label == "player"){
                myMatrix[a][b] = 3;
            }
            if(label == "arrow"){
                myMatrix[a][b] = 5;
            }
            if(label == "chest"){
                myMatrix[a][b] = 4;
            }
        }
    }
}

// Returns true if key(k) is in unordered_map
bool checkKey(int k)
{
    if(nodeMap.find(k) == nodeMap.end())
        return false;
    return true;
}

// Returns true is pair(x,y) is in unordered_map
bool checkPair(int x, int y){
    for(auto it = nodeMap.begin(); it != nodeMap.end(); ++it){
        if((*it).second.first == x && (*it).second.second == y)
            return true;
        }
    return false;
}

int returnKey(int x, int y){
    for(auto it = nodeMap.begin(); it != nodeMap.end(); ++it){
        if((*it).second.first == x && (*it).second.second == y)
            return (*it).first;
        }
    return 0;
}

Graph g(200); // Total 5 vertices in graph

void checkMatrix(){
    int nodeCounter = -1;
    int edgeCount = 0;

    for(int i = mazeSize - 1; i >= 0; i--){
        for(int j = 0; j < mazeSize; j++){
            if(myMatrix[j][i] != 1){
                nodeCounter++;
                cout << "Inserting (" << j << ", " << i << ") into map with key: " << nodeCounter << endl;
                nodeMap.insert({nodeCounter, make_pair(j, i)});
            }
        }
    }

    cout << "Number of vertices: " << nodeCounter << endl;

    cout << "Addes edges Left and Right\n";
    for(int i = mazeSize - 1; i >= 0; i--){
        for(int j = 0; j < mazeSize - 1; j++){
            if(myMatrix[j][i] != 1 && myMatrix[j+1][i] != 1){
                cout << "Adding edge: [" << returnKey(j, i) << "](" << j << ", " << i << ")" << " -> [" << returnKey(j+1, i) << "](" << j+1 << ", " << i << ")\n";
                g.addEdge(returnKey(j, i), returnKey(j+1, i));
                cout << "Adding edge: [" << returnKey(j+1, i) << "](" << j+1 << ", " << i << ")" << " -> [" << returnKey(j, i) << "](" << j << ", " << i << ")\n\n";
                g.addEdge(returnKey(j+1, i), returnKey(j, i));
                edgeCount += 2;
            }
        }
    }
    cout << "Adding edges up and down\n";
    for(int j = 0; j < mazeSize; j++){
        for(int i = mazeSize - 1; i - 1 >= 0; i--){
            if(myMatrix[j][i] != 1 && myMatrix[j][i-1] != 1){
                cout << "Adding edge: [" << returnKey(j, i) << "](" << j << ", " << i << ")" << " -> [" << returnKey(j, i-1) << "](" << j << ", " << i-1 << ")\n";
                g.addEdge(returnKey(j, i), returnKey(j, i-1));
                cout << "Adding edge: [" << returnKey(j, i-1) << "](" << j << ", " << i-1 << ")" << " -> [" << returnKey(j, i) << "](" << j << ", " << i << ")\n\n";
                g.addEdge(returnKey(j, i-1), returnKey(j, i));
                edgeCount += 2;
            }
        }
    }

    cout << "Number of vertices: " << nodeCounter << endl;
    cout << "Number of edges: " << edgeCount << endl;
            /*
            if(j < mazeSize - 1){
                if(myMatrix[j+1][i] != 1){
                    cout << "Adding edge: [" << nodeCounter-1 << "](" << j << ", " << i << ")" << " -> [" << nodeCounter << "](" << j+1 << ", " << i << ")\n";
                    g.addEdge(nodeCounter-1, nodeCounter);
                    cout << "Adding edge: [" << nodeCounter << "](" << j+1 << ", " << i << ")" << " -> [" << nodeCounter-1 << "](" << j << ", " << i << ")\n\n";
                    g.addEdge(nodeCounter, nodeCounter-1);
                }
            }
            if(i - 1 >= 0){
                if(myMatrix[j][i-1] != 1){
                    cout << "Adding edge: [" << nodeCounter-1 << "](" << j << ", " << i << ")" << " -> [" << nodeCounter << "](" << j << ", " << i-1 << ")\n";
                    g.addEdge(nodeCounter-1, nodeCounter);
                    cout << "Adding edge: [" << nodeCounter << "](" << j << ", " << i-1 << ")" << " -> [" << nodeCounter-1 << "](" << j << ", " << i << ")\n\n";
                    g.addEdge(nodeCounter, nodeCounter-1);
                    
                }
            }
            */    

            /*
            cout << "- j : " <<  j << " \n";
            if(myMatrix[j][i] != 1){
                if(i - 1 > 0){
                    if(myMatrix[j][i-1] != 1)`
                        nodeCounter++;
                        if(!(checkPair(j, i))){
                            cout << "Inserting (" << j << ", " << i << ") into map with key: " << nodeCounter << endl;
                            nodeMap.insert({nodeCounter, make_pair(j, i)});
                            tempV = nodeCounter;
                        }
                        else
                            tempV = returnKey(j, i);

                        nodeCounter++;
                        if(!(checkPair(j, i-1))){
                            cout << "Inserting (" << j << ", " << i-1 << ") into map with key: " << nodeCounter << endl;
                            nodeMap.insert({nodeCounter, make_pair(j, i-1)});
                            tempW = nodeCounter;
                        }
                        else
                            tempW = returnKey(j, i-1);

                        cout << "Adding edge: [" << tempV << "](" << j << ", " << i << ")" << " -> [" << tempW << "](" << j << ", " << i-1 << ")\n\n";
                        g.addEdge(tempV, tempW);
                        //cout << "[" << nodeCounter++ << "](" << i << ", " << j << ")" << " -> " << "[" << nodeCounter++ << "](" << i << ", " << j-1 << ")\n";
                }
                if(j + 1 < mazeSize){
                    if(myMatrix[i][j+1] != 1) //Up
                        cout << "[" << nodeCounter++ << "](" << i << ", " << j << ")" << " -> " << "[" << nodeCounter++ << "](" << i << ", " << j+1 << ")\n";
                }
                */
}

void deleteMatrix(){
    for(int i = 0; i < mazeSize; i++)
        delete [] myMatrix;
    delete [] myMatrix;
}
 
int main()
{

    initMatrix();
    readFile();
    PrintMatrix();
    checkMatrix();

   /*
    nodeMap.insert({0, make_pair(0, 0)});
    nodeMap.insert({1, make_pair(0, 1)});
    nodeMap.insert({2, make_pair(1, 1)});
    nodeMap.insert({3, make_pair(1, 2)});
    nodeMap.insert({4, make_pair(1, 0)});

    //cout << "(1,3) exists in map? " << boolalpha << checkPair(1, 3) << endl;

    /*
    for(int i = 0; i < nodeMap.size(); i++){
        cout << nodeMap[i].first << "," << nodeMap[i].second << endl;
    }
    */

    //cout << nodeMap[0].first << "," << nodeMap[0].second << endl;

    /*
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(0, 4);

    cout << "Following is Depth First Traversal\n";
    g.DFS(0);
    cout << endl;
    */
    g.DFS(returnKey(0, 3));

    return 0;
}