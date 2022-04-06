#include <vector>
#include <queue>

#include "Precinct.h"
#include "Graph.h"

using std::vector;
using std::queue;

void Graph::BFS(int start, int end){

    //established temporary vector to keep track of visited nodes
    vector<int> visited;
    for(int i = 0; i < precincts.size(); i++){
        visited.push_back(0);
    }

    //
    queue<int> search;
    search.push(start);

    while(!search.empty()){
        int cur = search.front();
        search.pop();

        // returns if end is found
        if(cur == end){
            return;
        }

        // otherwise, if not searched, set searched and add everything in connected
        if(visited[cur] == 0){
            visited[cur] = 1;
            for(int i = 0; i < edges[cur].size(); i++){
                if(edges[cur][i] == 1){
                    search.push(i);
                }
            }
        }
    }
}

void Graph::BFSComponent(int vertex){
}


//