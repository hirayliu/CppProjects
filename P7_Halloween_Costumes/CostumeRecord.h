//
//  CostumeRecord.h
//  
//
//  Created by rliu on 11/1/17.
//

#ifndef CostumeRecord_h
#define CostumeRecord_h

#include <vector>
#include <cassert>
#include "person.h"

class CostumeRecord {
public:
    CostumeRecord(int copies) { copies_available_ = copies; }
    int copies() const { return copies_available_; }
    const std::vector<Person>& renters() const { return renters_; }
    void add(int copies) { copies_available_ += copies; }
    bool renting(const Person& p);
    bool rent(const Person& p);
    void ret(const Person& p);
private:
    int copies_available_;
    std::vector<Person> renters_;
};

#endif /* CostumeRecord_h */
