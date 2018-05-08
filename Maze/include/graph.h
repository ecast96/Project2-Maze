#ifndef GRAPH_H
#define GRAPH_H
#include <list>
#include <unordered_map>
#include <stack>
#include <vector>
#include <utility>
#include <iostream>
using namespace std;

class graph
{
public:

    graph(int verticesCount);  // Constructor
    virtual ~graph();

    bool checkKey(int);
    int returnKey(int, int);
    bool checkPair(int, int);

    void initGraph(int **, int);
    void addEdge(int v, int w); // to add an edge to graph
    void DFS(int s);  // prints all vertices in DFS manner

private:

    unordered_map<int, pair<int,int> > nodeMap; // Used to store keys and pairs of coordinates

    int verticesCount;    // Number of vertices
    list<int> *adjList;    // adjacency lists
};

#endif // GRAPH_H
