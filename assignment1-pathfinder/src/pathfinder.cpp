#include "pathfinder.h"


Pathfinder::Pathfinder()
{
}

Pathfinder::~Pathfinder()
{
}

void Pathfinder::intitialise()
{
    SetTargetFPS(60);

    m_graph.nodes.clear();

    add_node(m_graph, 'A', { half_w - gap, half_h });
    add_node(m_graph, 'B', { half_w, half_h });
    add_node(m_graph, 'C', { half_w, half_h - gap });
    add_node(m_graph, 'D', { half_w, half_h + gap });
    add_node(m_graph, 'E', { half_w + gap, half_h + gap });
    add_node(m_graph, 'F', { half_w + gap, half_h });
    add_node(m_graph, 'G', { half_w + (2 * gap), half_h - gap });
    add_double_edge(m_graph, 'A', 'B');
    add_double_edge(m_graph, 'B', 'C');
    add_double_edge(m_graph, 'B', 'D');
    add_double_edge(m_graph, 'C', 'A');
    add_double_edge(m_graph, 'D', 'E');
    add_double_edge(m_graph, 'D', 'A');
    add_double_edge(m_graph, 'E', 'B');
    add_double_edge(m_graph, 'B', 'F');
    add_double_edge(m_graph, 'C', 'F');
    add_double_edge(m_graph, 'C', 'G');
    add_double_edge(m_graph, 'F', 'G');

    m_startTime = GetTime();
    m_start = 'A';
    m_end = 'G';

    m_playerPath.clear();

    m_playerPath.push_back(m_start); // initialize the player_path with start node

    m_tokens = 2000;
    m_score = 0;
}

bool Pathfinder::is_connected(node_t node1, node_t node2)
{
    // Check if an edge exists between node 1 and node 2
    return edge_info.find(std::pair{ node1, node2 }) != edge_info.end();
}

unsigned int Pathfinder::path_cost(const std::vector<node_t> &path)
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

std::vector<node_t> Pathfinder::astar_pathfind(const Graph &g, node_t start, node_t goal)
{
    std::unordered_map<node_t, node_t> came_from;
    std::unordered_map<node_t, double> cost_so_far;
    a_star_search(g, start, goal, came_from, cost_so_far);
    std::vector<node_t> path = reconstruct_path(start, goal, came_from);
    return path;
}

void Pathfinder::run()
{
    while (!m_window.ShouldClose()) // Detect window close button or ESC key
    {
        BeginDrawing();

        int elapsed_time = GetTime() - m_startTime;
        int remaining_time = m_time - elapsed_time;

        if (remaining_time <= 0) {
            remaining_time = 0; // Update to add game over and update the game / scores
        }

        DrawText(TextFormat("Tokens: %04i", m_tokens), 5, 5, 20, BLUE);
        DrawText(TextFormat("Score: %06i", m_score), 5, 45, 20, BLUE);
        DrawText(TextFormat("Highscore: %06i", m_highScore), 5, 85, 20, BLUE);
        DrawText(TextFormat("Time: %02i secs", remaining_time), 5, 125, 20, BLACK);

        ClearBackground(LIGHTGRAY);

        draw_graph(m_graph);

        DrawCircleV(node_info[m_start], node_radius * 1.1, GREEN); // Colour starting node green
        DrawCircleV(node_info[m_end], node_radius * 1.1, RED); // Colour the end node red

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (auto opt = get_nearby_node(GetMousePosition()))
            {
                node_t clickedNode = *opt;

                // Check if the node is connected to previous node
                if (is_connected(m_playerPath.back(), clickedNode))
                {
                    if (clickedNode == m_end) {
                        m_playerPath.push_back(clickedNode);
                        m_coinSound.Play();//plays coin audio
                        m_tokens = m_tokens - path_cost(m_playerPath);
                        // Add end game update
                        m_score = m_tokens;

                        if (m_score > m_highScore) {
                            m_highScore = m_score;
                        }
                    }
                    else
                    {
                        m_playerPath.push_back(clickedNode);
                        m_coinSound.Play();//plays coin audio
                        m_tokens = m_tokens - path_cost(m_playerPath); // remove path cost from tokens
                    }
                }
                if (clickedNode == m_end || m_tokens <= 0)
                {
                    // Add score/update high score, reward player with tokens and reset
                }
            }
        }

        for (size_t i = 0; i + 1 < m_playerPath.size(); ++i)
        {
            // Highlight the edge between nodes
            DrawLineEx(node_info[m_playerPath[i]], node_info[m_playerPath[i + 1]], 5, YELLOW);
        }

        EndDrawing();
    }
}
