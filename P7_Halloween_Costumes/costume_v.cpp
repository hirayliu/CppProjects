//
//  costume_v.cpp
//  
//
//  Created by rliu on 11/1/17.
//

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cctype>
#include <iomanip>
#include "person.h"
#include "CostumeRecord.h"
#include "costume_v.h"

// finds costume in costume_shop, and returns the iterator
COSTUME_SHOP_TYPE_v::iterator find(COSTUME_SHOP_TYPE_v& costume_shop, const std::string& costume_name) {
    COSTUME_SHOP_TYPE_v::iterator itr;
    for (itr=costume_shop.begin(); itr!=costume_shop.end(); itr++)
        if (itr->first == costume_name)
            return itr;
    return itr;
}

// get person's current costume if any. If person is not registered, register him/her first.
std::string& get(PEOPLE_TYPE_v& people, const Person& person) {
    PEOPLE_TYPE_v::iterator itr;
    // people vector is sorted by first element so the order is the same with the map version
    for (itr=people.begin(); itr!=people.end(); itr++) {
        if (itr->first == person)
            return itr->second;
        if (person < itr->first) {
            break;
        }
    }
    // either itr == people.end(), or itr->first>person. Insert
    itr = people.insert(itr, std::make_pair(person, std::string()));
    return itr->second;
}

/*
 Format: a ghost 2
 
 This operation will add a specified number of copies of a particular costume type
 into the costume shop inventory. If the costume is already in the shop,
 the number of copies is incremented. A useful message is printed to
 the screen following successful execution of this file.
 
 Prints out
 Added 1 ballerina costume.
 Or
 Added 4 pirate costumes.
 */
void addCostume(COSTUME_SHOP_TYPE_v& costume_shop) {
    std::string costume_name;
    int num;
    std::cin >> costume_name >> num;
    COSTUME_SHOP_TYPE_v::iterator itr = find(costume_shop, costume_name);
    if (itr == costume_shop.end()) {
        costume_shop.push_back(std::make_pair(costume_name, CostumeRecord(num)));
    }
    else {
        itr->second.add(num);
    }
    std::cout << "Added " << num << " " << costume_name;
    std::cout << ( num==1 ? " costume." : " costumes." ) << std::endl;
}

/*
 Format: r Sally Smith princess
 
 In this operation, a customer is attempting to rent a particular costume from the shop.
 The customer name is specified by two strings: his/her first and last names, in that order,
 and then the costume is specified by a single string, as above.
 If the shop does not carry that type of costume, if no copies are currently available,
 or if the person is currently renting a copy of this costume, then the request is declined
 with an appropriate message (see sample output). Each person is only allowed to rent
 one costume at a time; thus, if the person is currently renting a different costume,
 then that costume must first be returned, which will be done automatically by your code.
 Messages indicating the returned costume (if any) and rented costume are output.
 
 Prints out
 Betty Lee rents a Cinderella costume.
 Or
 Betty Lee returns a Cinderella costume before renting a transformer costume.
 Or
 Paul Rodriguez already has a pirate costume.
 Or
 No transformer costumes available.
 Or
 The shop doesn't carry witch costumes.
 */
void rentCostume(COSTUME_SHOP_TYPE_v& costume_shop, PEOPLE_TYPE_v& people) {
    std::string first_name;
    std::string last_name;
    std::string costume_name;
    std::cin >> first_name >> last_name >> costume_name;
    
    Person person(first_name, last_name);
    
    // get person's current costume if any. If person is not registered, register him/her.
    std::string& old_costume = get(people, person);
    
    // If no such costume, indicate so, and return.
    COSTUME_SHOP_TYPE_v::iterator itr = find(costume_shop, costume_name); // use reference?
    if (itr == costume_shop.end()) {
        std::cout << "The shop doesn't carry " << costume_name << " costumes." << std::endl;
        return;
    }
    
    // If no available copies, indicate so, and return.
    if (itr->second.copies() == 0) {
        std::cout << "No " << costume_name << " costumes available." << std::endl;
        return;
    }
    
    CostumeRecord& rec = itr->second;
    
    // If person already has the costume...
    if (rec.renting(person)) {
        std::cout << person << " already has a " << costume_name << " costume." << std::endl;
        return;
    }
    
    if (old_costume.empty()) {
        std::cout << person << " rents a " << costume_name << " costume." << std::endl;
    }
    // If person current rents another costume, return it.
    else {
        COSTUME_SHOP_TYPE_v::iterator old_itr = find(costume_shop, old_costume);
        assert(old_itr != costume_shop.end());
        old_itr->second.ret(person);
        std::cout << person << " returns a " << old_costume << " costume before renting a ";
        std::cout << costume_name << " costume." << std::endl;
    }

    // Let person rent the costume
    rec.rent(person);
    
    // put(people, person, costume_name);
    old_costume = costume_name;     // this should replace the current costume with the new one for the person
}

/*
 Prints out
 Shop info for pirate costumes:
 3 copies available
 1 copy rented by:
 Paul Rodriguez
 Or
 Shop info for transformer costumes:
 2 copies rented by:
 Betty Lee
 Sharon Jackson
 Or
 Shop info for Cinderella costumes:
 1 copy available
 Or
 The shop doesn't carry skeleton costumes.
 */
void lookup(COSTUME_SHOP_TYPE_v& costume_shop) {
    std::string costume_name;
    std::cin >> costume_name;
    
    COSTUME_SHOP_TYPE_v::iterator itr = find(costume_shop, costume_name);
    if (itr == costume_shop.end()) {
        std::cout << "The shop doesn't carry " << costume_name << " costumes." << std::endl;
    }
    else {
        CostumeRecord& rec = itr->second;
        std::cout << "Shop info for " << costume_name << " costumes:" << std::endl;
        if (rec.copies() > 0)   {
            std::cout << std::setw(3) << rec.copies();
            std::cout << (rec.copies()==1 ? " copy" : " copies");
            std::cout << " available" << std::endl;
        }
        const std::vector<Person>& renters = rec.renters();
        if (!renters.empty()) {
            int num = renters.size();
            std::cout << std::setw(3) << num;
            std::cout << (num==1 ? " copy" : " copies") << " rented by:" << std::endl;
            for (int i=0; i<num; i++)
                std::cout << "    " << renters[i] << std::endl;
        }
    }
}


/*
 This operation prints all customers who have ever attempted to rent from the shop
 (in alphabetical order by last name then first name) and the costume
 (if any) that they are currently renting.
 
 Prints out
 Costume info for the 4 party attendees:
 Mary Brown does not have a costume.
 Sharon Jackson is wearing a transformer costume.
 Betty Lee is wearing a transformer costume.
 Paul Rodriguez is wearing a pirate costume.
 Or
 Costume info for the 1 party attendee:
 Sharon Jackson is wearing a pirate costume.
 */
void printPeople(PEOPLE_TYPE_v& people) {
    int size = people.size();
    
    if (size == 0)
        return;
    
    std::cout << "Costume info for the " << size << " party ";
    std::cout << (size==1 ? "attendee:" : "attendees:") << std::endl;
    for (PEOPLE_TYPE_v::iterator itr=people.begin(); itr!=people.end(); itr++) {
        std::cout << "  " << itr->first << " ";
        if (itr->second.empty())
            std::cout << "does not have a costume." << std::endl;
        else
            std::cout << "is wearing a " << itr->second << " costume." << std::endl;
    }
}
