//

#ifndef PROJECT1_TUPLE_H
#define PROJECT1_TUPLE_H
#include <vector>
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
        }
    };
    Tuple();
    void addTuple(std::string value);
    void toString();
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
void Tuple::toString(){
    for(auto i: this->values){
        std::cout << i  + " ";
    }
}

#endif //PROJECT1_TUPLE_H
