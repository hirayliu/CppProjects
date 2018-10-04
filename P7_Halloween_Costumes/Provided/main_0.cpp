#include <iostream>
#include <string>

typedef ? COSTUME_SHOP_TYPE;
typedef ? PEOPLE_TYPE;

// prototypes for the helper functions
void addCostume(COSTUME_SHOP_TYPE &costume_shop);
void rentCostume(COSTUME_SHOP_TYPE& costume_shop, PEOPLE_TYPE& people);
void lookup(COSTUME_SHOP_TYPE& costume_shop);
void printPeople(PEOPLE_TYPE& people);

int main() {
  // two data structures store all of the information for efficiency
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


void addCostume(COSTUME_SHOP_TYPE& costume_shop) {
  std::string costume_name;
  int num;
  std::cin >> costume_name >> num;





}


void rentCostume(COSTUME_SHOP_TYPE& costume_shop, PEOPLE_TYPE& people) {
  std::string first_name;  
  std::string last_name;
  std::string costume_name;
  std::cin >> first_name >> last_name >> costume_name;





}


void lookup(COSTUME_SHOP_TYPE& costume_shop) {
  std::string costume_name;
  std::cin >> costume_name;





}


void printPeople(PEOPLE_TYPE& people) {






}
