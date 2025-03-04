#ifndef _GRAPH_HPP_
#define _GRAPH_HPP_

#include <numeric>
#include <unordered_map>
#include <vector>
#include <utility>        // std::pair
#include <cmath>
#include <functional>

#include "implementation.hpp" // in redblob-games

// We need this class template specialisation of std::hash before declaring
// std::unordered_map variables involving a std::pair; such as edge_t.

namespace std
{
  template <typename T, typename U>
  struct hash<std::pair<T,U>>
  {
    std::size_t operator()(const std::pair<T,U>& n) const
    {
      return std::hash<T>{}(n.first) ^ std::hash<U>{}(n.second);
    }
  };
}

using node_t  = char;
using edge_t  = std::pair<node_t, node_t>;
using coord_t = ai::Vector2;
inline std::unordered_map<node_t, coord_t> node_info;
inline std::unordered_map<edge_t, double>  edge_info;

const float line_thickness = 5.0f;
const float node_radius    = line_thickness * 2.0f;

inline double heuristic(node_t, node_t);


// neighbors and cost are needed by a_star_search
struct Graph
{
  // std::unordered_map<node_t, std::pair<coord_t, std::vector<node_t>>> ?
  // That would remove the need for node_info. Less readable?
  std::unordered_map<node_t, std::vector<node_t>> nodes; // node + its n'bours

  std::vector<node_t> neighbors(node_t id)
  {
    return nodes[id];
  }

  double cost(node_t current, node_t next)
  {
    return edge_info[{current,next}];
  }
};

inline double calc_distance(const coord_t& npos, const coord_t& gpos)
{
    return std::sqrt(std::pow(npos.x - gpos.x, 2) + std::pow(npos.y - gpos.y, 2)); // computes the distance using euclidean distance 
    //return std::abs(npos.x - gpos.x) + std::abs(npos.y - gpos.y);   // compute distance using manhattan distance
}

inline double heuristic(node_t next, node_t goal)
{
    const coord_t& npos = node_info[next]; // find the postion of the next node 
    const coord_t& gpos = node_info[goal]; // find the postion of the end node 
    return calc_distance(npos, gpos);
    //return std::sqrt(std::pow(npos.x - gpos.x, 2) + std::pow(npos.y - gpos.y, 2)); // computes the distance using euclidean distance 
}



#endif // _GRAPH_HPP_
