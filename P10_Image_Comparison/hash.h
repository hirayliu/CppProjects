#ifndef __HASH_H__
#define __HASH_H__

#include <iostream>
#include <unordered_set>
#include "image.h"

// ======================================================================
// A size x size seed in the image

class Seed {
public:
    Seed(const Image<int>& i, unsigned int x0, unsigned int y0, int s)
    : image_(i), x0_(x0), y0_(y0), size_(s) {}
    bool operator==(const Seed& other) const {
        return x0_==other.x0_ && y0_==other.y0_ && size_==other.size_;
    }
    // x and y are relative to x0 and y0.
    int getPixel(unsigned int x, unsigned int y) const {
        return image_.GetPixel(x0_+x, y0_+y);
    }
    unsigned int x0() const { return x0_; }
    unsigned int y0() const { return y0_; }
    unsigned int size() const { return size_; }
private:
    const Image<int>& image_;
    unsigned int x0_;
    unsigned int y0_;
    int size_;
};



std::ostream& operator<< (std::ostream &ostr, const Seed& seed);

// ======================================================================

class MyHashFunctor {
public:
    // TODO: improve the hash function. Look at the collision rate. 
    unsigned int operator() (const Seed& seed) const {
        unsigned int hash = 1315423911;
        for(unsigned int i=0; i<seed.size(); i++) {
            unsigned int sum = 0;
            for(unsigned int j=0; j<seed.size(); j++) {
                sum = (sum << 4) + seed.getPixel(i,j);
            }
            hash ^= ((hash << 5) + sum + (hash >> 2));
        }
        return hash;
    }
};

typedef std::unordered_set<Seed,MyHashFunctor> hashset;

void HashImage(hashset& hashtable, const Image<int>& image, int seed_size);

// prints out hashtable's size, max_size, bucket_count, max_bucket_count, bucket_size,
// and how many elements have 1 or more collisions
void HashInfo(const hashset& hashtable);

#endif
