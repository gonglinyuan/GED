//
// Created by gonglinyuan on 2018-05-04.
//

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <cassert>
#include <iostream>
#include <map>
#include "Graph.h"
#include "main.h"

using std::string;
using std::pair;
using std::make_pair;
using std::stoi;
using boost::property_tree::ptree;
using boost::property_tree::read_xml;
using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::max;

static pair<int, int> read_node(const ptree &t) {
    int id = -1, label = -1;
    for (auto i = t.begin(); i != t.end(); ++i) {
        if (i->first == "<xmlattr>") {
            for (auto j = i->second.begin(); j != i->second.end(); ++j) {
                if (j->first == "id") {
                    string str = j->second.data();
                    if (str.at(0) == '_') {
                        id = stoi(str.substr(1));
                    } else {
                        id = stoi(str);
                    }
                }
            }
        } else if (i->first == "attr") {
            for (auto j = i->second.begin(); j != i->second.end(); ++j) {
                if (j->first == "int") {
                    string str = j->second.data();
                    label = stoi(str);
                } else if (j->first == "string") {
                    string str = j->second.data();
                    label = (int)str.at(0);
                }
            }
        }
    }
    assert(id != -1);
    assert(label != -1);
    return make_pair(id, label);
}

static pair<pair<int, int>, int> read_edge(const ptree &t) {
    int x = -1, y = -1, type = -1;
    for (auto i = t.begin(); i != t.end(); ++i) {
        if (i->first == "<xmlattr>") {
            for (auto j = i->second.begin(); j != i->second.end(); ++j) {
                if (j->first == "from") {
                    string str = j->second.data();
                    if (str.at(0) == '_') {
                        x = stoi(str.substr(1));
                    } else {
                        x = stoi(str);
                    }
                } else if (j->first == "to") {
                    string str = j->second.data();
                    if (str.at(0) == '_') {
                        y = stoi(str.substr(1));
                    } else {
                        y = stoi(str);
                    }
                }
            }
        } else if (i->first == "attr") {
            for (auto j = i->second.begin(); j != i->second.end(); ++j) {
                if (j->first == "int") {
                    string str = j->second.data();
                    type = stoi(str);
                }
            }
        }
    }
    assert(x != -1);
    assert(y != -1);
    assert(type != -1);
    return make_pair(make_pair(x, y), type);
}

void Graph::read_graph(string filename) {
    ptree tree, t;
    n = 0;
    e.clear();
    map<int, int> labels;
    read_xml(filename, tree);
    t = tree.get_child("gxl.graph");
    for (auto i = t.begin(); i != t.end(); ++i) {
        if (i->first == "node") {
            auto node = read_node(i->second);
            labels[node.first] = node.second;
            n = max(n, node.first);
        } else if (i->first == "edge") {
            auto edge = read_edge(i->second);
            e.push_back((Edge) {edge.first.first, edge.first.second, edge.second});
        }
    }
    label = new int[n + 1];
    for (int i = 1; i <= n; ++i) {
        label[i] = labels[i];
    }
}

Graph::Graph() {
    n = 0;
    label = nullptr;
    e = vector<Edge>();
}

Graph::~Graph() {
    delete[](label);
}

Graph::Graph(const Graph &pre) {
    n = pre.n;
    label = new int[n + 5];
    for (int i = 1; i <= n; ++i) label[i] = pre.label[i];
    e = pre.e;
}