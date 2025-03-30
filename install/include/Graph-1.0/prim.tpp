#include <bits/stdc++.h>
#include "../include/prim.h"



template <std::integral ReturnType
, std::integral WeightType
, std::integral VertexType>
  ReturnType
primMST_weight
(const Graph <WeightType, VertexType >& graph
    , VertexType start_v)
{
  std::priority_queue <std::pair <int, int >
    , std::vector <std::pair <int, int >>
    , std::greater <std::pair <int, int >>> min_heap;

  std::vector <bool> visited(graph.size(), 0);
  int min_cost = 0;

  while (! min_heap.empty())
  {
    auto [w, u] = min_heap.top(); min_heap.pop();
    if ( visited[u] )
      continue;
    // 將要選擇此點，並加入他的鄰點
    min_cost += w;
    visited[u] = 1;
    std::cout << "DEBUG: " << "ADD " << u << "and weight " << w << '\n';
    for (auto& v: graph[u])
    {
      if ( visited[v.second] )
        continue;
      min_heap.push ({v.first, v.second});
    }

  }
  return min_cost;
}

template <std::integral WeightType
, std::integral VertexType
, std::integral IntegralType>
  std::unordered_map <VertexType, std::vector <std::pair <WeightType, VertexType >>>
primMST
(const Graph <WeightType, VertexType >* i_graph
    , const IntegralType start_v)
{
  std::unordered_map <VertexType
    , std::vector <std::pair <WeightType, VertexType >>> graph = i_graph->get_adj_list();

  // return buffer
  std::unordered_map <VertexType
    , std::vector <std::pair <WeightType, VertexType >>> mst;

  // tuple min_heap, tuple = <v_weight, v_vertex, u_vertex>
  std::priority_queue < std::tuple <WeightType, VertexType, VertexType>
    , std::vector <std::tuple <WeightType, VertexType, VertexType>>
    , std::greater <std::tuple <WeightType, VertexType, VertexType>> > min_heap;

  std::vector <bool> visited(graph.size(), 0);
  bool first = true;

  min_heap.push ({0, start_v, 0});
  while (! min_heap.empty())
  {
    auto [w, u, from_vertex] = min_heap.top(); min_heap.pop();
    if ( visited[u] )
      continue;
    // 將要選擇此點，並加入他的鄰點
    if (first == false)
    {
      mst[from_vertex].push_back ({w, u});
      mst[u].push_back ({w, from_vertex});

    }
    first = false;

    visited[u] = 1;
    for (auto v: graph[u])
    {
      if ( visited[v.second] )
        continue;
      min_heap.push (std::make_tuple(v.first, v.second, u));
    }

  }
  return mst;
}






