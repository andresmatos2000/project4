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
    void addTuple(Tuple tuple);
    void toString(int variableSize);
    Relation* select(Relation* relation, int index1, int index2);
    Relation* select(Relation* relation, std::string index1, int index2);
    int getSize();
    Relation* project(Relation* relation,std::vector<int> recordedVariables,std::map<int, std::string> variables);
    Relation* rename(Relation* relation,std::map<int, std::string> constants);

    Relation(std::string name, Header* header){
        this->name = name;
        this->header = header;
    };

    ~Relation(){
        delete header;
        Tuples.clear();
    }
};
void Relation::toString(int variableSize) {
    if(variableSize == 0)
        ;
        else
    for(Tuple i : Tuples){
        if(i.values.size() == 1){
            std::cout << "  " + header->getValue(0) + "=" + i.values[0] + "";
        } else
        for(unsigned int j = 0; j < i.values.size(); j++){
            if(j == 0){ //first
                std::cout << "  " + header->getValue(j) + "=" + i.values[j] +  ", ";
            }
            else if(j == i.values.size()-1) // last
                std::cout << header->getValue(j) + "=" + i.values[j];
            else {
                std::cout << header->getValue(j) + "=" + i.values[j] + ", ";
            }
        };
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


}

Relation *Relation::rename(Relation* relation,std::map<int, std::string> variables) {
    Header* temp = new Header(relation->header);
    std::set<Tuple> newTuples = relation->Tuples;
    Relation* newRelation = new Relation(relation->name,temp);
    for(auto i: variables){
        temp->changeHeader(i.second,i.first);
    }
    for(auto i: newTuples){
        newRelation->addTuple(i);
    }

    return newRelation;

}

void Relation::addTuple(Tuple tuple) {
    Tuples.insert(tuple);
};
#endif //PROJECT1_RELATION_H
