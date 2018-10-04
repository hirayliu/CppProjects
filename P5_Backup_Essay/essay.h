#ifndef essay_h_
#define essay_h_
// A simplified implementation of the STL list container class,
// including the iterator, but not the const_iterators.  Three
// separate classes are defined: a Node class, an iterator class, and
// the actual list class.  The underlying list is doubly-linked, but
// there is no dummy head node and the list is not circular.
#include <cassert>

// -----------------------------------------------------------------
// NODE CLASS
class Node {
public:
    Node() : next_(NULL), prev_(NULL), backup_next_(NULL), backup_prev_(NULL), added_(false), removed_(false) {}
    Node(const std::string& v) : value_(v), next_(NULL), prev_(NULL),
        backup_next_(NULL), backup_prev_(NULL), added_(false), removed_(false) {}

    void debug() const;
    
    // REPRESENTATION
    std::string value_;
    Node* next_;
    Node* prev_;
    Node* backup_next_;
    Node* backup_prev_;
    bool added_;
    bool removed_;
};

// A "forward declaration" of this class is needed
class Essay;

// -----------------------------------------------------------------
// LIST ITERATOR
class list_iterator {
public:
    // default constructor, copy constructor, assignment operator, & destructor
    list_iterator(Node* p=NULL, bool backup=false) : ptr_(p), backup_(backup) {}
    // NOTE: the implicit compiler definitions of the copy constructor,
    // assignment operator, and destructor are correct for this class
    
    // dereferencing operator gives access to the value at the pointer
    std::string& operator*()  { return ptr_->value_;  }
    
    // increment & decrement operators
    list_iterator& operator++() { // pre-increment, e.g., ++iter
        ptr_ = backup_ ? ptr_->backup_next_ : ptr_->next_;
        return *this;
    }
    list_iterator operator++(int) { // post-increment, e.g., iter++
        list_iterator temp(*this);
        ptr_ = backup_ ? ptr_->backup_next_ : ptr_->next_;
        return temp;
    }
    list_iterator& operator--() { // pre-decrement, e.g., --iter
        ptr_ = backup_ ? ptr_->backup_prev_ : ptr_->prev_;
        return *this;
    }
    list_iterator operator--(int) { // post-decrement, e.g., iter--
        list_iterator temp(*this);
        ptr_ = backup_ ? ptr_->backup_prev_ : ptr_->prev_;
        return temp;
    }
    // the Essay class needs access to the private ptr_ member variable
    friend class Essay;
    
    // Comparions operators are straightforward
    bool operator==(const list_iterator& r) const {
        return ptr_ == r.ptr_; }
    bool operator!=(const list_iterator& r) const {
        return ptr_ != r.ptr_; }

private:
    // REPRESENTATION
    Node* ptr_;    // ptr to node in the list
    bool backup_;    // is this a backup iterator?
};

// -----------------------------------------------------------------
// LIST CLASS DECLARATION
// Note that it explicitly maintains the size of the list.
class Essay {
public:
    // default constructor, copy constructor, assignment operator, & destructor
    Essay() : head_(NULL), tail_(NULL), size_(0), backup_head_(NULL), backup_tail_(NULL) {}
    Essay(const Essay& old) { copy_list(old); }
    Essay& operator= (const Essay& old);
    ~Essay() { destroy_list(); }
    
    typedef list_iterator iterator;
    
    // simple accessors & modifiers
    unsigned int size() const { return size_; }
    bool empty() const { return head_ == NULL; }
    void clear() { destroy_list(); }
    unsigned int num_added() const;
    unsigned int num_removed() const;

    // read/write access to contents
    const std::string& front() const { return head_->value_;  }
    std::string& front() { return head_->value_; }
    const std::string& back() const { return tail_->value_; }
    std::string& back() { return tail_->value_; }
    
    // modify the linked list structure
    void push_front(const std::string& v);
    void pop_front();
    void push_back(const std::string& v);
    void pop_back();
    
    iterator erase(iterator itr);
    iterator insert(iterator itr, const std::string& v);
    iterator begin() { return iterator(head_); }
    iterator end() { return iterator(NULL); }
    iterator backup_begin() { return iterator(backup_head_, true); }
    iterator backup_end() { return iterator(NULL); }

    void debug() const;
    
    void backup();

    void revert();
    
    bool splice(const iterator &_begin, const iterator &_end,
                const iterator &_destination);
    
private:
    // private helper functions
    void copy_list(const Essay& old);
    void destroy_list();
    
    //REPRESENTATION
    Node* head_;
    Node* tail_;
    Node* backup_head_;
    Node* backup_tail_;
    unsigned int size_;
};

#endif
