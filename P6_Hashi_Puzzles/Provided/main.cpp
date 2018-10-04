#include <fstream>
#include <cassert>
#include <cstdlib>
#include <iostream>

#include "graph.h"


int main(int argc, char* argv[] ) {

  
  // Parse the arguments
  if (argc < 2) {
    std::cout << "ERROR!  Must specify input file" << std::endl;
    exit(1);
  }
  std::ifstream istr(argv[1]);
  if (!istr.good()) {
    std::cout << "ERROR!  Could not open input file '" << argv[1] << "'" << std::endl;
    exit(1);
  }
  bool find_all_solutions = false;
  bool connected = false;
  for (int i = 2; i < argc; i++) {
    if (argv[i] == std::string("--find_all_solutions")) {
      find_all_solutions = true;
    } else if (argv[i] == std::string("--connected")) {
      connected = true;
    } else {
      std::cout << "ERROR!  Unknown argument '" << argv[i] << "'" << std::endl;
      exit(1);
    }
  }


  // Create an empty graph object
  Graph g;
  // Read in the puzzle from the input file
  int x,y,n;
  // std::vector<Point> pts;
  while (istr >> x >> y >> n) {
    pts.push_back(Point(x,y));
    g.addNode(Point(x,y),n);
  }

  // The statements below hardcode the solution to puzzle1.txt.
  // You should remove this code and replace it with a general solver.
    /*
  if (argv[1] == std::string("puzzle1.txt")) {
    g.addEdge(pts[0],pts[1]);
    g.addEdge(pts[0],pts[1]);
    g.addEdge(pts[1],pts[2]);
    g.addEdge(pts[1],pts[2]);
    g.addEdge(pts[2],pts[3]);
  }
     */
    
    g.hashi();
    
    // print the edges (any order is ok)
    g.printSolution();

    // print a visualization of the (solved or unsolved) puzzle
    g.printGraph();

}
