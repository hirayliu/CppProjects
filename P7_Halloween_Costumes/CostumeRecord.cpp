//
//  CostumeRecord.cpp
//  
//
//  Created by rliu on 11/1/17.
//

#include <stdio.h>
#include "CostumeRecord.h"

bool CostumeRecord::rent(const Person& p) {
    if(copies_available_>0) {
        copies_available_--;
        renters_.push_back(p);
        return true;
    }
    else
        return false;
}

bool CostumeRecord::renting(const Person& p) {
    std::vector<Person>::iterator itr;
    for (itr=renters_.begin(); itr!=renters_.end(); itr++)
        if (*itr == p)
            break;
    return itr != renters_.end();
}

void CostumeRecord::ret(const Person& p) {
    copies_available_++;
    std::vector<Person>::iterator itr;
    for (itr=renters_.begin(); itr!=renters_.end(); itr++)
        if (*itr == p)
            break;
    assert(itr != renters_.end());
    renters_.erase(itr);
}
