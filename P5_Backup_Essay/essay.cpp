#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <iomanip>

#include "essay.h"

// -----------------------------------------------------------------
// LIST CLASS IMPLEMENTATION
Essay& Essay::operator= (const Essay& old) {
    // check for self-assignment
    if (&old != this) {
        destroy_list();
        copy_list(old);
    }
    return *this;
}

void Essay::push_front(const std::string& v) {
    Node *node = new Node(v);
    node->added_ = true;
    node->next_ = head_;
    if( head_ == NULL ) {
        tail_ = node;
    }
    else {
        head_->prev_ = node;
    }
    head_ = node;
    size_++;
}

void Essay::pop_front() {
    if(head_ == NULL){
        std::cout << "pop_front(): nothing to pop";
        exit(1);
    }
    Node *node = head_;
    head_ = node->next_;
    head_->prev_ = NULL;
    if( node->backup_prev_==NULL && node->backup_next_==NULL )
        // not in backup version; just delete it
        delete node;
    else    {
        node->removed_ = true; node->prev_ = node->next_ = NULL;
    }
    size_--;
}

void Essay::push_back(const std::string& v) {
    Node *node = new Node(v);
    node->added_ = true;
    if( tail_ == NULL ) {
        head_ = node;
    }
    else {
        node->prev_ = tail_;
        tail_->next_ = node;
    }
    tail_ = node;
    size_++;
}

void Essay::pop_back() {
    if(tail_ == NULL)   {
        std::cout << "pop_back(): nothing to pop";
        exit(1);
    }
    Node *node = tail_;
    tail_ = node->prev_;
    tail_->next_ = NULL;
    if( node->backup_prev_==NULL && node->backup_next_==NULL )
        // not in backup version; just delete it
        delete node;
    else {
        node->removed_ = true; node->prev_ = node->next_ = NULL;
    }
    size_--;
    if( size_ == 0 )
        head_ = NULL;
}

// do these lists look the same (length & contents)?
bool operator== (Essay& left, Essay& right) {
    if (left.size() != right.size()) return false;
    // walk over both of the current versions, looking for a mismatched value
    Essay::iterator left_itr = left.begin();
    Essay::iterator right_itr = right.begin();
    while (left_itr != left.end()) {
        if (*left_itr != *right_itr
            // || left_itr.ptr_->added_ != right_itr.ptr_->added_
            // || left_itr.ptr_->removed_ != right_itr.ptr_->removed_
            )
            return false;
        left_itr++; right_itr++;
    }
    // walk over the removed nodes of the backup versions, looking for a mismatched value
    left_itr = left.backup_begin();
    right_itr = right.backup_begin();
    while (left_itr != left.backup_end()) {
        if (*left_itr != *right_itr
            // || left_itr.ptr_->added_ != right_itr.ptr_->added_
            // || left_itr.ptr_->removed_ != right_itr.ptr_->removed_
            )
            return false;
        left_itr++; right_itr++;
    }
    return true;
}

bool operator!= (Essay& left, Essay& right){ return !(left==right); }

Essay::iterator Essay::erase(iterator itr) {
    if( itr.ptr_ == NULL )   {
        std::cout << "erase(): nothing to erase";
        exit(1);
    }
    // Should be able to combine the cases
    // Case: itr == head_ && itr == tail_
    if( itr.ptr_ == head_ && itr.ptr_ == tail_ ) {
        head_ = NULL;
        tail_ = NULL;
        if( itr.ptr_->backup_prev_==NULL && itr.ptr_->backup_next_==NULL )
            // not in backup version; just delete it
            delete itr.ptr_;
        else { // otherwise, soft delete
            itr.ptr_->removed_ = true;
            itr.ptr_->prev_ = itr.ptr_->next_ = NULL;
        }
        size_--;
        return NULL;
    }
    // Case: itr == head_ && itr != tail_
    if( itr.ptr_ == head_ && itr.ptr_ != tail_ ) {
        head_ = itr.ptr_->next_;
        itr.ptr_->next_->prev_ = NULL;
        if( itr.ptr_->backup_prev_==NULL && itr.ptr_->backup_next_==NULL )
            // not in backup version; just delete it
            delete itr.ptr_;
        else    {
            itr.ptr_->removed_ = true;
            itr.ptr_->prev_ = itr.ptr_->next_ = NULL;
        }
        size_--;
        return head_;
    }
    // Case: itr == tail_ && itr != head_
    if( itr.ptr_ == tail_ && itr.ptr_ != head_ ) {
        tail_ = itr.ptr_->prev_;
        tail_->next_ = NULL;
        if( itr.ptr_->backup_prev_==NULL && itr.ptr_->backup_next_==NULL )
            // not in backup version; just delete it
            delete itr.ptr_;
        else {
            itr.ptr_->removed_ = true;
            itr.ptr_->prev_ = itr.ptr_->next_ = NULL;
        }
        size_--;
        return NULL;
    }
    // Case: itr is in the middle
    Node* ptr = itr.ptr_->next_;
    itr.ptr_->prev_->next_ = ptr;
    ptr->prev_ = itr.ptr_->prev_;
    if( itr.ptr_->backup_prev_==NULL && itr.ptr_->backup_next_==NULL )
        // not in backup version; just delete it
        delete itr.ptr_;
    else {
        itr.ptr_->removed_ = true;
        itr.ptr_->prev_ = itr.ptr_->next_ = NULL;
    }
    size_--;
    return ptr;
}

Essay::iterator Essay::insert(iterator itr, const std::string& v) {
    Node *node = new Node(v);
    node->added_ = true;
    node->next_ = itr.ptr_;
    // Case 0: empty list
    if( head_ == NULL ) {
        head_ = node;
        tail_ = node;
    }
    // Case 1: itr == end()
    else if( itr.ptr_ == NULL ) {
        node->prev_ = tail_;
        tail_->next_ = node;
        tail_ = node;
    }
    // Case 2: itr is the 1st node
    else if( itr.ptr_ == head_ ) {
        itr.ptr_->prev_ = node;
        head_ = node;
    }
    // Case 3: itr is a node after an existing node
    else {
        node->prev_ = itr.ptr_->prev_;
        itr.ptr_->prev_->next_ = node;
        itr.ptr_->prev_ = node;
    }

    size_++;
    
    return node;
}

void Essay::copy_list(const Essay& old) {
    size_ = old.size_;
    backup_head_ = backup_tail_ = NULL;
    if( size_ == 0 ) {
        head_ = tail_ = NULL;
        return;
    }
    
    // copy the current version
    Node* optr = old.head_;
    Node* prev = new Node(*optr);    // 1st node
    prev->prev_ = NULL;
    head_ = prev;
    Node* ptr;
    for(optr=optr->next_; optr!=NULL; optr=optr->next_, prev=ptr) {
        ptr = new Node(*optr);
        ptr->prev_ = prev;
        prev->next_ = ptr;
    }
    // prev is the last node
    prev->next_ = NULL;
    tail_ = ptr;

    // copy the backup version
    /*
    optr = old.backup_head_;
    if( optr == NULL ) {
        backup_head_ = backup_tail_ = NULL;
        return;
    }
    if( optr->removed_ ){   // if 1st node is removed, we need to create a node in this
        prev = new Node(*optr);    // 1st node
        prev->prev_ = NULL;
        head_ = prev;
    }
    Node* ptr;
    for(optr=optr->next_; optr!=NULL; optr=optr->next_, prev=ptr) {
        ptr = new Node(*optr);
        ptr->prev_ = prev;
        prev->next_ = ptr;
    }
    // prev is the last node
    prev->next_ = NULL;
    tail_ = ptr;
*/
}

void Essay::destroy_list() {
    // first, delete removed nodes from the backup version
    for(Node* ptr = backup_head_; ptr != NULL; ) {
        Node* next = ptr->backup_next_;
        if(ptr->removed_)
            delete ptr;
        ptr = next;
    }

    // next, destroy the current version
    for(Node* ptr = head_; ptr != NULL; ) {
        Node* next = ptr->next_;
        delete ptr;
        ptr = next;
    }
}

unsigned int Essay::num_added() const {
    unsigned int num = 0;
    // added_ nodes are only in the current version
    for(Node* ptr = head_; ptr != NULL; ptr=ptr->next_) {
        if(ptr->added_)
            num++;
    }
    return num;
}

unsigned int Essay::num_removed() const {
    unsigned int num = 0;
    // removed_ nodes are only in the backup version
    for(Node* ptr = backup_head_; ptr != NULL; ptr=ptr->backup_next_) {
        if(ptr->removed_)
            num++;
    }
    return num;
}

/* save the current version as backup version */
void Essay::backup() {
    // go through the backup version and remove removed nodes
    for (Node *ptr = backup_head_; ptr != NULL; ) {
        if(ptr->removed_) {
            Node* next = ptr->backup_next_;
            delete ptr;
            ptr = next;
        }
        else
            ptr = ptr->backup_next_;
    }

    // save the current version as the backup, making added nodes permanent
    backup_head_ = head_;
    
    for(Node* ptr = head_; ptr != NULL; ptr=ptr->next_) {
        if(ptr->added_) {
            ptr->added_ = false;    // making added node permanent
        }
        ptr->backup_prev_ = ptr->prev_;
        ptr->backup_next_ = ptr->next_;
    }
    backup_tail_ = tail_;
}

/* revert the current version to the backup version */
void Essay::revert() {
    // go through the current version, and delete added nodes
    for(Node* ptr = head_; ptr != NULL; ) {
        if(ptr->added_) {
            Node* next = ptr->next_;
            delete ptr;
            ptr = next;
        }
        else
            ptr = ptr->next_;
    }
    
    // revert the current version to the backup version, unremove removed nodes
    head_ = backup_head_;
    unsigned int num = 0;
    for(Node* ptr = backup_head_; ptr != NULL; ptr=ptr->backup_next_, num++) {
        if(ptr->removed_)
            ptr->removed_ = false;
        ptr->next_ = ptr->backup_next_;
        ptr->prev_ = ptr->backup_prev_;
    }
    tail_ = backup_tail_;
    size_ = num;
}

// No nodes are added or removed by splice. Instead links between existing nodes are changed.
// The splice should fail if either splice_begin does not precede
// splice_end or destination is contained within the range of
// slice_begin and splice end
bool Essay::splice(const Essay::iterator &_begin, const Essay::iterator &_end,
                   const Essay::iterator &_dest) {
    // check to see if _begin precedes _end
    if( _begin == _end ) {
        std::cerr << "Splice ERROR: _begin == _end\n";
        return false;
    }
    Essay::iterator i;
    for( i = _begin; i != end(); i++ )
        if(i == _end)   // _begin does precede _end
            break;
    if( i == end() && _end != end() ) {
        std::cerr << "Splice ERROR: _begin > _end\n";
        return false;
    }
    // check to see if _dest falls between _begin and _end
    for( i = _begin; i != _end; i++ )
        if(i == _dest) {
            std::cerr << "Splice ERROR: _dest falls between _begin and _end\n";
            return false;
        }
    
    // now splice
    // Step 1: finds the last node
    Node* first = _begin.ptr_;
    Node* last;
    if( _end.ptr_ != NULL ) {
        last = _end.ptr_->prev_;
    } else {
        Node* i;
        for( i=first; i->next_!=_end.ptr_; i=i->next_ )
            ;
        last = i;
    }
    // Step 2: cut the section
    if( first == head_ ) {   // the 1st node
        assert( last != tail_ );
        head_ = _end.ptr_;
        _end.ptr_->prev_ = NULL;
    }
    else if( last == tail_ ) {
        assert( first != head_ );
        tail_ = first->prev_;
        first->prev_->next_ = NULL;
    }
    else {
        first->prev_->next_ = _end.ptr_;
        _end.ptr_->prev_ = first->prev_;
    }
    // Step 3: paste the section to before _dest
    if( _dest == NULL ) { // _dest is end()
        Node* before = tail_;
        tail_ = last;
        last->next_ = NULL;
        first->prev_ = before;
        before->next_ = first;
    } else {
        Node* before = _dest.ptr_->prev_;
        _dest.ptr_->prev_ = last;
        last->next_ = _dest.ptr_;
        if( before == NULL )    {
            assert( head_ == _dest.ptr_ );
            head_ = first;
            first->prev_ = NULL;
        } else {
            first->prev_ = before;
            before->next_ = first;
        }
    }
    
    return true;
}

