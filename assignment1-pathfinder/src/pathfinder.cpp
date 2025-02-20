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

void Pathfinder::pop_last_node()
{
    node_t n1 = m_playerPath.back();
    m_playerPath.pop_back();
    node_t n2 = m_playerPath.back();

    std::vector<node_t> pathCostList;
    pathCostList.push_back(n1);
    pathCostList.push_back(n2);

    m_tokens = m_tokens + path_cost(pathCostList); // refind path cost to tokens
    m_curr = m_playerPath.back();
}

void Pathfinder::push_node(node_t node)
{
    node_t n1 = m_playerPath.back();
    node_t n2 = node;

    std::vector<node_t> pathCostList;
    pathCostList.push_back(n1);
    pathCostList.push_back(n2);

    m_playerPath.push_back(node);
    m_coinSound.Play(); // plays coin audio
    m_tokens = m_tokens - path_cost(pathCostList); // remove path cost from tokens
}

bool Pathfinder::node_clicked(node_t node)
{
    for (auto n : m_playerPath)
    {
        if (n == node)
        {
            return true;
        }
    }


    return false;
}

void Pathfinder::calc_score()
{
    std::vector<node_t> idealPath = astar_pathfind(m_graph, m_start, m_end);

    int idealCost = path_cost(idealPath);
    int currCost = path_cost(m_playerPath);

    m_score = idealCost - (idealCost - currCost);

    if (m_score > m_highScore)
    {
        m_highScore = m_score;
    }
}

void Pathfinder::proc_node_click(node_t clickedNode)
{
    if (clickedNode == m_curr)
    {
        pop_last_node();
        return;
    }

    if (node_clicked(clickedNode))
    {
        return;
    }

    // Check if the node is connected to previous node
    if (is_connected(m_playerPath.back(), clickedNode))
    {
        push_node(clickedNode);
        m_curr = clickedNode;
    }

    if (m_curr == m_end || m_tokens <= 0)
    {
        calc_score();

        intitialise();
    }
}

void Pathfinder::run()
{
    while (!m_window.ShouldClose()) // Detect window close button or ESC key
    {
        m_elapsedTime = GetTime() - m_startTime;
        m_remainingTime = m_time - m_elapsedTime;

        if (m_remainingTime <= 0) {
            calc_score();
            intitialise();
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (auto opt = get_nearby_node(GetMousePosition()))
            {
                node_t clickedNode = *opt;
                proc_node_click(clickedNode);
            }
        }

        draw();
    }
}


void Pathfinder::draw()
{
    BeginDrawing();

    DrawText(TextFormat("Tokens: %04i", m_tokens), 5, 5, 20, BLUE);
    DrawText(TextFormat("Score: %06i", m_score), 5, 45, 20, BLUE);
    DrawText(TextFormat("Highscore: %06i", m_highScore), 5, 85, 20, BLUE);
    DrawText(TextFormat("Time: %02i secs", m_remainingTime), 5, 125, 20, BLACK);

    ClearBackground(LIGHTGRAY);

    draw_graph(m_graph);

    DrawCircleV(node_info[m_start], node_radius * 1.1, GREEN); // Colour starting node green
    DrawCircleV(node_info[m_end], node_radius * 1.1, RED); // Colour the end node red

    for (size_t i = 0; i + 1 < m_playerPath.size(); ++i)
    {
        // Highlight the edge between nodes
        DrawLineEx(node_info[m_playerPath[i]], node_info[m_playerPath[i + 1]], 5, YELLOW);
    }

    EndDrawing();
}
