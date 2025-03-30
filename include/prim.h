#pragma once

#include <unordered_map>
#include <vector>
#include "graph.h"

template <std::integral WeightType
, std::integral VertexType
, std::integral IntegralType
>
std::unordered_map <VertexType, std::vector <std::pair <WeightType, VertexType >>>
primMST (
    const Graph <WeightType, VertexType >* i_graph
    , const IntegralType start_v
    );

template <std::integral ReturnType, std::integral WeightType, std::integral VertexType>
ReturnType
primMST_weight (const Graph <WeightType, VertexType >& graph
    , VertexType start_v);

#include "prim.tpp"
