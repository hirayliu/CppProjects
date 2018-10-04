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
    Node() : children[0](NULL), children[2](NULL), children[2](NULL), children[3](NULL), parent(NULL) {}
    Node(const Point<number_type>& p, const label_type& l) : pt(p), label(l),
        children[0](NULL), children[2](NULL), children[2](NULL), children[3](NULL), parent(NULL) {}
    // representation
    Point<number_type> pt;
    label_type label;
    Node<number_type, label_type>* children[4];
    Node<number_type, label_type>* parent;
};

// ==============================================================
// ==============================================================
// Node Depth-First Iterator Class

template <class number_type, class label_type>
class DepthIterator {
public:
    DepthIterator() : ptr_(NULL) {}
    DepthIterator(Node<number_type, label_type>* p) : ptr_(p) {}
    DepthIterator(const DepthIterator& old) : ptr_(old.ptr_) {}
    ~DepthIterator() {}
    DepthIterator& operator=(const DepthIterator& old) { ptr_ = old.ptr_;  return *this; }
    // operator* gives constant access to the value at the pointer
    const Point<number_type>& operator*() const { return ptr_->pt; }
    // comparions operators are straightforward
    bool operator== (const DepthIterator& rgt) { return ptr_ == rgt.ptr_; }
    bool operator!= (const DepthIterator& rgt) { return ptr_ != rgt.ptr_; }
    // increment & decrement
    
private:
    // representation
    Node<number_type, label_type>* ptr_;
};

// ==============================================================
// ==============================================================
// QuadTree Class

template <class number_type, class label_type>
class QuadTree {
public:
    QuadTree() : root_(NULL), size_(0) {}
    QuadTree(const QuadTree<number_type, label_type>& old) : size_(old.size_) {
        root_ = this->copy_tree(old.root_); }
    ~QuadTree() { this->destroy_tree(root_);  root_ = NULL; }
    QuadTree& operator=(const QuadTree<number_type, label_type>& old) {
        if (&old != this) {
            this->destroy_tree(root_);
            root_ = this->copy_tree(old.root_);
            size_ = old.size_;
        }
        return *this;
    }
    
    typedef DepthIterator<number_type, label_type> iterator;
    
    int size() const { return size_; }
    bool operator==(const QuadTree<number_type, label_type>& old) const { return (old.root_ == this->root_); }
    
    // FIND, INSERT & ERASE
    iterator find(const number_type& x, const number_type& y) { return find(x, y, root_); }
    // std::pair< iterator, bool > insert(T const& key_value) { return insert(key_value, root_); }
    // int erase(T const& key_value) { return erase(key_value, root_); }
    
    // OUTPUT & PRINTING
    
    // ITERATORS
    iterator begin() const {
        if (!root_) return iterator(NULL);
        Node<number_type, label_type>* p = root_;
        //while (p->left) p = p->left;
        return iterator(p);
    }
    iterator end() const { return iterator(NULL); }
    
private:
    // REPRESENTATION
    Node<number_type, label_type>* root_;
    size_type size_;
    
    // PRIVATE HELPER FUNCTIONS
    Node<number_type, label_type>*  copy_tree(Node<number_type, label_type>* old_root) {
        return NULL;
/*
        if (!old_root)
            return NULL;
        Node<number_type, label_type>* new_root = new Node<number_type, label_type>(old_root->value);
        new_root->left = copy_tree(old_root->left);
        new_root->right = copy_tree(old_root->right);
        return new_root;
 */
    }
    
    void destroy_tree(Node<number_type, label_type>* p) { /* Implemented in Lecture 18 */  }
    
    iterator find(const number_type& x, const number_type& y, Node<number_type, label_type>* p) {
        return NULL;
        /*
        if (!p) return iterator(NULL);
        if (p->value > key_value)
            return find(key_value, p->left);
        else if (p->value < key_value)
            return find(key_value, p->right);
        else
            return iterator(p);
         */
    }

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
// ==============================================================

};

#endif
