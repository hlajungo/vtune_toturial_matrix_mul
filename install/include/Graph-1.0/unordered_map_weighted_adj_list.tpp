#include <bits/stdc++.h>
#include "../include/graph.h"

template <std::integral WeightType, std::integral VertexType>
Unordered_map_weighted_adj_list<WeightType, VertexType>::
  Unordered_map_weighted_adj_list
(size_t n)
{
  for (size_t i=0; i<n; ++i)
  {
    std::vector <std::pair <WeightType, VertexType>> temp;
    graph[i] = temp;
  }
}

template <std::integral WeightType, std::integral VertexType>
void
Unordered_map_weighted_adj_list<WeightType, VertexType>::
  add_edge
(WeightType w, VertexType u, VertexType v)
{
  graph[u].emplace_back(w, v);
  graph[v].emplace_back(w, u);
}

template <std::integral WeightType, std::integral VertexType>
void
Unordered_map_weighted_adj_list<WeightType, VertexType>::
  add_undirected_edge
(WeightType w, VertexType u, VertexType v)
{
  graph[u].emplace_back(w, v);
}

template <std::integral WeightType, std::integral VertexType>
Unordered_map_weighted_adj_list <WeightType, VertexType>
Unordered_map_weighted_adj_list<WeightType, VertexType>::
get_adj_list
() const
{
  return graph;
}

template <std::integral WeightType, std::integral VertexType>
size_t
Unordered_map_weighted_adj_list<WeightType, VertexType>::
vertex_count
() const
{
  return graph.size();
}



