#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <time.h>

#include "image.h"
#include "visualization.h"
#include "hash.h"



// ======================================================================
// Helper function to read the optional arguments and filenames from
// the command line.
void parse_arguments(int argc, char* argv[], 
                     std::string& method, int& seed, int& table_size, float& compare,
                     std::vector<std::string>& filenames) {
    for (int i = 1; i < argc; i++) {
        if (argv[i] == std::string("-method")) {
            i++;
            assert (i < argc);
            method = argv[i];
            assert (method == "simple" || method == "hashtable");
        } else if (argv[i] == std::string("-seed")) {
            i++;
            assert (i < argc);
            seed = atoi(argv[i]);
            assert (seed >= 1);
        } else if (argv[i] == std::string("-table")) {
            i++;
            assert (i < argc);
            table_size = atoi(argv[i]);
            assert (table_size >= 1);
        } else if (argv[i] == std::string("-compare")) {
            i++;
            assert (i < argc);
            compare = atof(argv[i]);
            assert (compare > 0.0 && compare <= 1.0);
        } else {
            filenames.push_back(argv[i]);
        }
    }
    assert (filenames.size() > 0);
}


// ======================================================================
// This simple algorithm is rather inefficient, and may not find the
// largest overlapping subregion.  But it will find a subregion match
// of size seed x seed, if one exists.
void SimpleCompare(const Image<int>& a, const Image<int>& b, 
                   Image<Color> &out, int which_color,int seed, float& percent,
                   std::vector<std::pair<BoundingBox,BoundingBox> >& regions) {
    
    // First, find a small seed that matches
    bool found = false;
    Point offset_a;
    Point offset_b;
    
    // Search over all possible points in image a
    for (int i = 0; i <= a.Width()-seed && !found; i++) {
        for (int j = 0; j <= a.Height()-seed && !found; j++) {
            // Search over all possible points in image b
            for (int i2 = 0; i2 <= b.Width()-seed && !found; i2++) {
                for (int j2 = 0; j2 <= b.Height()-seed && !found; j2++) {
                    bool match = true;
                    // Check for small seed match
                    for (int x = 0; x < seed && match; x++) {
                        for (int y = 0; y < seed && match; y++) {
                            if (a.GetPixel(i+x,j+y) != b.GetPixel(i2+x,j2+y)) {
                                match = false;
                            }
                        }
                    }
                    if (match) {
                        // break out of these loops!
                        HighlightSeed(out,which_color,Point(i,j),seed);
                        found = true;
                        offset_a = Point(i,j);
                        offset_b = Point(i2,j2);
                    }
                }
            }
        }
    }
    if (!found) {
        // no match between these images
        percent = 0.0;
        return;
    }
    
    int width = seed;
    int height = seed;
    
    //
    //
    // ASSIGNMENT:  COMPLETE THIS IMPLEMENTATION
    //
    //
    
    // First, expand the region match widthwise, until we hit the right
    // edge of one of the images or a mismatched pixel.
    // Region: seed x seed box starting with offset_a and offset_b. width is to be incremented.
    for (bool matching=true; matching && width<b.Width()-offset_b.x && width<a.Width()-offset_a.x; width++) {
        for (int j=0; matching && j<height; j++) {
            if (a.GetPixel(offset_a.x+width, offset_a.y+j) != b.GetPixel(offset_b.x+width, offset_b.y+j))
                matching = false;
        }
    }
    width--;    // backtrack by 1 so the boundary also matches
    
    // Then, expand the region match heightwise, until we hit the top
    // edge of one of the images or a mismatched pixel.
    for (bool matching=true; matching && height<b.Height()-offset_b.y && height<a.Height()-offset_a.y; height++) {
        for (int i=0; matching && i<width; i++) {
            if (a.GetPixel(offset_a.x+i, offset_a.y+height) != b.GetPixel(offset_b.x+i, offset_b.y+height))
                matching = false;
        }
    }
    height--;    // backtrack by 1 so the boundary also matches

    // End of Assignment
    
    BoundingBox bbox_a(offset_a,Point(offset_a.x+width,offset_a.y+height));
    BoundingBox bbox_b(offset_b,Point(offset_b.x+width,offset_b.y+height));
    regions.push_back(std::make_pair(bbox_a,bbox_b));
    // return fraction of pixels
    percent = bbox_a.Width()*bbox_a.Height() / float (a.Width()*a.Height());
}

// ======================================================================

bool SeedMatch(const Seed& seed_a, const Seed& seed_b) {
    assert(seed_a.size() == seed_b.size());
    for (int x = 0; x < seed_a.size(); x++) {
        for (int y = 0; y < seed_a.size(); y++) {
            if (seed_a.getPixel(x,y) != seed_b.getPixel(x,y)) {
                return false;
            }
        }
    }
    
    return true;
}

void HashCompare(const Image<int>& a, const Image<int>& b,
                 Image<Color> &out, int which_color, int seed, float& percent,
                 size_t table_size, float compare,
                 std::vector<std::pair<BoundingBox,BoundingBox> >& regions) {
    
    hashset hashtable_a(table_size);
    HashImage(hashtable_a, a, seed);
    //HashInfo(hashtable_a);
    hashset hashtable_b(table_size);
    HashImage(hashtable_b, b, seed);
    //HashInfo(hashtable_b);

    // bounding box
    size_t x_min_a, x_min_b, y_min_a, y_min_b;
    size_t x_max_a, x_max_b, y_max_a, y_max_b;
    x_min_a = x_min_b = y_min_a = y_min_b = 1000;
    x_max_a = x_max_b = y_max_a = y_max_b = 0;
    
    // walk through hashtable a
    bool matching = false;
    size_t count = 0;
    // size_t attempts = 0;
    for (const Seed& s: hashtable_a) {
        count++;
        // only check percent*100% seeds
        float random = rand()*1.0/RAND_MAX;
        if (random >= compare)
            continue;
        // attempts++;
        size_t bucket_a = hashtable_a.bucket(s);
        size_t bucket_size_b = hashtable_b.bucket_size(bucket_a);
        // If s has the same hash value with seeds in Image b
        if (bucket_size_b) {
            // Need to make sure there is pixel-level match with one of the seeds in b's bucket
            for (hashset::local_iterator it = hashtable_b.begin(bucket_a);
                 it!= hashtable_b.end(bucket_a); ++it )
                if (SeedMatch(s,*it)) {
                    HighlightSeed(out, which_color, Point(s.x0(),s.y0()), seed);
                    if (s.x0() < x_min_a) x_min_a = s.x0();
                    if (s.y0() < y_min_a) y_min_a = s.y0();
                    if (s.x0() > x_max_a) x_max_a = s.x0();
                    if (s.y0() > y_max_a) y_max_a = s.y0();
                    if (it->x0() < x_min_b) x_min_b = it->x0();
                    if (it->y0() < y_min_b) y_min_b = it->y0();
                    if (it->x0() > x_max_b) x_max_b = it->x0();
                    if (it->y0() > y_max_b) y_max_b = it->y0();
                    matching = true;
                }
        }
    }
    /*
    std::cout << "    attempts/total=" << attempts << "/" << count
    << " or "
    << std::right << std::setw(7) << std::fixed << std::setprecision(4)
    << (attempts*1.0/count) << std::endl;
     */

    if (matching) {
        BoundingBox bbox_a(Point(x_min_a,y_min_a),Point(x_max_a+seed-1,y_max_a+seed-1));
        BoundingBox bbox_b(Point(x_min_b,y_min_b),Point(x_max_b+seed-1,y_max_b+seed-1));
        regions.push_back(std::make_pair(bbox_a,bbox_b));
        // return fraction of pixels
        percent = bbox_a.Width()*bbox_a.Height() / float (a.Width()*a.Height());
    }
}

// ======================================================================

int main(int argc, char* argv[]) {
    
    // default command line argument values
    std::string method = "simple";
    int seed = 5;
    int table_size = 1000000;
    float compare = 0.05;
    std::vector<std::string> filenames;
    parse_arguments(argc,argv,method,seed,table_size,compare,filenames);
    
    /* initialize random seed: */
    srand(time(NULL));
    
    // Load all of the images
    std::vector<Image<int> > images(filenames.size());
    for (int i = 0; i < filenames.size(); i++) {
        images[i].Load(filenames[i]);
    }
    
    // Loop over all input images, comparing to every other input image
    for (int a = 0; a < filenames.size(); a++) {
        std::cout << filenames[a] << std::endl;
        // prepare a color visualization image for each input file
        Image<Color> out;
        
        // printing out ===
        // marbles.pgm
        InitializeOutputFile(images[a],out);
        int which_color = -1;
        for (int b = 0; b < filenames.size(); b++) {
            if (a == b) continue;
            which_color++;
            
            // Find the one (or more!) subregions that match between this pair of images
            std::vector<std::pair<BoundingBox,BoundingBox> > regions;
            float percent = 0.0;
            if (method == "simple") {
                SimpleCompare(images[a],images[b],out,which_color,seed,percent,regions);
            } else {
                assert (method == "hashtable");
                
                //
                //
                // ASSIGNMENT:  IMPLEMENT THE HASHTABLE MATCHING ALGORITHM
                //
                //
                HashCompare(images[a],images[b],out,which_color,seed,percent,
                            table_size,compare,regions);
            }

            // printing out ===
            //    10.7% match with marbles_1.pgm  (50,100):(149,199) similar to (0,0):(99,99)
            std::cout << std::right << std::setw(7) << std::fixed << std::setprecision(1)
            << 100.0 * percent << "% match with " << std::left << std::setw(20) << filenames[b];
            
            for (int i = 0; i < regions.size(); i++) {
                std::cout << "   " << regions[i].first << " similar to " << regions[i].second;
                // add the frame data to the visualization image
                DrawBoundingBox(out,regions[i].first,which_color);
            }
            std::cout << std::endl;
            // Save the visualization image
            std::string f = "output_" + filenames[a].substr(0,filenames[a].size()-4) + ".ppm";
            out.Save(f);
        }
    }
}
