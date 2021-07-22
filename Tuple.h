//

#ifndef PROJECT1_TUPLE_H
#define PROJECT1_TUPLE_H
#include <vector>
#include "Header.h"

//
// Created by munch on 7/12/2021.
//• Tuple - contains vector of values (string)
class Tuple {
private:
public:
    std::vector<std::string> values;
    Tuple(std::vector<std::string> values){
        for(auto i : values){
            this->values.push_back(i);
        };
    };
    Tuple();
    void addTuple(std::string value);
    void toString(Header* header);
    std::vector<std::string>getTuple();
    bool operator< (const Tuple & other) const;
};
bool Tuple::operator< (const Tuple & other) const {
    return this->values < other.values;
}
void Tuple::addTuple(std::string value) {
  values.push_back(value);
};
std::vector<std::string> Tuple::getTuple(){
    return values;
}
void Tuple::toString(Header* header){
        if (this->values.size() == 1) {
            std::cout << "  " + header->getValue(0) + "=" + this->values[0] + "";
        } else {
            for (unsigned int j = 0; j < this->values.size(); j++) {
                if (j == 0) { //first
                    std::cout << "  " + header->getValue(j) + "=" + this->values[j] + ", ";
                } else if (j == this->values.size() - 1) // last
                    std::cout << header->getValue(j) + "=" + this->values[j];
                else {
                    std::cout << header->getValue(j) + "=" + this->values[j] + ", ";
                }
            };
        }
        std::cout << std::endl;
    }

#endif //PROJECT1_TUPLE_H
