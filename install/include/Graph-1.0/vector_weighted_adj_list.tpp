#include <bits/stdc++.h>
#include "../include/graph.h"

  template <std::integral WeightType, std::integral VertexType>
Vector_weighted_adj_list <WeightType, VertexType >::
  Vector_weighted_adj_list
(size_t n)
{
  for (size_t i=0; i<n; ++i)
  {
    std::vector <std::pair <WeightType, VertexType >> temp;
    graph.emplace_back (temp);
  }
}

template <std::integral WeightType, std::integral VertexType>
void
Vector_weighted_adj_list <WeightType, VertexType >::
  add_edge
(WeightType w, VertexType u, VertexType v)
{
  graph[u].emplace_back(w, v);
  graph[v].emplace_back(w, u);
}

template <std::integral WeightType, std::integral VertexType>
void
Vector_weighted_adj_list <WeightType, VertexType >::
  add_undirected_edge
(WeightType w, VertexType u, VertexType v)
{
  graph[u].emplace_back(w, v);
}

template <std::integral WeightType, std::integral VertexType>
std::unordered_map<VertexType, std::vector<std::pair<WeightType, VertexType>>>
Vector_weighted_adj_list <WeightType, VertexType >::
get_adj_list
() const
{
  std::unordered_map<VertexType, std::vector<std::pair<WeightType, VertexType>>> adjList;
  for (size_t u = 0; u < graph.size(); ++u)
  {
    adjList[u] = graph[u];
  }
  return adjList;
}

template <std::integral WeightType, std::integral VertexType>
size_t
Vector_weighted_adj_list <WeightType, VertexType >::
vertex_count
() const
{
  return graph.size();
}

