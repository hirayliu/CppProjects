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
    std::vector<Point> pts;
    while (istr >> x >> y >> n) {
        pts.push_back(Point(x,y));
        g.addNode(Point(x,y),n);
    }
    
    g.hashi(connected, find_all_solutions);
    
}
