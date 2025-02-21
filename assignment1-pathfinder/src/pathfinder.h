#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "vec.hpp"
#include "draw-triangle-pro.hpp"
#include "raylib-cpp.hpp"
//#include "graph.hpp"
#include "graph-utils.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

class Pathfinder
{
private:
    const int w{ 2880 / 2 }, h{ 1620 / 2 };
    const float half_w{ w / 2.0f }, half_h{ h / 2.0f }, gap{ w / 8.0f };
    int m_startTime;
    int m_time{ 60 };
    int m_tokens{ 2000 };
    int m_score;
    int m_highScore;
    
    node_t m_start;
    node_t m_end;

    std::vector<node_t> m_playerPath;

    Graph m_graph;
    raylib::Window m_window{ w, h, "Pathfinder" };
    raylib::AudioDevice m_audio;
    raylib::Sound m_coinSound{ "../../deps/raylib-cpp/examples/audio/resources/coin.wav" };
public:
    Pathfinder();
    ~Pathfinder();

    void intitialise();

    bool is_connected(node_t node1, node_t node2);

    unsigned int path_cost(const std::vector<node_t>& path);

    std::vector<node_t> astar_pathfind(const Graph& g, node_t start, node_t goal);
    
    void run();
};


#endif // !PATHFINDER_H

