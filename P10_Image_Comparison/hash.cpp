#include "hash.h"


// Seed functions

std::ostream& operator<< (std::ostream& ostr, const Seed& seed) {
    ostr << "Seed at (" << seed.x0() << "," << seed.y0() << ") size " << seed.size();
    return ostr;
}

// ======================================================================

// Find all possible seeds, and add them to the hashtable
void HashImage(hashset& hashtable, const Image<int>& image, int seed_size) {
    for (int i = 0; i <= image.Width() - seed_size; i++) {
        for (int j = 0; j <= image.Height() - seed_size; j++) {
            bool success = hashtable.insert(Seed(image, i, j, seed_size)).second;
            assert(success);
        }
    }
}

// prints out hashtable's size, max_size, bucket_count, max_bucket_count, bucket_size,
// and how many elements have 1 or more collisions
void HashInfo(const hashset& hashtable) {
    unsigned collisions = 0;
    for (unsigned i=0; i<hashtable.bucket_count(); i++) {
        unsigned size = hashtable.bucket_size(i);
        if (size > 1)
            collisions += size;
    }
    std::cout
    << "    Hashset: size=" << hashtable.size()
    << ", bucket_count=" << hashtable.bucket_count()
    << "; collisions=" << collisions
    << " or " << collisions*100.0/hashtable.size()
    << "%" << std::endl;
}

