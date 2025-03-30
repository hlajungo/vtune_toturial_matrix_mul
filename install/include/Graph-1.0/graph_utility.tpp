#include <bits/stdc++.h>
#include "../include/graph_utility.h"

template <typename T>
void
  print_unordered_map_adj_list
(T& graph)
{
  std::cout << "[\n";
  for (auto u: graph)
  {
    std::cout << "  " << u.first << " : [ ";
    for (auto v: u.second)
    {
      std::cout << v.second << " ";
    }
    std::cout << "]\n";
  }
  std::cout << "]\n";
}

template <typename T>
void
  print_vector_adj_list
(T& graph)
{
  std::cout << "[\n";
  int i=0;
  for (auto u: graph)
  {
    std::cout << "  " << i++ << " : [ ";
    for (auto v: u)
    {
      std::cout << v.second << " ";
    }
    std::cout << "]\n";
  }
  std::cout << "]\n";
}
