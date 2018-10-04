#ifndef priority_queue_h_
#define priority_queue_h_

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>

// A custom templated priority queue.  The priority queue is a min
// heap (smallest values on top).  The heap is stored in a vector.
// The integer location for an element within the vector can be
// quickly looked up using a map.

// find out the current index of the element within the heap.

// ASSIGNMENT: The class implementation is incomplete.  Finish the
//   implementation of this class, and add any functions you need.


// =========================================================================

template <class T>
class PriorityQueue {
public:
    // CONSTRUCTOR
    PriorityQueue() {}
    
    // =========================
    // ACCESSORS
    unsigned int size() const { return m_heap.size(); }
    T top() const;
    
    // =========================
    // MODIFIERS
    // add an element
    void push(T element);
    // remove the top element, or an element elsewhere in the structure
    void pop();
    void remove(T element);
    // update the priority of an element somewhere in the structure
    void update_position(T element);
    
    // =========================
    // for debugging
    void print_heap(std::ostream & ostr) const;
    bool check_heap() const;
    
private:
    // =========================
    // PRIVATE ACCESSOR FUNCTIONS
    int last_non_leaf() const { return ((int)size()-1) / 2; }
    int get_parent(int i) const { assert (i > 0 && i < (int)size()); return (i-1) / 2; }
    bool has_left_child(int i) const { return (2*i)+1 < (int)size(); }
    bool has_right_child(int i) const { return (2*i)+2 < (int)size(); }
    int get_left_child(int i) const { assert (i >= 0 && has_left_child(i)); return 2*i + 1; }
    int get_right_child(int i) const { assert (i >= 0 && has_right_child(i)); return 2*i + 2; }
    // PRIVATE MODIFIER FUNCTIONS
    void swap(int x, int y);
    void percolate_up(int i);
    void percolate_down(int i);
    
    // =========================
    // REPRESENTATION
    // the heap is stored in a vector representation
    // (the binary tree structure "unrolled" one row at a time)
    std::vector<T> m_heap;
    // the map stores a correpondence between elements & indices in the heap
    std::map<T,int> locations;
    
};

// =========================================================================


// access the top element (minimum value) in the heap
template <class T>
T PriorityQueue<T>::top() const  {
    assert(!m_heap.empty());
    return m_heap[0];
}


// add a new element to the heap
template <class T>
void PriorityQueue<T>::push(T element) {
    // first, verify that the element isn't already in the heap
    typename std::map<T,int>::iterator itr = locations.find(element);
    if (itr != locations.end()) {
        std::cout << "ERROR!  priority queue aleady contains " << element << std::endl;
        assert (element == itr->first);
    }
    assert (itr == locations.end());
    // add the element at the edge of heap vector and percolate up
    m_heap.push_back(element);
    locations[element] = m_heap.size()-1;
    this->percolate_up(int(m_heap.size()-1));
}


// remove the top element (minimum value) from the heap
template <class T>
void PriorityQueue<T>::pop() {
    assert(!m_heap.empty());
    int success = locations.erase(m_heap[0]);
    assert (success == 1);
    // place the last element temporarily at the top of the heap, but
    // push it down to a proper position using percolate down
    m_heap[0] = m_heap.back();
    m_heap.pop_back();
    locations[m_heap[0]] = 0;
    this->percolate_down(0);
}


// remove a specific element that could be anywhere in the heap
template <class T>
void PriorityQueue<T>::remove(T element) {
    
    //
    // ASSIGNMENT: Implement this function
    //
    assert(!m_heap.empty());
    
    // locate element in the heap
    typename std::map<T,int>::iterator itr = locations.find(element);
    assert (itr != locations.end());
    int idx = itr->second;

    // remove element from locations
    int success = locations.erase(element);
    assert (success == 1);

    // If the element is the last one, pop_back and we are done.
    if (idx == (int)size()-1) {
        m_heap.pop_back();
        return;
    }
        
    // copy the last element to idx
    m_heap[idx] = m_heap.back();
    m_heap.pop_back();
    locations[m_heap[idx]] = idx;
    
    // the element at idx has changed, so need to percolate up and down
    this->percolate_up(idx);
    this->percolate_down(idx);
}


// this element may have had its value change, so we should adjust the
// position of the element within the heap (it might need to move up
// or down)
template <class T>
void PriorityQueue<T>::update_position(T element) {
    typename std::map<T,int>::iterator itr = locations.find(element);
    assert (itr != locations.end());
    this->percolate_up(itr->second);
    this->percolate_down(itr->second);
}

// =========================================================================
// =========================================================================

// for debugging, print all of the data in the heap
template <class T>
void PriorityQueue<T>::print_heap(std::ostream & ostr) const {
    for (int i=0; i<(int)m_heap.size(); ++i)
        // Why can we call getPriorityValue() on T? Because there is no compile-time check.
        // It's just a lexical substitution.
        ostr << "[" << std::setw(4) << i << "] : "
        << std::setw(6) << m_heap[i]->getPriorityValue()
        << " " << *m_heap[i] << std::endl;
}


// for debugging, walk over the heap and check all of the elements &
// their values
template <class T>
bool PriorityQueue<T>::check_heap() const {
    if (m_heap.size() <= 1) return true;
    bool error_found = false;
    
    // first check to see if the element values are up-to-date
    for (int i=0; i<(int)m_heap.size(); ++i) {
        if (m_heap[i]->CheckValue()) {
            error_found = true;
        }
    }
    
    // next loop over all nodes that have at least one child
    for (int i=0; has_left_child(i); ++i) {
        assert (has_left_child(i));
        int left_child = 2*i+1;
        // verify that this is a min heap
        // (parent is smaller value than both children
        if (m_heap[i]->getPriorityValue() > m_heap[left_child]->getPriorityValue()) {
            error_found = true;
            std::cout << "Error: at heap location " << i
            << ", the value is greater than the value at the left child.\n";
            std::cout << "value@" << i<< "=" <<  m_heap[i]->getPriorityValue()
            << "value@" << left_child << "=" << m_heap[left_child]->getPriorityValue() << std::endl;
            
        }
        int right_child = 2*i+2;
        if (right_child < (int)m_heap.size() &&
            m_heap[i]->getPriorityValue() > m_heap[right_child]->getPriorityValue()) {
            error_found = true;
            std::cout << "Error: at heap location " << i
            << ", the value is greater than the value at the right child.\n";
            std::cout << "value@" << i<< "=" <<  m_heap[i]->getPriorityValue()
            << "value@" <<right_child << "=" << m_heap[right_child]->getPriorityValue() << std::endl;
        }
    }
    
    if (error_found) {
        print_heap(std::cout);
    }
    return !error_found;
}


// =========================================================================
// =========================================================================

template <class T>
void PriorityQueue<T>::swap(int x, int y) {
    T t = m_heap[x];
    m_heap[x] = m_heap[y];
    m_heap[y] = t;
    locations[m_heap[x]] = x;
    locations[m_heap[y]] = y;
}

// allow the element at this location to move up
template <class T>
void PriorityQueue<T>::percolate_up(int i) {
    
    //
    // ASSIGNMENT: Implement this function
    //
    // std::cout << "::percolate_up with " << i << std::endl;
    
    while (i>0) {
        // std::cout << "    get_parent with " << i << std::endl;
        int p = get_parent(i);
        if (m_heap[i]->getPriorityValue() < m_heap[p]->getPriorityValue()) {
            swap(i, p);  // value and other non-pointer member vars
            i = p;
        } else
            break;
    }
    /*
    while (p->parent)
        if (p->value < p->parent->value) {
            swap(p, parent);  // value and other non-pointer member vars
            p = p->parent;
        } else
            break;
     */
}

// allow the element at this location to move down
template <class T>
void PriorityQueue<T>::percolate_down(int i) {
    
    //
    // ASSIGNMENT: Implement this function
    //
    while (has_left_child(i)) {
        int left_child = get_left_child(i);
        int child;
        //  Choose the child to compare against
        if (has_right_child(i)) {
            int right_child = get_right_child(i);
            if (m_heap[right_child]->getPriorityValue() < m_heap[left_child]->getPriorityValue())
                child = right_child;
            else
                child = left_child;
        }
        else
            child = left_child;
        if (m_heap[child]->getPriorityValue() < m_heap[i]->getPriorityValue()) {
            swap(child, i); // value and other non-pointer member vars
            i = child;
        }
        else
            break;
    }
    /*
    while (p->left) {
        TreeNode<T>* child;
        //  Choose the child to compare against
        if (p->right && p->right->value < p->left->value)
            child = p->right;
        else
            child = p->left;
        if (child->value < p->value) {
            swap(child, p); // value and other non-pointer member vars
            p = child;
        }
        else break;
    }
     */
}

// =========================================================================
// =========================================================================
#endif
