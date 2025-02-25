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
    int m_remainingTime;
    int m_elapsedTime;
    int m_time{ 60 };
    int m_tokens{ 2000 };
    int m_score;
    int m_highScore;

    bool m_gameOver;
    
    node_t m_start;
    node_t m_end;
    node_t m_curr;

    std::vector<node_t> m_playerPath;
    std::vector<node_t> m_availableNodes;

    Graph m_graph;
    raylib::Window m_window{ w, h, "Pathfinder" };
    raylib::AudioDevice m_audio;
    raylib::Sound m_coinSound{ "../../deps/raylib-cpp/examples/audio/resources/coin.wav" };

    unsigned int path_cost(const std::vector<node_t>& path);                                // calculates the cost of a given path

    std::vector<node_t> astar_pathfind(const Graph& g, node_t start, node_t goal);          // calculates the most optimal path using the a* pathfinding algorithm

    bool is_connected(node_t node1, node_t node2);                                          // checks wheather any given two nodes/points are connected with one another

    void reset();                                                                           // reset the current game

    void set_startend_nodes();                                                              // initialises the start and end nodes for the current game

    node_t get_random_node();                                                               // gets a random node from the list of available nodes

    void pop_last_node();                                                                   // removes the last node from the player path

    void push_node(node_t node);                                                            // adds a new node to the player path

    bool node_clicked(node_t node);                                                         // checks to see whether the given node has been clicked

    int calc_score();                                                                       // calculates the score for the current game

    void proc_node_click(node_t node);                                                      // processes a click event for a given node.

    void update_time();                                                                     // updates game time / timers.

public:
    Pathfinder();               //Constructor 

    ~Pathfinder();              //Destructor

    void Initialise();          //Initialise
   
    void Run();                 //Run loop

    void Draw();                // Render/Draw call
};


#endif // !PATHFINDER_H

