#include <string>
#include <iostream>
#include <cassert>
#include "graph.h"

// Ray

unsigned int min( unsigned int x, unsigned int y);
bool in_middle( unsigned int x, unsigned int end1, unsigned int end2 );

void Graph::hashi(bool connected, bool find_all_solutions) {
    std::vector<Edge> partial_solution;
    
    hashi_r(*this, partial_solution, connected, find_all_solutions);
}

// The recursive function
void
Graph::hashi_r
(
 Graph remaining_graph, // remaining graph
 std::vector<Edge> partial_solution,
 bool to_be_connected,
 bool find_all_solutions
)
{
    static unsigned int solution_id = 0;
    
    if( !find_all_solutions && solution_id>0 )
        return;
    
    std::cout << "\nRecursive call to hashi_r ========================================= \n"
    << "    partial_solution: " << partial_solution << "\n"
    << "    remaining_graph: " << remaining_graph.nodes << "\n";
    printPartialGraph(partial_solution, remaining_graph);
    
    // If remaning_graph is empty, we have found a solution. Print it.
    if( remaining_graph.empty() ) {
        // put partial_solution into this graph so it can be properly processed.
        edges = partial_solution;
        if( !to_be_connected || connected() ) {
            ++solution_id;
            std::cout << "SOLUTION " << solution_id << "===========" << std::endl;
            printSolution();
            printGraph();
        }
        else
            std::cout << "SOLUTION FOUND but ruled out since it's not conneced." << std::endl;
        return;
    }
    
    // take the 1st node from remaning_graph
    std::vector<Node> &r_nodes = remaining_graph.nodes;
    Node node = r_nodes[0];
    r_nodes.erase(r_nodes.begin()); // can use pop_back() instead so it's faster
    
    // figure out all the stepwise solutions, each of which is a possible set of bridges
    // the node has with its neighbors
    std::vector<std::vector<Edge>> stepwise_solutions
        = stepwise(remaining_graph, node, partial_solution);
    
    // If no more stepwise solution, the exploration has failed. Just return.
    // Otherwise, for each stepwise solution, add it to partial_solution,
    // deduct it from remaining graph, and call hashi_r recursively.
    for( unsigned int i = 0; i < stepwise_solutions.size(); i++ )    {
        std::vector<Edge> solution = stepwise_solutions[i];
        // make a copy of partial_solution and remaining_graph, update them,
        // and pass them into the recursive call.
        std::vector<Edge> partial_solution_1 = partial_solution;
        Graph remaining_graph_1 = remaining_graph;
        for( unsigned int j = 0; j < solution.size(); j++ )
            partial_solution_1.push_back(solution[j]);
        remaining_graph_1.reduce(solution, node);
        hashi_r(remaining_graph_1, partial_solution_1, to_be_connected, find_all_solutions);
    }

    std::cout << "\nRETURN from hashi_r ========================================= \n"
    << "    partial_solution: " << partial_solution << "\n"
    << "    remaining_graph: " << remaining_graph.nodes << "\n";
}

// figure out all the stepwise solutions, each of which is a possible set of bridges
// the node has with its neighbors within the subgraph
std::vector<std::vector<Edge>>
Graph::stepwise(const Graph &subgraph, const Node &node, const std::vector<Edge> &p_solution) {
    std::vector<std::vector<Edge>> solutions;

    std::cout << "  Stepwise() with Node " << node << std::endl;

    // find all its neighbors, except whose resulting bridge crosses p_solutions
    const std::vector<Node> &s_nodes = subgraph.nodes;
    std::vector<Node> neighbors;
    for(unsigned int i=0; i<s_nodes.size(); i++) {
        if(s_nodes[i].is_neighbor(node))    {
            Edge bridge(node, s_nodes[i]);
            if(subgraph.bridge_crosses_nodes(bridge)) {
                // std::cout << "  BRIDGE " << bridge << " CROSSES NODES" << std::endl;
                continue;
            }
            if(bridge.crosses(p_solution)) {
                // std::cout << "  BRIDGE " << bridge << " CROSSES p_solution" << std::endl;
                continue;
            }
            neighbors.push_back(s_nodes[i]);
        }
    }
    std::cout << "    Neighbors: " << neighbors << std::endl;
    
    // find all stepwise solutions
    unsigned int num_bridges = node.get_num_paths();
    assert( 0 < num_bridges );
    assert( neighbors.size() <= 4 );
    
    // how many ways can we allocate num_bridges over its neighbors
    // so each neighbor gets 0 or 1 or 2 but no more than its num_paths?
     if( neighbors.empty() ) {
         std::cout << "    NO Stepwise solution: neighbors=0\n";
     } else if( neighbors.size() == 1 && num_bridges <= 2 ) {
         Node neighbor = neighbors[0];
         unsigned int num_paths = neighbor.get_num_paths();
         assert( 0 < num_paths );
         for( unsigned int north=1; north<=min(num_paths,2); north++ ) {
             if( north == num_bridges ) {
                 std::cout << "    Stepwise solution: " << north << "\n";
                 std::vector<Edge> solution;
                 for( unsigned int i=0; i<north; i++ ){
                     Edge e(node, neighbors[0]);
                     solution.push_back( e );
                 }
                 solutions.push_back( solution );
             }
         }
         if(solutions.empty())
             std::cout << "    NO Stepwise solution: neighbors=1, bridges=" << num_bridges << "\n";
     } else if( neighbors.size() == 2  && num_bridges <= 4 ) {
         for( unsigned int north=0; north<=min(neighbors[0].get_num_paths(),2); north++ )
             for( unsigned int east=0; east<=min(neighbors[1].get_num_paths(),2); east++ )
                 if( north+east == num_bridges ) {
                     std::cout << "    Stepwise solution: " << north << ", "
                     << east << "\n";
                     std::vector<Edge> solution;
                     for( unsigned int i=0; i<north; i++ ){
                         Edge e(node, neighbors[0]);
                         solution.push_back( e );
                     }
                     for( unsigned int i=0; i<east; i++ ){
                         Edge e(node, neighbors[1]);
                         solution.push_back( e );
                     }
                     solutions.push_back( solution );
                 }
         if(solutions.empty())
             std::cout << "    NO Stepwise solution: neighbors=2, bridges=" << num_bridges << "\n";
     } else if( neighbors.size() == 3  && num_bridges <= 6 ) {
         for( unsigned int north=0; north<=min(neighbors[0].get_num_paths(),2); north++ )
             for( unsigned int east=0; east<=min(neighbors[1].get_num_paths(),2); east++ )
                 for( unsigned int south=0; south<=min(neighbors[2].get_num_paths(),2); south++ )
                     if( north+east+south == num_bridges ) {
                         std::cout << "    Stepwise solution: " << north << ", "
                         << east << ", " << south << "\n";
                         std::vector<Edge> solution;
                         for( unsigned int i=0; i<north; i++ ){
                             Edge e(node, neighbors[0]);
                             solution.push_back( e );
                         }
                         for( unsigned int i=0; i<east; i++ ){
                             Edge e(node, neighbors[1]);
                             solution.push_back( e );
                         }
                         for( unsigned int i=0; i<south; i++ ){
                             Edge e(node, neighbors[2]);
                             solution.push_back( e );
                         }
                         solutions.push_back( solution );
                     }
         if(solutions.empty())
             std::cout << "    NO Stepwise solution: neighbors=3, bridges=" << num_bridges << "\n";
     } else if( neighbors.size() == 4  && num_bridges <= 8 ) {
         for( unsigned int north=0; north<=min(neighbors[0].get_num_paths(),2); north++ )
             for( unsigned int east=0; east<=min(neighbors[1].get_num_paths(),2); east++ )
                 for( unsigned int south=0; south<=min(neighbors[2].get_num_paths(),2); south++ )
                     for( unsigned int west=0; west<=min(neighbors[3].get_num_paths(),2); west++ )
                         if( north+east+south+west == num_bridges ) {
                             std::cout << "    Stepwise solution: " << north << ", "
                             << east << ", "
                             << south << ", " << west << "\n";
                             std::vector<Edge> solution;
                             for( unsigned int i=0; i<north; i++ ) {
                                 Edge e(node, neighbors[0]);
                                 solution.push_back( e );
                             }
                             for( unsigned int i=0; i<east; i++ ){
                                 Edge e(node, neighbors[1]);
                                 solution.push_back( e );
                             }
                             for( unsigned int i=0; i<south; i++ ){
                                 Edge e(node, neighbors[2]);
                                 solution.push_back( e );
                             }
                             for( unsigned int i=0; i<west; i++ ){
                                 Edge e(node, neighbors[3]);
                                 solution.push_back( e );
                             }
                             solutions.push_back( solution );
                         }
         if(solutions.empty())
             std::cout << "    NO Stepwise solution: neighbors=4, bridges=" << num_bridges << "\n";
     } else {
         std::cout << "    NO Stepwise solution: neighbors=" << neighbors.size() << " bridges="
                     << num_bridges << "\n";
     }

    return solutions;
}

// Remove all edges in stepwise_solution from the graph
void Graph::reduce(const std::vector<Edge> &stepwise_solution, const Node &node) {
    for( unsigned int i = 0; i < stepwise_solution.size(); i++ ) {
        const Edge &edge = stepwise_solution[i];
        // find the other node, and reduce its num_paths.
        // If num_paths reaches 0, remove the node.
        const Point &other_point = edge.get_other(node.get_pt());
        reduce_node(other_point);
    }
}

// Use p, find the node in this graph, and decrement its num_paths.
// If num_paths reaches 0, remove the node.
void Graph::reduce_node(const Point &p) {
    for( unsigned int i=0; i<nodes.size(); i++) {
        Node & node = nodes[i];
        if(node.get_pt().equals(p)) {
            if( node.reduce_num_paths() == 0 )
                nodes.erase( nodes.begin() + i );
            return;
        }
    }
    assert(false);
}

// Test to see if bridge crosses any nodes in the graph
bool Graph::bridge_crosses_nodes(const Edge &bridge) const {
    for( unsigned int i=0; i<nodes.size(); i++ ) {
        const Point &point = nodes[i].get_pt();
        const Point &end1 = bridge.get_a();
        const Point &end2 = bridge.get_b();
        if( end1.get_x() == end2.get_x() && end2.get_x() == point.get_x()
           && in_middle(point.get_y(), end1.get_y(), end2.get_y()) ){
            //std::cout << "In_middle: " << point << " between " << end1 << " and " << end2 << "\n";
            return true;
        }
        if( end1.get_y() == end2.get_y() && end2.get_y() == point.get_y()
           && in_middle(point.get_x(), end1.get_x(), end2.get_x()) ){
            //std::cout << "In_middle: " << point << " between " << end1 << " and " << end2 << "\n";
            return true;
        }
    }
    return false;
}

// min(x, y)
unsigned int min( unsigned int x, unsigned int y) {
    return (x > y) ? y : x;
}

// Test to see if x is between end1 and end2
bool in_middle( unsigned int x, unsigned int end1, unsigned int end2 ) {
    assert( end1 != end2 );
    if( end1 < end2 )
        return end1 < x && x < end2;
    else
        return end1 > x && x > end2;
}

// Check to see if this bridge crosses that
bool Edge::crosses(const Edge &that) const {
    if( this->vertical() && that.horizontal() ) {
        assert( this->get_a().get_x() == this->get_b().get_x() );
        unsigned int this_x = this->get_a().get_x();
        assert( that.get_a().get_y() == that.get_b().get_y() );
        unsigned int that_y = that.get_a().get_y();
        return in_middle( that_y, this->get_a().get_y(), this->get_b().get_y() )
            && in_middle( this_x, that.get_a().get_x(), that.get_b().get_x() );
    }
    if( this->horizontal() && that.vertical() ) {
        assert( this->get_a().get_y() == this->get_b().get_y() );
        unsigned int this_y = this->get_a().get_y();
        assert( that.get_a().get_x() == that.get_b().get_x() );
        unsigned int that_x = that.get_a().get_x();
        return in_middle( this_y, that.get_a().get_y(), that.get_b().get_y() )
            && in_middle( that_x, this->get_a().get_x(), this->get_b().get_x() );
    }
    return false;
}

// Add indexes to nodes reachable from Node #tried_index to reached_nodes
void Graph::reach_nodes(std::vector<Node> &reached_nodes, unsigned int tried_index) const {
    const Node &node = reached_nodes[tried_index];
    for( unsigned int i=0; i<nodes.size(); i++ ){
        const Node &g_node = nodes[i];
        if( !node.equals(g_node) && node.is_neighbor(g_node) && !g_node.included(reached_nodes)) {
            Edge e(node, g_node);
            if( e.included(edges) ) {
                reached_nodes.push_back(g_node);
            }
        }
    }
}

// Check to see if this graph is connected
bool Graph::connected() const {
    unsigned int size = nodes.size();
    assert( size > 1 );
    std::vector<Node> reached_nodes;
    reached_nodes.push_back(nodes[0]);
    for( unsigned int tried_index=0; tried_index<reached_nodes.size(); tried_index++ ){
        reach_nodes(reached_nodes, tried_index);
    }
    return reached_nodes.size() == nodes.size();
}

bool included(const Point &p, const std::vector<Node> &nodes_) {
    for( unsigned int i=0; i<nodes_.size(); i++ )
        if( p.equals(nodes_[i].get_pt()) )
            return true;
    return false;
}

void Graph::printPartialGraph(const std::vector<Edge> &partial_solution, const Graph &remaining_graph) {
    Graph g;
    for(unsigned int i=0; i<remaining_graph.nodes.size(); i++)
        g.addNode(remaining_graph.nodes[i].get_pt(), remaining_graph.nodes[i].get_num_paths());
    for(unsigned int i=0; i<partial_solution.size(); i++){
        const Point &a = partial_solution[i].get_a();
        const Point &b = partial_solution[i].get_b();
        if(!included(a, g.nodes))
            g.addNode(a, 0);
        if(!included(b, g.nodes))
            g.addNode(b, 0);
        g.addEdge(a, b);
    }
    g.printGraph();
}

