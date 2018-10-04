//
//  costume.h
//  
//
//  Created by rliu on 11/1/17.
//

#ifndef costume_h
#define costume_h

#include <string>
#include <map>
#include "person.h"
#include "CostumeRecord.h"

// maps costume to CostumeRecord
typedef std::map<std::string, CostumeRecord> COSTUME_SHOP_TYPE;

// maps person's Name to his/her costume (if any)
typedef std::map<Person, std::string> PEOPLE_TYPE;

// prototypes for the helper functions
void addCostume(COSTUME_SHOP_TYPE& costume_shop);
void rentCostume(COSTUME_SHOP_TYPE& costume_shop, PEOPLE_TYPE& people);
void lookup(COSTUME_SHOP_TYPE& costume_shop);
void printPeople(PEOPLE_TYPE& people);

#endif /* costume_h */
