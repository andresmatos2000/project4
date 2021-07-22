//
// Created by munch on 7/15/2021.
//

#ifndef PROJECT1_DATABASE_H
#define PROJECT1_DATABASE_H
#include <vector>
#include <map>
#include "Relation.h"
class Database {
private:
std::map<std::string, Relation*> relations;
public:
    void addRelation(std::string name, Relation* relation);
    void print();
    void addFactsToRelation(std::string name, Tuple tuple);
    int addTuplesToRelation(std::string name, Relation* relation);
    void toString(std::string,int);
    Relation* getRelation(std::string string);
};
Relation* Database::getRelation(std::string string){
    return relations.find(string)->second;
}
void Database::toString(std::string name,int added){
    Relation* relation = relations.find(name)->second;
    if(added == 0)
        ;
    else
        for(Tuple i : relation->getTuples()) {
            if (i.values.size() == 1) {
                std::cout << "  " + relation->getHeader()->getValue(0) + "=" + i.values[0] + "";
            } else {
                for (unsigned int j = 0; j < i.values.size(); j++) {
                    if (j == 0) { //first
                        std::cout << "  " + relation->getHeader()->getValue(j) + "=" + i.values[j] + ", ";
                    } else if (j == i.values.size() - 1) // last
                        std::cout << relation->getHeader()->getValue(j) + "=" + i.values[j];
                    else {
                        std::cout << relation->getHeader()->getValue(j) + "=" + i.values[j] + ", ";
                    }
                };
            }
            std::cout << std::endl;
        }
};
void Database::addRelation(std::string name, Relation* relation){
    relations.insert(std::pair<std::string,Relation*>(name,relation));
}
int Database::addTuplesToRelation(std::string name, Relation* relation){
    int isAdded = 0;
    for(auto i: relation->getTuples()) {
        bool added = false;
        added = relations.find(name)->second->addTuple(i);
        if (added) {
            isAdded++;
            i.toString(relations.find(name)->second->getHeader());
        }
    }
return isAdded;
}
void Database::addFactsToRelation(std::string name, Tuple tuple){
    for(auto i: relations){
        if(i.first == name){
            i.second->addTuple(tuple);
        }
    }
};
void Database::print(){
  for(auto i: relations){
      std::cout << i.first << std::endl;
  }
};
#endif //PROJECT1_DATABASE_H
