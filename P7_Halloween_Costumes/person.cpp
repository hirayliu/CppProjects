//
//  person.cpp
//  
//
//  Created by rliu on 11/1/17.
//

#include <iostream>
#include "person.h"

bool operator==(const Person& x, const Person& y) {
    return x.last_name() == y.last_name() && x.first_name() == y.first_name();
}

bool operator< (const Person& x, const Person& y) {
    return x.last_name() < y.last_name() ||
    ( x.last_name() == y.last_name() && x.first_name() < y.first_name() );
}

std::ostream& operator<<(std::ostream& os, const Person& p) {
    os << p.first_name() << " " << p.last_name();
    return os;
}

