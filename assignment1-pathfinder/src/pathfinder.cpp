#include "vec.hpp"
#include "draw-triangle-pro.hpp"
#include "raylib-cpp.hpp"
#include "graph.hpp"
#include "graph-utils.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

std::vector<node_t> astar_pathfind(const Graph& g, node_t start, node_t goal)
{
    std::unordered_map<node_t, node_t> came_from;
    std::unordered_map<node_t, double> cost_so_far;
    a_star_search(g, start, goal, came_from, cost_so_far);
    std::vector<node_t> path = reconstruct_path(start, goal, came_from);
    return path;
}

unsigned int path_cost(const std::vector<node_t>& path)
{
    double dcost = 0;
    if (path.size() >= 2) // then we have some lines to draw
    {
        const int num_edges = path.size() - 1;
        for (int i = 0; i < num_edges; i++)
        {
            dcost = dcost + edge_info[std::pair{ path[i], path[i + 1] }];
        }
    }

    return static_cast<unsigned int>(dcost);
}

bool is_connected(const Graph& g, node_t node1, node_t node2) {
    // Check if an edge exists between node 1 and node 2
    return edge_info.find(std::pair{ node1, node2 }) != edge_info.end();
}

int main()
{
    const int w{ 2880 / 2 }, h{ 1620 / 2 }, half_w{ w / 2 }, half_h{ h / 2 }, gap{ w / 8 };
    raylib::Window window{ w, h, "Pathfinder" };

    SetTargetFPS(60);

    Graph g;
    add_node(g, 'A', { half_w - gap, half_h });
    add_node(g, 'B', { half_w, half_h });
    add_node(g, 'C', { half_w, half_h - gap });
    add_node(g, 'D', { half_w, half_h + gap });
    add_node(g, 'E', { half_w + gap, half_h + gap });
    add_node(g, 'F', { half_w + gap, half_h });
    add_node(g, 'G', { half_w + (2 * gap), half_h - gap });
    add_double_edge(g, 'A', 'B');
    add_double_edge(g, 'B', 'C');
    add_double_edge(g, 'B', 'D');
    add_double_edge(g, 'C', 'A');
    add_double_edge(g, 'D', 'E');
    add_double_edge(g, 'D', 'A');
    add_double_edge(g, 'E', 'B');
    add_double_edge(g, 'B', 'F');
    add_double_edge(g, 'C', 'F');
    add_double_edge(g, 'C', 'G');
    add_double_edge(g, 'F', 'G');

    raylib::AudioDevice audio;
    raylib::Sound coin("../../deps/raylib-cpp/examples/audio/resources/coin.wav"); // Set coin sound

    int t{ 60 }; // time
    int start_time = GetTime();
    node_t start = 'A';
    node_t end = 'G';
    std::vector<node_t> player_path{ start }; // initialize the player_path with start node
    int tokens{ 2000 }, score{}, high_score{}; // try with more/less tokens?

    while (!window.ShouldClose()) // Detect window close button or ESC key
    {
        BeginDrawing();

        int elapsed_time = GetTime() - start_time;
        int remaining_time = t - elapsed_time;

        if (remaining_time <= 0) {
            remaining_time = 0; // Update to add game over and update the game / scores
        }


        DrawText(TextFormat("Tokens: %04i", tokens), 5, 5, 20, BLUE);
        DrawText(TextFormat("Score: %06i", score), 5, 45, 20, BLUE);
        DrawText(TextFormat("Highscore: %06i", high_score), 5, 85, 20, BLUE);
        DrawText(TextFormat("Time: %02i secs", remaining_time), 5, 125, 20, BLACK);

        ClearBackground(LIGHTGRAY);

        draw_graph(g);

        DrawCircleV(node_info[start], node_radius * 1.1, GREEN); // Colour starting node green
        DrawCircleV(node_info[end], node_radius * 1.1, RED); // Colour the end node red

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (auto opt = get_nearby_node(GetMousePosition()))
            {
                node_t clickedNode = *opt;

                // Check if the node is connected to previous node
                if (is_connected(g, player_path.back(), clickedNode))
                {
                    if (clickedNode == end) {
                        player_path.push_back(clickedNode);
                        coin.Play();//plays coin audio
                        tokens = tokens - path_cost(player_path);
                        // Add end game update
                        score = tokens;
                        if (score > high_score) {
                            high_score = score;
                        }
                    }
                    else
                    {
                        player_path.push_back(clickedNode);
                        coin.Play();//plays coin audio
                        tokens = tokens - path_cost(player_path); // remove path cost from tokens
                    }
                }
                if (clickedNode == end || tokens <= 0)
                {
                    // Add score/update high score, reward player with tokens and reset
                }
            }
        }

        for (size_t i = 0; i + 1 < player_path.size(); ++i)
        {
            // Highlight the edge between nodes
            DrawLineEx(node_info[player_path[i]], node_info[player_path[i + 1]], 5, YELLOW);
        }

        EndDrawing();
    }

    return 0;
}
