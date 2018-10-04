#include <iostream>
#include <string>
//#include "person.h"
//#include "CostumeRecord.h"
#include "costume.h"
#include "costume_v.h"

int main(int argc, char* argv[]) {
    bool use_vector = false;
    
    if (argc == 2) {
        std::string argv1(argv[1]);
        if (argv1 == "--use_vector")
            use_vector = true;
        else {
            std::cerr << "Usage: " << argv[0] << " --use_vector" << std::endl;
            exit(1);
        }
    }
    
    // two data structures store all of the information for efficiency
    if (!use_vector) {
        COSTUME_SHOP_TYPE costume_shop;
        PEOPLE_TYPE people;
        char c;
        while (std::cin >> c) {
            if (c == 'a') {
                addCostume(costume_shop);
            } else if (c == 'r') {
                rentCostume(costume_shop,people);
            } else if (c == 'l') {
                lookup(costume_shop);
            } else if (c == 'p') {
                printPeople(people);
            } else {
                std::cerr << "error unknown char " << c << std::endl;
                exit(0);
            }
        }
    }
    else {
        COSTUME_SHOP_TYPE_v costume_shop;
        PEOPLE_TYPE_v people;
        char c;
        while (std::cin >> c) {
            if (c == 'a') {
                addCostume(costume_shop);
            } else if (c == 'r') {
                rentCostume(costume_shop,people);
            } else if (c == 'l') {
                lookup(costume_shop);
            } else if (c == 'p') {
                printPeople(people);
            } else {
                std::cerr << "error unknown char " << c << std::endl;
                exit(0);
            }
        }
    }
}
