//
// Created by gonglinyuan on 2018-05-04.
//

#ifndef GED_GRAPH_H
#define GED_GRAPH_H


#include <string>
#include <vector>

class Graph {
public:
    int n; // number of nodes
    int *label; // label of nodes
    struct Edge {
        int x, y, type;
    };
    std::vector<Edge> e; // edges
    Graph();
    ~Graph();
    Graph(const Graph &pre);
    void read_graph(std::string filename);
};


#endif //GED_GRAPH_H
