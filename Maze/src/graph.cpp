#include "graph.h"

graph::graph(int vert)
{
    this->verticesCount = vert;
    adjList = new list<int>[vert];
}

graph::~graph()
{
    for(int i = 0; i < verticesCount; i++){
        adjList[i].clear();
    }
    adjList->clear();
}

// Returns true if key(k) is in unordered_map
bool graph::checkKey(int k)
{
    if(nodeMap.find(k) == this->nodeMap.end())
        return false;
    return true;
}

// Returns key associated with pair(x, y)
int graph::returnKey(int x, int y){
    for(auto it = nodeMap.begin(); it != nodeMap.end(); ++it){
        if((*it).second.first == x && (*it).second.second == y)
            return (*it).first;
        }
    return 0;
}

// Returns true is pair(x,y) is in unordered_map
bool graph::checkPair(int x, int y){
    for(auto it = nodeMap.begin(); it != nodeMap.end(); ++it){
        if((*it).second.first == x && (*it).second.second == y)
            return true;
        }
    return false;
}

void graph::addEdge(int v, int w)
{
    adjList[v].push_back(w); // Add w to v’s list.
}

void graph::DFS(int s)
{
    // Initially mark all verices as not visited
    vector<bool> visited(verticesCount, false);

    // Create a stack for DFS
    stack<int> stack;

    // Push the current source node.
    stack.push(s);

    while (!stack.empty())
    {
        s = stack.top();
        stack.pop();

        if (!visited[s])
        {
            cout << "(" << nodeMap[s].first << ", " << nodeMap[s].second << ") - > ";
            visited[s] = true;
        }

        for (auto i = adjList[s].begin(); i != adjList[s].end(); ++i)
            if (!visited[*i])
                stack.push(*i);
    }
}

void graph::initGraph(int **myMatrix, int mazeSize){
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
                addEdge(returnKey(j, i), returnKey(j+1, i));
                cout << "Adding edge: [" << returnKey(j+1, i) << "](" << j+1 << ", " << i << ")" << " -> [" << returnKey(j, i) << "](" << j << ", " << i << ")\n\n";
                addEdge(returnKey(j+1, i), returnKey(j, i));
                edgeCount += 2;
            }
        }
    }
    cout << "Adding edges up and down\n";
    for(int j = 0; j < mazeSize; j++){
        for(int i = mazeSize - 1; i - 1 >= 0; i--){
            if(myMatrix[j][i] != 1 && myMatrix[j][i-1] != 1){
                cout << "Adding edge: [" << returnKey(j, i) << "](" << j << ", " << i << ")" << " -> [" << returnKey(j, i-1) << "](" << j << ", " << i-1 << ")\n";
                addEdge(returnKey(j, i), returnKey(j, i-1));
                cout << "Adding edge: [" << returnKey(j, i-1) << "](" << j << ", " << i-1 << ")" << " -> [" << returnKey(j, i) << "](" << j << ", " << i << ")\n\n";
                addEdge(returnKey(j, i-1), returnKey(j, i));
                edgeCount += 2;
            }
        }
    }

    cout << "Number of vertices: " << nodeCounter << endl;
    cout << "Number of edges: " << edgeCount << endl;
}
