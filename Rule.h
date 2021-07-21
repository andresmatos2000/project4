//
// Created by munch on 7/3/2021.
//

#ifndef PROJECT1_RULE_H
#define PROJECT1_RULE_H
#include "Predicate.h"
#include <vector>
class Rule {
private:
    Predicate* headPredicate;
public:
    std::vector<Predicate*> predicateList;
    void setHeadPredicate(Predicate* headPredicate);
    Predicate* getHeadPredicate();
    std::string To_String();
    void addPredicate(Predicate* predicate);
    Rule(Predicate* headPredicate){
        this->headPredicate = headPredicate;
    }
};

void Rule::setHeadPredicate(Predicate* headPredicate) {
    this->headPredicate = headPredicate;
}

void Rule::addPredicate(Predicate* predicate) {
    predicateList.push_back(predicate);
}
Predicate* Rule::getHeadPredicate(){
    return this->headPredicate;
}
std::string Rule::To_String(){
    std::string fullString = this->getHeadPredicate()->To_String() + " :- ";
    for (unsigned int i = 0; i < predicateList.size(); ++i) {
        if(i == predicateList.size()-1){
            fullString += predicateList[i]->To_String();
        } else {
            fullString += predicateList[i]->To_String() + ",";
        }
    }
    return fullString;
}
#endif //PROJECT1_RULE_H
