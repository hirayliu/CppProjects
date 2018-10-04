#include <vector>
#include <iomanip>

#include "image.h"
#include "visualization.h"



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

  // Then, expand the region match heightwise, until we hit the top
  // edge of one of the images or a mismatched pixel.

  //
  //
  //

  BoundingBox bbox_a(offset_a,Point(offset_a.x+width,offset_a.y+height));
  BoundingBox bbox_b(offset_b,Point(offset_b.x+width,offset_b.y+height));
  regions.push_back(std::make_pair(bbox_a,bbox_b));
  // return fraction of pixels
  percent = bbox_a.Width()*bbox_a.Height() / float (a.Width()*a.Height());
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

      } 

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
