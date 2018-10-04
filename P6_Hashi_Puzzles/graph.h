#include <vector>

// ====================================================================================
// A Point is just a 2D coordinate.

class Point {
public:
    // constructor
    Point(int x_, int y_);
    // accessors
    int get_x() const { return x; }
    int get_y() const { return y; }
    bool equals(const Point &p) const { return x==p.x && y==p.y; }
    bool is_neighbor(const Point &p) const { return x==p.x || y==p.y; }
private:
    // representation
    int x;
    int y;
};

// helper function for printing Points
std::ostream& operator<<(std::ostream &ostr, const Point &p);

// ====================================================================================
// A Node is a Point with a count indicating the number of Edges that should touch it.

class Node {
public:
    Node(const Point &p, int n);
    // accessors
    const Point& get_pt() const { return pt; }
    int get_num_paths() const { return num_paths; }
    bool equals(const Node &n) const { return pt.equals(n.pt) && num_paths==n.num_paths; }
    bool is_neighbor(const Node &n) const { return get_pt().is_neighbor(n.get_pt()); }
    // is this node included in nodes_?
    bool included(const std::vector<Node> &nodes_) const {
        for( unsigned int i=0; i<nodes_.size(); i++ )
            if( this->equals(nodes_[i]) )
                return true;
        return false;
    }
    // modifiers
    int reduce_num_paths() { return --num_paths; }
private:
    Point pt;
    int num_paths;
};

// helper function for printing a Node
std::ostream& operator<<(std::ostream &ostr, const Node &n);
// helper function for printing a list of Nodes
std::ostream& operator<<(std::ostream &ostr, const std::vector<Node> &nodes);

// ====================================================================================
// An Edge is a vertical or horizontal line segment connecting 2 points.

class Edge {
public:
    // constructor
    Edge(const Point &a_, const Point &b_) : a(a_), b(b_) { assert_edge(a_, b_); };
    Edge(const Node &a_, const Node &b_) : a(a_.get_pt()), b(b_.get_pt()) {
        assert_edge(a_.get_pt(), b_.get_pt());
    };
    // accessors
    const Point& get_a() const { return a; }
    const Point& get_b() const { return b; }
    const Point& get_other(const Point &p) const { return p.equals(a) ? b : a; }
    bool equals(const Edge &e) const {
        return ( a.equals(e.a) && b.equals(e.b) ) || ( a.equals(e.b) && b.equals(e.a) );
    }
    bool vertical() const { return a.get_x() == b.get_x(); }
    bool horizontal() const { return a.get_y() == b.get_y(); }
    // does this edge cross that?
    bool crosses(const Edge &that) const;
    bool crosses(const std::vector<Edge> &bridges) const {
        for(unsigned int i=0; i<bridges.size(); i++)
            if(crosses(bridges[i]))
                return true;
        return false;
    }
    // is this edge included in bridges?
    bool included(const std::vector<Edge> bridges) const {
        for(unsigned int i=0; i<bridges.size(); i++)
            if(this->equals(bridges[i]))
               return true;
        return false;
    }

private:
    // representation
    Point a;
    Point b;
    // helper function
    void assert_edge(const Point &a_, const Point &b_);
};

// helper function for printing Edges
std::ostream& operator<<(std::ostream &ostr, const Edge &e);
std::ostream& operator<<(std::ostream &ostr, const std::vector<Edge> &edges);

// ====================================================================================
// A Graph is a collection of Nodes and Edges.

class Graph {
public:
    // constructor
    Graph();
    // modifiers
    void addNode(const Point &p, int n);
    void addEdge(const Point &a, const Point &b);
    // print helper functions
    void printSolution() const;
    void printGraph() const;
    // hashi
    bool empty() const { return nodes.empty(); }
    void hashi(bool connected, bool find_all_solutions);
private:
    // representation
    int max_x;
    int max_y;
    std::vector<Node> nodes;
    std::vector<Edge> edges;
    
    // helper functions
    void reduce_node(const Point &p);
    void reduce(const std::vector<Edge> &stepwise_solution, const Node &node);
    void hashi_r(Graph subgraph, std::vector<Edge> partial_solution,
                 bool connected, bool find_all_solutions);
    std::vector<std::vector<Edge>> stepwise(const Graph &subgraph, const Node &node,
                                            const std::vector<Edge> &p_solutions);
    bool bridge_crosses_nodes(const Edge &bridge) const;
    bool connected() const;
    void reach_nodes(std::vector<Node> &reached_nodes, unsigned int tried_index) const;
    void printPartialGraph(const std::vector<Edge> &partial_solution, const Graph &remaining_graph);
};

// ====================================================================================
