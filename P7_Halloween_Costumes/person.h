//
//  person.h
//  
//
//  Created by rliu on 11/1/17.
//

#ifndef person_h
#define person_h

#include <string>

class Person {
public:
    Person(std::string first, std::string last) : first_(first), last_(last) {};
    const std::string& first_name() const { return first_; }
    const std::string& last_name() const { return last_; }
private:
    std::string first_;
    std::string last_;
};

bool operator==(const Person& x, const Person& y);

bool operator<(const Person& x, const Person& y);

std::ostream& operator<<(std::ostream& os, const Person& p);

#endif /* person_h */
