#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "Graph.h"
#include "Model.h"
#include "main.h"

using boost::property_tree::ptree;
using boost::property_tree::read_xml;
using std::cout;
using std::endl;
using std::stoi;

int main(int argc, char **argv) {
    if (argc != 6 + 1) {
        cout << "usage: GED cnode,sub cnode,del/ins cedge,sub cedge,del/ins g1.gxl g2.gxl" << endl;
        return 0;
    }
    int c_node_sub = stoi(argv[1]), c_node_ins = stoi(argv[2]), c_edge_sub = stoi(argv[3]), c_edge_ins = stoi(argv[4]);
    Graph g1, g2;
    g1.read_graph(argv[5]);
    g2.read_graph(argv[6]);
    Model model(c_node_sub, c_node_ins, c_edge_sub, c_edge_ins, g1, g2);
    auto ans = model.tree_solve();
   /* cout << ans.first << ';' << "29.99";
    bool first_one = true;
    for (int it : ans.second) {
        cout << (first_one ? ";" : " ");
        first_one = false;
        cout << it;
    }
#ifdef OUTPUT_DIST
    cout << " ;" << model.check_ans(ans.second);
#endif
    cout << endl;*/
   cout << model.check_ans(ans.second) << endl;
    return 0;
}