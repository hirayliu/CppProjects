//
//  costume_v.h
//  
//
//  Created by rliu on 11/1/17.
//

#ifndef costume_v_h
#define costume_v_h

#include <string>
#include <vector>
#include "person.h"
#include "CostumeRecord.h"

// maps costume to CostumeRecord
typedef std::vector<std::pair<std::string, CostumeRecord> > COSTUME_SHOP_TYPE_v;

// maps person's Name to his/her costume (if any)
typedef std::vector<std::pair<Person, std::string> > PEOPLE_TYPE_v;

// prototypes for the helper functions
void addCostume(COSTUME_SHOP_TYPE_v &costume_shop);
void rentCostume(COSTUME_SHOP_TYPE_v& costume_shop, PEOPLE_TYPE_v& people);
void lookup(COSTUME_SHOP_TYPE_v& costume_shop);
void printPeople(PEOPLE_TYPE_v& people);

#endif /* costume_v_h */
