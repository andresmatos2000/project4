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
    Relation* getRelation(std::string string);
};
Relation* Database::getRelation(std::string string){
    return relations.find(string)->second;
}
void Database::addRelation(std::string name, Relation* relation){
    relations.insert(std::pair<std::string,Relation*>(name,relation));
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
