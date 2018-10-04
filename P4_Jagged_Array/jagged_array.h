//
//  jagged_array.h
//  
//
//  Created by rliu on 9/30/17.
//

#ifndef jagged_array_h
#define jagged_array_h

// TYPEDEFS
typedef unsigned int size_type;

template <class T> class JaggedArray {
public:
    // CONSTRUCTORS, ASSIGNMNENT OPERATOR, & DESTRUCTOR
    // JaggedArray() { this->create(); }
    JaggedArray(size_type n) { this->create(n); }
    JaggedArray(const JaggedArray& ja) { copy(ja); }
    JaggedArray& operator=(const JaggedArray& ja) {
        if (this != &ja) { clear_all(); copy(ja); }
        return *this;
    }
    ~JaggedArray() { clear_all(); }

    // MEMBER FUNCTIONS AND OTHER OPERATORS
    // accessors
    size_type numElements() const { return num_elements; }
    size_type numBins() const { return num_bins; }
    size_type numElementsInBin(size_type bin) const;
    const T& getElement(size_type bin, size_type i) const;
    bool isPacked() const { return counts == NULL; }
    void print() const;         // print JaggedArray
    // modifiers - only when it's unpacked
    void addElement(size_type bin, const T& e);
    void removeElement(size_type bin, size_type i);
    // others
    void clear();   // clear this to the state of JaggedArray(num_bins) is called
    void pack();
    void unpack();

private:
    // PRIVATE MEMBER FUNCTIONS
    void create(size_type n);
    void clear_all();   // clears everything, releasing all arrays
    void copy(const JaggedArray<T>& ja);

    // REPRESENTATION
    size_type num_elements;     // total number of elements stored in all the bins
    size_type num_bins;         // number of bins
    size_type* counts;         // for unpacked - array of integers
                                // containing the counts of how many elements are in each bin
    T** unpacked_values;       // for unpacked - array of pointers to arrays that hold these elements
    size_type* offsets;        // for packed - array of offsets for the start of each bin
    T* packed_values;          // for packed - a single large array storing all elements, grouped by bin
};

template<class T>
void JaggedArray<T>::create(size_type n) {
    num_elements = 0;
    num_bins = n;
    counts = new size_type[n];
    for(size_type b=0; b<num_bins; b++)
        counts[b] = 0;
    unpacked_values = new T*[n];
    for(size_type b=0; b<num_bins; b++)
        unpacked_values[b] = NULL;
    offsets = NULL;
    packed_values = NULL;
}

template<class T>
void JaggedArray<T>::copy(const JaggedArray<T>& ja) {
    num_elements = ja.num_elements;
    num_bins = ja.num_bins;
    if(ja.counts == NULL) {  // ja is packed
        counts = NULL;
        unpacked_values = NULL;
        offsets = new size_type[num_bins];
        for(size_type b=0; b<num_bins; b++)
            offsets[b] = ja.offsets[b];
        packed_values = new T[num_elements];
        for(size_type e=0; e<num_elements; e++)
            packed_values[e] = ja.packed_values[e];
    }
    else {  // ja is unpacked
        offsets = NULL;
        packed_values = NULL;
        counts = new size_type[num_bins];
        unpacked_values = new T*[num_bins];
        for(size_type b=0; b<num_bins; b++){
            counts[b] = ja.counts[b];
            if(counts[b] > 0){
                unpacked_values[b] = new T[counts[b]];
                for(size_type e=0; e<counts[b]; e++)
                    unpacked_values[b][e] = ja.unpacked_values[b][e];
            }
        }
    }
}

template<class T>
void JaggedArray<T>::clear_all() {
    if( counts == NULL ) {   // packed mode
        delete [] offsets; offsets = NULL;
        delete [] packed_values; packed_values = NULL;
    }
    else {  // unpacked mode
        for(size_type b=0; b<num_bins; b++)
            if(counts[b] > 0) {
                delete [] unpacked_values[b];
            }
        delete [] unpacked_values; unpacked_values = NULL;
        delete [] counts; counts = NULL;
    }
    num_elements = num_bins = 0;
}

template<class T>
void JaggedArray<T>::clear() {
    if(counts == NULL) {    // packed mode
        std::cerr << "Clear cannot be done to a packed JaggedArray; unpack it first." << std::endl;
        exit(1);
    }
    num_elements = 0;
    // num_bins = 0;
    for(size_type b=0; b<num_bins; b++)
        if(counts[b] > 0) {
            delete [] unpacked_values[b];
            counts[b] = 0;
        }
    // leave the 2 arrays there, returning to the state after JA(num_bins) is called
}

template<class T>
void JaggedArray<T>::pack() {
    if(counts == NULL) {    // packed mode
        std::cerr << "JaggedArray is already packed; cannot pack it again." << std::endl;
        exit(1);
    }
    // create offsets[]
    offsets = new size_type[num_bins];
    offsets[0] = 0;     // always start with 0
    for(size_type b=1, offset=0; b<num_bins; b++) {
        offset += counts[b-1];
        offsets[b] = offset;
    }
    // create packed_values[]
    packed_values = new T[num_elements];
    for(size_type b=0, offset=0; b<num_bins; b++)
        for(size_type e=0; e<counts[b]; e++)
            packed_values[offset++] = unpacked_values[b][e];
    // destroy packed arrays
    for(size_type b=0; b<num_bins; b++)
        if(counts[b] != 0)
            delete [] unpacked_values[b];
    delete [] unpacked_values; unpacked_values = NULL;
    delete [] counts; counts = NULL;
}

template<class T>
void JaggedArray<T>::unpack() {
    if(offsets == NULL) {    // unpacked mode
        std::cerr << "JaggedArray is already unpacked; cannot unpack it again." << std::endl;
        exit(1);
    }
    // std::cout << "DEBUG: unpack a JaggedArray bins=" << num_bins << " and nums=" << num_elements << std::endl;
    // create counts[]
    counts = new size_type[num_bins];
    for(size_type b=0; b<num_bins-1; b++) {
        counts[b] = offsets[b+1] - offsets[b];
    }
    counts[num_bins-1] = num_elements - offsets[num_bins-1];
    // create unpacked_values[]
    unpacked_values = new T*[num_bins];
    size_type offset = 0;
    for(size_type b=0; b<num_bins; b++) {
        size_type bin_size = counts[b];
        if(bin_size > 0) {
            unpacked_values[b] = new T[bin_size];
            for(size_type e=0; e<bin_size; e++)
                unpacked_values[b][e] = packed_values[offset++];
        }
    }
    // destroy unpacked arrays
    delete [] offsets; offsets = NULL;
    delete [] packed_values; packed_values = NULL;
}

template<class T>
void JaggedArray<T>::print() const {
    if(counts == NULL) {    // packed mode
        std::cout << "packed JaggedArray" << std::endl;
        std::cout << "  num_bins: " << num_bins << std::endl;
        std::cout << "  num_elements: " << num_elements << std::endl;
        std::cout << "  offsets:  ";
        for(size_type i=0; i<num_bins; i++)
            std::cout << offsets[i] << " ";
        std::cout << std::endl;
        std::cout << "  values:   ";
        for(size_type i=0; i<num_elements; i++) {
            std::cout << packed_values[i];
        }
        std::cout << std::endl << std::endl;
    }
    else {  // unpacked mode
        std::cout << "unpacked JaggedArray" << std::endl;
        std::cout << "  num_bins: " << num_bins << std::endl;
        std::cout << "  num_elements: " << num_elements << std::endl;
        // print counts[]
        std::cout << "  counts: ";
        for(size_type i=0; i<num_bins; i++)
            std::cout << counts[i] << " ";
        std::cout << std::endl;
        // print values[]
        std::cout << "  values: ";
        bool to_continue = true;
        for(size_type j=0; to_continue; j++) {
            // print jth element in all the bins
            if( j > 0 )
                std::cout << "          ";  // covering the width of "  values: "
            to_continue = false;
            for(size_type b=0; b<num_bins; b++) {
                // std::cout << "DEBUG: j="<<j<<", b="<<b<<", counts[b]="<<counts[b] << std::endl;
                if( counts[b] == 0 || j > counts[b] - 1 ) {
                    std::cout << " ";
                    // std::cout << "skipping..." << std::endl;
                }
                else   {
                    std::cout << unpacked_values[b][j];
                    to_continue = true;
                }
                std::cout << " ";
            }
            std::cout << std::endl;
        }
    }
}

// add an element to the unpacked JaggedArray.
template<class T>
void JaggedArray<T>::addElement(size_type bin, const T& e) {
    if(counts == NULL) {    // packed mode
        std::cerr << "addElement cannot be done to a packed JaggedArray; unpack it first." << std::endl;
        exit(1);
    }
    T* old_array = unpacked_values[bin];
    // allocate a new array, copy the existing elements over, and append the new element
    unpacked_values[bin] = new T[counts[bin] + 1];
    if( counts[bin] != 0 )   {
        // copy from the old array
        for( size_type i=0; i<counts[bin]; i++)   {
            unpacked_values[bin][i] = old_array[i];
        }
        // delete the old array
        delete [] old_array;
    }
    unpacked_values[bin][counts[bin]] = e;
    counts[bin]++;
    num_elements++;
}

// remove an element from the unpacked JaggedArray.
template<class T>
void JaggedArray<T>::removeElement(size_type bin, size_type i) {
    if(counts == NULL) {    // packed mode
        std::cerr << "removeElement cannot be done to a packed JaggedArray; unpack it first." << std::endl;
        exit(1);
    }
    if(i >= counts[bin])    {
        std::cerr << "Cannot remove " << i << "th element in Bin " << bin << ", which only has "
                  << counts[bin] << "." << std::endl;
        exit(1);
    }
    assert(counts[bin] > 0);
    if(counts[bin] == 1) {  // just one element. Destroy the array.
        delete [] unpacked_values[bin];
    }
    else {  // more than 1 elements in the bin. Remove it
        T* old_array = unpacked_values[bin];
        // allocate a new array with size-1
        unpacked_values[bin] = new T[counts[bin] - 1];
        // copy remaining elements over
        for( size_type j=0; j<i; j++)   {
            unpacked_values[bin][j] = old_array[j];
        }
        for( size_type j=i+1; j<counts[bin]; j++)   {
            unpacked_values[bin][j-1] = old_array[j];
        }
    }
    counts[bin]--;
    num_elements--;
}

template<class T>
size_type JaggedArray<T>::numElementsInBin(size_type bin) const {
    if(counts == NULL) {    // packed mode
        assert(bin < num_bins);
        if(bin == num_bins-1)
            return num_elements - offsets[bin];
        else
            return offsets[bin+1] - offsets[bin];
    }
    else {  // unpacked mode
        return counts[bin];
    }
}

template<class T>
const T& JaggedArray<T>::getElement(size_type bin, size_type i) const {
    if(counts == NULL) {    // packed mode
        assert(bin < num_bins);
        return packed_values[ offsets[bin] + i ];
    }
    else {  // unpacked mode
        return unpacked_values[bin][i];
    }
}

#endif /* jagged_array_h */
