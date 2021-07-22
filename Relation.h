//
// Created by munch on 7/12/2021.
//

#ifndef PROJECT1_RELATION_H
#define PROJECT1_RELATION_H
#include "Tuple.h"
#include "Header.h"
#include <set>
class Relation {
private:
    std::string name;
    Header* header;
    std::set<Tuple> Tuples;

public:
    bool addTuple(Tuple tuple);
    void toString(int variableSize);
    Relation* select(Relation* relation, int index1, int index2);
    Relation* select(Relation* relation, std::string index1, int index2);
    int getSize();
    Relation* project(Relation* relation,std::vector<int> recordedVariables,std::map<int, std::string> variables);
    Relation* projectRule(Relation* relation,Rule* rule);
    Relation* rename(Relation* relation,std::map<int, std::string> constants);
    Relation* renameRule(Relation* relation, std::map<int,std::string> variables);
    Header* getHeader();
    std::set<Tuple> getTuples();
    void setName(std::string name);
    void setTuples(std::set<Tuple> tuples);
    std::map<int, std::string> getVariables(std::string name,Header* header);
    std::string getName();
    Relation(std::string name, Header* header){
        this->name = name;
        this->header = header;
    };

    ~Relation(){
        delete header;
        Tuples.clear();
    }
};
void Relation::setName(std::string name){
    this->name = name;
}
void Relation::setTuples(std::set<Tuple> tuples) {
    this->Tuples = tuples;
}
Header* Relation::getHeader(){
    return this->header;
}
std::set<Tuple> Relation::getTuples(){
  return this->Tuples;
};

std::map<int, std::string> Relation::getVariables(std::string name,Header* header){
    std::vector<std::string> list = header->getHeaderList();
    std::map<int,std::string> newMap;
    for(int i=0; i < name.size(); i++){
    std::string s(1,name[i]);
        for(int j=0; j < list.size();j++){
            if(s == list[j]){
                newMap.insert(std::pair<int,std::string>(j,list[j]));
            }
        }
    }
    return newMap;
}
std::string Relation::getName(){
    return this->name;
}
void Relation::toString(int variableSize) {
    if(variableSize == 0)
        ;
        else
    for(Tuple i : Tuples) {
            if (i.values.size() == 1) {
                std::cout << "  " + header->getValue(0) + "=" + i.values[0] + "";
            } else {
                for (unsigned int j = 0; j < i.values.size(); j++) {
                    if (j == 0) { //first
                        std::cout << "  " + header->getValue(j) + "=" + i.values[j] + ", ";
                    } else if (j == i.values.size() - 1) // last
                        std::cout << header->getValue(j) + "=" + i.values[j];
                    else {
                        std::cout << header->getValue(j) + "=" + i.values[j] + ", ";
                    }
                };
            }
            std::cout << std::endl;
    }
}

int Relation::getSize(){
    return Tuples.size();
}
Relation *Relation::select(Relation* relation, int index1, int index2) {
    Relation* newRelation = new Relation(relation->name,relation->header);
    std::set<Tuple> newTuples = relation->Tuples;
    int j = 0;
    for(auto i: newTuples){
        if(i.getTuple().size() == 0){
            return newRelation;
        }
        if(i.getTuple()[index2] == i.getTuple()[index1]){
            newRelation->addTuple(i);
        }
        j++;
    }
    return newRelation;
}
Relation *Relation::select(Relation* relation, std::string index1, int index2) {
    Relation* newRelation = new Relation(relation->name,relation->header);
    std::set<Tuple> newTuples = relation->Tuples;

    for(auto i: newTuples){
        if(index1 == i.getTuple()[index2]){
            newRelation->addTuple(i);
        }
    }
    return newRelation;
}
Relation *Relation::project(Relation* relation,std::vector<int> recordedVariables, std::map<int, std::string> variables) {
    Header* temp = new Header(relation->header);
    std::vector<int> indexes = temp->fixHeader(variables);
    Relation* newRelation = new Relation(relation->name,temp);
    std::set<Tuple> newTuples = relation->Tuples;
    for(auto i : newTuples){
        std::vector<std::string> tuple;
        for(unsigned int j = 0; j < recordedVariables.size(); j++){
            tuple.push_back(i.getTuple()[recordedVariables[j]]);
        }
        newRelation->addTuple(tuple);
    }
    return newRelation;


};
Relation *Relation::rename(Relation* relation,std::map<int, std::string> variables) {
    Header* temp = new Header(relation->header);
    std::set<Tuple> newTuples = relation->Tuples;
    Relation* newRelation = new Relation(relation->name,temp);
    if(variables.size()>1) {
        for (auto i: variables) {
            temp->changeHeader(i.second, i.first);
        }
    } else{
        temp->addHeader(variables.find(0)->second);
    }
    for(auto i: newTuples){
        newRelation->addTuple(i);
    }

    return newRelation;

}
Relation* Relation::renameRule(Relation* relation, std::map<int,std::string> variables){
    std::vector<std::string> newHeader;
    std::vector<std::string> originalHeader = relation->getHeader()->getHeaderList();
    for(auto i: relation->getName() ){
        std::string s(1,i);
        newHeader.push_back(s);
    }
    relation->getHeader()->setHeader(newHeader);
    return relation;
}
Relation* Relation::projectRule(Relation* relation,Rule* rule){
  std::set<Tuple> tuples = relation->getTuples();
  std::set<Tuple> newTuples;
  std::vector<std::string> test =  relation->getHeader()->getHeaderList();
  std::vector<Parameter *> ruleVariables = rule->getHeadPredicate()->getParameters();
  std::vector<int> keepValues;
  for(auto i: ruleVariables){
      int k = 0;
      for(auto j: relation->getHeader()->getHeaderList()){
          if(i->getValue() == j){
          keepValues.push_back(k);
          }
          k++;
      }
  }
  for(auto i : tuples){
      std::vector<std::string> tuple;
      for(auto j : keepValues){
          tuple.push_back(i.getTuple()[j]);
      }
      newTuples.insert(Tuple(tuple));
  }
 // Relation* newRelation = new Relation(relation->getName(),relation->getHeader());
  relation->Tuples = newTuples;
  return relation;
};
bool Relation::addTuple(Tuple tuple) {
    int current = Tuples.size();
    Tuples.insert(tuple);
    int now = Tuples.size();
    if(now>current){
        return true;
    } else{
        return false;
    }
}

#endif //PROJECT1_RELATION_H
