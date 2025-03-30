#pragma once
#include <unordered_map>
#include <vector>

template <std::integral WeightType, std::integral VertexType>
class Graph
{
  public:
    Graph () = default;
    virtual ~Graph () = default;


    virtual void add_edge (WeightType w, VertexType v, VertexType u) = 0;
    virtual void add_undirected_edge (WeightType w, VertexType v, VertexType u) = 0;
    // 取得所有頂點數
    virtual size_t vertex_count () const = 0;
    // 取得圖的鄰接表表示（所有類型的 Graph 都應該能轉換成這個格式）
    virtual std::unordered_map <VertexType, std::vector <std::pair <WeightType, VertexType >>>
      get_adj_list () const = 0;
};


template <std::integral WeightType, std::integral VertexType>
class Unordered_map_weighted_adj_list : public Graph<WeightType, VertexType>
{
  private:
    std::unordered_map <size_t, std::vector <std::pair <WeightType, VertexType>>> graph;

  public:
    Unordered_map_weighted_adj_list (size_t n);

    void
      add_edge (WeightType w, VertexType u, VertexType v) override;

    void
      add_undirected_edge (WeightType w, VertexType u, VertexType v) override;

    Unordered_map_weighted_adj_list <WeightType, VertexType>
      get_adj_list() const override;

    size_t
      vertex_count() const override;
};

template <std::integral WeightType, std::integral VertexType>
class Vector_weighted_adj_list : public Graph <WeightType, VertexType>
{
  private:
    std::vector <std::vector <std::pair <WeightType, VertexType >>> graph;

  public:
    Vector_weighted_adj_list(size_t n);

    void
      add_edge (WeightType w, VertexType u, VertexType v) override;

    void
      add_undirected_edge (WeightType w, VertexType u, VertexType v) override;

    std::unordered_map<VertexType, std::vector<std::pair<WeightType, VertexType>>>
      get_adj_list() const override;

    size_t
      vertex_count() const override;
};

#include "unordered_map_weighted_adj_list.tpp"
#include "vector_weighted_adj_list.tpp"

