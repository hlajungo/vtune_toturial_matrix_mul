#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include "graph.h"
#include "prim.h"
#include "graph_utility.h"

TEST_CASE("Graph operations and Prim's MST")
{
    Graph<int, int>* graph = new Vector_weighted_adj_list<int, int>(5);
    graph->add_edge(10, 1, 0);
    graph->add_edge(1, 2, 0);
    graph->add_edge(3, 2, 1);
    graph->add_edge(2, 3, 1);
    graph->add_edge(8, 3, 2);
    graph->add_edge(4, 4, 3);

    auto mst = primMST(graph, 0);
    print_unordered_map_adj_list(mst);
    
    CHECK(mst.size() > 0); // 簡單的檢查 MST 是否非空
    delete graph;
}


