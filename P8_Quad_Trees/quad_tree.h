// ===================================================================
//
// We provide the Point class and the implementation of several
// QuadTree member functions for printing.  
//
// IMPORTANT: You should modify this file to add all of the necessary
// functionality for the QuadTree class and its helper classes: Node,
// DepthIterator, and BreadthIterator.
//
// ===================================================================

#ifndef quad_tree_h_
#define quad_tree_h_

#include <iostream>
#include <vector>
#include <list>
#include <cassert>


// ==============================================================
// ==============================================================
// A tiny templated class to store 2D coordinates.  This class works
// with number_type = int, float, double, unsigned char (a 1 byte=8
// bit integer), short (a 2 byte=16 bit integer).

template <class number_type>
class Point {
public:
    Point(const number_type& x_, const number_type& y_) : x(x_), y(y_) {}
    Point(const Point& p) : x(p.x), y(p.y) {}
    // REPRESENTATION
    number_type x;
    number_type y;
};

// a helper function to print Points to STL output stream
template <class number_type>
inline std::ostream& operator<<(std::ostream &ostr, const Point<number_type> &pt) {
    ostr << "(" << pt.x << "," << pt.y << ")";
    return ostr;
}

// ==============================================================
// ==============================================================
// Node class

template<class number_type, class label_type>
class Node {
public:
    Node() {
        for (size_t i=0; i<4; i++) children[i] = NULL;
        parent = NULL;
    }
    Node(const Point<number_type>& p, const label_type& l) : pt(p), label(l) {
        for (size_t i=0; i<4; i++) children[i] = NULL;
        parent = NULL;
    }
    // representation
    Point<number_type> pt;
    label_type label;
    Node<number_type,label_type>* children[4];
    Node<number_type,label_type>* parent;
};

// ==============================================================
// ==============================================================
// Depth-First Iterator Class

template <class number_type, class label_type>
class DepthIterator {
public:
    DepthIterator() : ptr_(NULL) {}
    DepthIterator(Node<number_type,label_type>* p) : ptr_(p) {}
    DepthIterator(const DepthIterator& old) : ptr_(old.ptr_) {}
    ~DepthIterator() {}
    DepthIterator& operator=(const DepthIterator& old) { ptr_ = old.ptr_;  return *this; }
    // operator* gives constant access to the value at the pointer
    const Point<number_type>& operator*() const { return ptr_->pt; }
    // accessors
    const label_type& getLabel() const { return ptr_->label; }
    int getDepth() const {
        if (!ptr_) return -1;
        int depth = 0;
        for (Node<number_type,label_type>* ptr=ptr_; ptr->parent; depth++)
            ptr=ptr->parent;
        return depth;
    }
    // comparions operators are straightforward
    bool operator== (const DepthIterator& rgt) { return ptr_ == rgt.ptr_; }
    bool operator!= (const DepthIterator& rgt) { return ptr_ != rgt.ptr_; }
    // increment & decrement
    // --------------------------------------------------------------
    // pre-increment
    DepthIterator& operator++() {
        // If ptr_ has any children, next is its first child node
        for (size_t i=0; i<4; i++)
            if (ptr_->children[i]) {
                ptr_ = ptr_->children[i];
                return *this;
            }
        // Else it's a leaf node, next is its next sibling if any,
        // or its parent's next sibling, or its parent's parent's next sibling, ...
        while (ptr_->parent!=NULL && next_sibling(ptr_)==NULL)
            ptr_ = ptr_->parent;
        if (ptr_->parent==NULL) ptr_ = NULL;
        else ptr_ = next_sibling(ptr_);
        return *this;
    }

    // post-increment
    DepthIterator operator++(int) {
        DepthIterator temp(*this);
        operator++();  // or ++(*this);
        return temp;
    }
    
private:
    // representation
    Node<number_type,label_type>* ptr_;
    
    // Helper functions
    Node<number_type,label_type>* next_sibling(Node<number_type,label_type>* ptr) {
        Node<number_type,label_type>* parent = ptr_->parent;
        assert(parent != NULL);
        size_t this_index;
        // which child is ptr?
        for (this_index=0; this_index<4; this_index++)
            if (parent->children[this_index] == ptr)
                break;
        assert(this_index < 4);
        if (this_index == 3) return NULL;
        // find its next sibling
        for (size_t i=this_index+1; i<4; i++)
            if (parent->children[i])
                return parent->children[i];
        return NULL;
    }
};

// ==============================================================
// ==============================================================
// Breadth-First Iterator Class

template <class number_type,class label_type>
class BreadthIterator {
public:
    BreadthIterator() : ptr_(NULL) {}
    BreadthIterator(Node<number_type,label_type>* p) : ptr_(p) {}
    BreadthIterator(const BreadthIterator& old) : ptr_(old.ptr_) {}
    ~BreadthIterator() {}
    BreadthIterator& operator=(const BreadthIterator& old) {
        ptr_ = old.ptr_;  return *this;
    }
    // operator* gives constant access to the value at the pointer
    const Point<number_type>& operator*() const { return ptr_->pt; }
    // accessors
    const label_type& getLabel() const { return ptr_->label; }
    int getDepth() const {
        return DepthIterator<number_type,label_type>(ptr_).getDepth(); }
    // comparions operators are straightforward
    bool operator== (const BreadthIterator& rgt) { return ptr_ == rgt.ptr_; }
    bool operator!= (const BreadthIterator& rgt) { return ptr_ != rgt.ptr_; }
    // increment & decrement
    // --------------------------------------------------------------
    // pre-increment
    BreadthIterator& operator++() {
        // If next sibling exists, returns it
        Node<number_type,label_type>* sibling;
        if (ptr_->parent && (sibling=next_sibling(ptr_)))
            ptr_=sibling;
        // Otherwise, find its next node at the same level if any,
        //   or first node at the next level if any
        else
            ptr_ = next_node(ptr_);     // might be NULL
        return *this;
    }
    
    // post-increment
    BreadthIterator operator++(int) {
        BreadthIterator temp(*this);
        operator++();  // or ++(*this);
        return temp;
    }
    
private:
    // representation
    Node<number_type,label_type>* ptr_;

    // Private helper functions
    Node<number_type,label_type>* next_sibling(Node<number_type,label_type>* ptr) {
        Node<number_type,label_type>* parent = ptr_->parent;
        assert(parent != NULL);
        size_t this_index;
        // which child is ptr?
        for (this_index=0; this_index<4; this_index++)
            if (parent->children[this_index] == ptr)
                break;
        assert(this_index < 4);
        if (this_index == 3) return NULL;
        // find its next sibling
        for (size_t i=this_index+1; i<4; i++)
            if (parent->children[i])
                return parent->children[i];
        return NULL;
    }
    
    Node<number_type,label_type>* next_node(Node<number_type,label_type>* ptr) {
        // find the root, and levels
        size_t level = 1;
        Node<number_type,label_type>* root;
        for (root=ptr; root->parent; root=root->parent)
            level++;
        // use BFS algorithm and create a list of nodes at ptr's level
        std::list<Node<number_type,label_type>*> this_level, next_level;
        this_level.push_back(root);
        for (size_t l=1; l<level; l++) {
            next_level.clear();
            while (!this_level.empty()) {
                Node<number_type,label_type>* tmp = this_level.front();
                this_level.pop_front();
                for (size_t i=0; i<4; i++)
                    if (tmp->children[i]) next_level.push_back(tmp->children[i]);
            }
            this_level = next_level;
        };
        // return ptr's next sibling in the list, if any
        typename std::list< Node<number_type,label_type>* >::iterator p;
        for (p=this_level.begin(); p!=this_level.end(); p++)
            if (*p == ptr)
                break;
        assert(p != this_level.end());
        if (++p != this_level.end())
            return *p;
        // otherwise, return the first node at the next level
        for (p=this_level.begin(); p!=this_level.end(); p++)
            for (size_t i=0; i<4; i++)
                if ((*p)->children[i])
                    return (*p)->children[i];
        return NULL;
    }
};

// ==============================================================
// ==============================================================
// QuadTree Class

template <class number_type, class label_type>
class QuadTree {
public:
    QuadTree() : root_(NULL), size_(0) {}
    QuadTree(const QuadTree<number_type,label_type>& old) : size_(old.size_) {
        root_ = this->copy_tree(old.root_); }
    ~QuadTree() { this->destroy_tree(root_);  root_ = NULL; }
    QuadTree& operator=(const QuadTree<number_type,label_type>& old) {
        if (&old != this) {
            this->destroy_tree(root_);
            root_ = this->copy_tree(old.root_);
            size_ = old.size_;
        }
        return *this;
    }
    
    typedef DepthIterator<number_type,label_type> iterator;
    typedef BreadthIterator<number_type,label_type> bf_iterator;

    size_t size() const { return size_; }
    
    // an empty tree has height == -1, while a tree with 1 node has height == 0
    int height() const { return height(root_); }
    
    bool operator==(const QuadTree<number_type,label_type>& old) const { return (old.root_ == this->root_); }
    
    // FIND, INSERT & ERASE
    iterator find(const number_type& x, const number_type& y) { return find(x, y, root_); }
    
    std::pair< iterator, bool > insert(const Point<number_type>& p, const label_type& l) {
        return insert(p, l, root_);
    }
    
    // int erase(T const& key_value) { return erase(key_value, root_); }
    
    // OUTPUT & PRINTING
    // ==============================================================
    // PROVIDED CODE : QUAD TREE MEMBER FUNCTIONS FOR PRINTING
    // ==============================================================
    
    // NOTE: this function only works for quad trees with non negative
    // integer coordinates and char labels
    
    // NOTE2: this function assumes that no two points have the same x
    // coordinate or the same y coordinate.
    
    // plot driver function
    // takes in the maximum x and y coordinates for these data points
    // the optional argument draw_lines defaults to true
    void plot(int max_x, int max_y, bool draw_lines=true) const {
        // allocate blank space for the center of the board
        std::vector<std::string> board(max_y+1,std::string(max_x+1,' '));
        // edit the board to add the point labels and draw vertical and
        // horizontal subdivisions
        plot(root_,board,0,max_x,0,max_y,draw_lines);
        // print the top border of the plot
        std::cout << "+" << std::string(max_x+1,'-') << "+" << std::endl;
        for (int i = 0; i <= max_y; i++) {
            // print each row of the board between vertical border bars
            std::cout << "|" << board[i] << "|" << std::endl;
        }
        // print the top border of the plot
        std::cout << "+" << std::string(max_x+1,'-') << "+" << std::endl;
    }
    
    // actual recursive function for plotting
    void plot(Node<number_type,label_type> *p, std::vector<std::string> &board,
              int x_min, int x_max, int y_min, int y_max, bool draw_lines) const {
        // base case, draw nothing if this node is NULL
        if (p == NULL) return;
        // check that the dimensions range of this node make sense
        assert (x_min >= 0 && x_min <= x_max);
        assert (y_min >= 0 && y_min <= y_max);
        assert (board.size() >= y_max);
        assert (board[0].size() >= x_max);
        // verify that the point stored at this node fits on the board
        assert (p->pt.y >= 0 && p->pt.y < board.size());
        assert (p->pt.x >= 0 && p->pt.x < board[0].size());
        // draw the vertical and horizontal bars extending across the
        // range of this node
        if (draw_lines) {
            for (int x = x_min; x <= x_max; x++) {
                board[p->pt.y][x] = '-';
            }
            for (int y = y_min; y <= y_max; y++) {
                board[y][p->pt.x] = '|';
            }
        }
        // draw this label
        board[p->pt.y][p->pt.x] = p->label;
        // recurse on the 4 children
        plot(p->children[0],board,x_min ,p->pt.x-1,y_min ,p->pt.y-1,draw_lines);
        plot(p->children[1],board,p->pt.x+1,x_max ,y_min ,p->pt.y-1,draw_lines);
        plot(p->children[2],board,x_min ,p->pt.x-1,p->pt.y+1,y_max ,draw_lines);
        plot(p->children[3],board,p->pt.x+1,x_max ,p->pt.y+1,y_max ,draw_lines);
    }
    
    
    // ==============================================================
    
    // prints all of the tree data with a pre-order (node first, then
    // children) traversal of the tree structure
    
    // driver function
    void print_sideways() const { print_sideways(root_,""); }
    
    // actual recursive function
    void print_sideways(Node<number_type,label_type>* p, const std::string &indent) const {
        // base case
        if (p == NULL) return;
        // print out this node
        std::cout << indent << p->label << " (" << p->pt.x << "," << p->pt.y << ")" << std::endl;
        // recurse on each of the children trees
        // increasing the indentation
        print_sideways(p->children[0],indent+"  ");
        print_sideways(p->children[1],indent+"  ");
        print_sideways(p->children[2],indent+"  ");
        print_sideways(p->children[3],indent+"  ");
    }
    
    // ==============================================================
    
    // ITERATORS
    iterator begin() const {
        if (!root_) return iterator(NULL);
        // root is the beginning in pre-order
        return iterator(root_);
    }
    
    iterator end() const { return iterator(NULL); }
    
    bf_iterator bf_begin() const {
        if (!root_) return bf_iterator(NULL);
        // root is the beginning in BFS
        return bf_iterator(root_);
    }
    
    bf_iterator bf_end() const { return bf_iterator(NULL); }
    
private:
    // REPRESENTATION
    Node<number_type,label_type>* root_;
    size_t size_;
    
    // PRIVATE HELPER FUNCTIONS
    Node<number_type,label_type>* copy_tree(Node<number_type,label_type>* old_root) {
        if (!old_root) return NULL;
        Node<number_type,label_type>* new_root =
            new Node<number_type,label_type>(old_root->pt, old_root->label);
        for (size_t i=0; i<4; i++) {
            new_root->children[i] = copy_tree(old_root->children[i]);
            if (new_root->children[i])
                new_root->children[i]->parent = new_root;
        }
        return new_root;
    }
    
    void destroy_tree(Node<number_type,label_type>* p) {
        if (!p) return;
        for (size_t i=0; i<4; i++) {
            destroy_tree(p->children[i]);
        }
        delete p;
    }
    
    // an empty tree has height == -1, while a tree with 1 node has height == 0
    int height(Node<number_type,label_type>* p) const {
        if (!p) return -1;
        int max_height_children = -1;
        for (size_t i=0; i<4; i++)
            if (height(p->children[i]) > max_height_children)
                max_height_children = height(p->children[i]);
        return 1 + max_height_children;
    }
    
    iterator find(const number_type& x, const number_type& y, Node<number_type,label_type>* p) {
        if (!p) return iterator(NULL);
        if (x==p->pt.x && y==p->pt.y) return iterator(p);
        for (size_t i=0; i<4; i++) {
            iterator found = find(x, y, p->children[i]);
            if (found != iterator(NULL)) return found;
        }
        return iterator(NULL);
    }

    // locates the rectangular region and decides which of 4 children (0..3) should have p.
    // If p already exists, returns 4.
    size_t locate_child(const Point<number_type>& p, Node<number_type,label_type>* root) {
        const Point<number_type>& p0 = root->pt;
        if (p.x==p0.x && p.y==p0.y) return 4;
        else if (p.x<p0.x && p.y<p0.y) return 0;
        else if (p.x>p0.x && p.y<p0.y) return 1;
        else if (p.x>p0.x && p.y>p0.y) return 3;
        else if (p.x<p0.x && p.y>p0.y) return 2;
        else { assert(false); return 4; }
    }
    
    // returns true if p did not exist. Otherwise, returns false.
    /* -- iterative version of insert
    std::pair< iterator, bool >
    insert(const Point<number_type>& p, const label_type& l, Node<number_type,label_type>*& root) {
        // if root is empty, insert the node
        if (!root) {
            root = new Node<number_type,label_type>(p, l);
            this->size_++;
            return std::make_pair(iterator(root), true);
        }
        // Using p's position, keep walking down the tree to the leaf level
        Node<number_type,label_type>* parent=NULL;
        Node<number_type,label_type>* ptr = root;
        size_t i = 0;
        while (ptr) {
            i = locate_child(p, ptr); assert(i>=0 && i<=4);
            if (i==4) { // p already exists in the tree
                return std::make_pair(iterator(ptr), false);
            }
            parent = ptr;
            ptr = ptr->children[i];
        }
        // and insert the node, and set the parent pointer
        ptr = new Node<number_type,label_type>(p, l);
        ptr->parent = parent;
        parent->children[i] = ptr;
        this->size_++;
        return std::make_pair(iterator(ptr), true);
    }
     */
    
    // recursive version of insert
    std::pair< iterator, bool >
    insert(const Point<number_type>& p, const label_type& l, Node<number_type,label_type>*& root) {
        // if root is empty, insert the node
        if (!root) {
            root = new Node<number_type,label_type>(p, l);
            this->size_++;
            return std::make_pair(iterator(root), true);
        }
        return insert1(p, l, root);
    }

    // insert it under parent, which is not NULL
    std::pair< iterator, bool >
    insert1(const Point<number_type>& p,const label_type& l,Node<number_type,label_type>* parent) {
        size_t i = locate_child(p, parent); assert(i>=0 && i<=4);
        if (i==4) { // p already exists in the tree
            return std::make_pair(iterator(parent), false);
        }
        // If we've reached leaf, insert the node to its children[i], and set the parent pointer
        if (parent->children[i] == NULL) {
            this->size_++;
            Node<number_type,label_type>* ptr = new Node<number_type,label_type>(p, l);
            parent->children[i] = ptr;
            ptr->parent = parent;
            return std::make_pair(iterator(ptr), true);
        }
        // Otherwise, try it with children[i]
        return insert1(p, l, parent->children[i]);
    }
    
};

#endif
