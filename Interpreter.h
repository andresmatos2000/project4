//
// Created by munch on 7/12/2021.
//

#ifndef PROJECT1_INTERPRETER_H
#define PROJECT1_INTERPRETER_H
#include "DatalogProgram.h"
#include "Database.h"
#include "Header.h"
#include "Tuple.h"
#include "Relation.h"
class Interpreter {
private:
    DatalogProgram* datalog;
    Database* database;
public:

    Interpreter(DatalogProgram* datalog){
        this->datalog = datalog;
        this->database = new Database();
        createDatabase();
    };
    Relation* evaluatePredicate(Predicate* p);
    void createDatabase();
    Relation* evaluateRules(Rule* rule);
    Relation* join(Relation* r1, Relation* r2);
    std::pair<std::vector<std::pair<int,int>>,Header*> combineHeaders(Header* h1, Header* h2);
    Tuple combineTuples(Tuple i, Tuple j);
};

void Interpreter::createDatabase(){
    //std::cout << "Creating database" << std::endl;
    for(auto i: datalog->Schemes){
//        std::cout << i->getName();
        Header* header = new Header(i);
        Relation* relation = new Relation(i->getName(),header);
        database->addRelation(i->getName(),relation);
    };
    for(auto i: datalog->Facts){
        std::vector<std::string> temp;
        for(auto j: i->getParameters()){
            temp.push_back(j->getValue());
        }
        database->addFactsToRelation(i->getName(),Tuple(temp));
    };
    int times = 0;
    std::cout << "Rule Evaluation" << std::endl;
    for(auto i: datalog->Rules){
        Relation* relation = evaluateRules(i);
        //relation->toString(relation->getHeader()->getSize());
        database->fixRelation(relation->getName(),relation);
        times++;
//print head
        std::cout << i->getHeadPredicate()->To_String() + " :- ";
        for(int j = 0; j < i->predicateList.size(); j++){
            if(j == i->predicateList.size()-1){
                std::cout << i->predicateList[j]->To_String() + ".";
            } else {
                std::cout << i->predicateList[j]->To_String() + ",";
            }
        }
        std::cout << std::endl;
        //print inside
        relation->toString(relation->getHeader()->getSize());
    }
    for(auto i: datalog->Rules){
        //print head
        std::cout << i->getHeadPredicate()->To_String() + " :- ";
        for(int j = 0; j < i->predicateList.size(); j++) {
            if (j == i->predicateList.size() - 1) {
                std::cout << i->predicateList[j]->To_String() + ".";
            } else {
                std::cout << i->predicateList[j]->To_String() + ",";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Schemes populated after " + std::to_string(times) + " passes through the Rules" << std::endl << std::endl;
    std::cout << "Query Evaluation" << std::endl;
    for(auto i: datalog->Queries){
       Relation* relation = evaluatePredicate(i);
        if(relation->getSize() > 0) {
            std::cout << i->To_String() + "? Yes(" + std::to_string(relation->getSize()) + ")" << std::endl;
        }
        else
            std::cout << i->To_String() + "? No" << std::endl;
        relation->toString(relation->getHeader()->getSize());
    };
}
Relation* Interpreter::evaluateRules(Rule* rule) {
    std::vector<Relation*> relations;
    for(auto i: rule->predicateList){
        relations.push_back(evaluatePredicate(i));
    }
    checkSize:
    if(relations.size() > 1){
        Relation* temp = join(relations[0],relations[1]);
        relations.erase(relations.begin());
        relations.erase(relations.begin());
        relations.insert(relations.begin(), temp);
        goto checkSize;
    } else {
        relations[0]->setName(rule->getHeadPredicate()->getName());
        std::map<int, std::string> variables = relations[0]->getVariables(relations[0]->getName(),relations[0]->getHeader());
        relations[0]->renameRule(relations[0],variables);
        relations[0]->projectRule(relations[0],variables);
        return relations[0];
    }

}


Relation *Interpreter::join(Relation *r1, Relation *r2) {
    std::pair<std::vector<std::pair<int,int>>,Header*> temp = combineHeaders(r1->getHeader(),r2->getHeader());
    Relation* newRelation = new Relation("", temp.second);
    std::vector<std::pair<int,int>> removed = temp.first;
    std::set<Tuple> tuples;
    std::set<Tuple> newTuples;
    for(auto i : r1->getTuples()){
        for(auto j: r2->getTuples()){
            tuples.insert(combineTuples(i,j));
        }
    }

for(auto i: tuples){
    std::vector<std::string> tuple;
    int itt = 0;
    bool broke = false;
        for(int k =0; k < removed.size();k++){
            continuing:
            if(itt < i.getTuple().size())
            for(int j = itt; j < i.getTuple().size(); j++){
                    if(i.getTuple()[removed[k].first] == i.getTuple()[removed[k].second]){
                        if(j == removed[k].second){
                            j++;
                            itt = j;
                            if(k == removed.size()-1) {
                                goto continuing;
                            }
                            else{
                                break;}
                        }
                        else {
                            tuple.push_back(i.getTuple()[j]);
                        }
                    }else {
                        broke = true;
                        break;
                    }
            }
    }
        if(!broke)
    newTuples.insert(Tuple(tuple));
}
    newRelation->setTuples(newTuples);
    return newRelation;
};
Tuple Interpreter::combineTuples(Tuple i, Tuple j){
    std::vector<std::string> newTuple = i.getTuple();
    for(auto i : j.getTuple()){
        newTuple.push_back(i);
    }
    return Tuple(newTuple);
};
std::pair<std::vector<std::pair<int,int>>,Header*> Interpreter::combineHeaders(Header* h1, Header* h2){
    std::vector<std::string> temp = h1->getHeaderList();
    std::vector<std::string> temp2 = h2->getHeaderList();
    std::vector<std::string> newList = temp;
    std::vector<std::pair<int,int>> removedIndexes;

    for(int i = 0; i < temp2.size(); i++){
        bool found = false;
        for(int j = 0 ; j < temp.size(); j++){
            if(temp2[i] == temp[j]){
                found = true;
                removedIndexes.push_back(std::pair<int,int>(j,i+temp.size()));  // j is the index of the match in the first relation, and i is the second relation
                break;
            }
        }
        if(!found) {
            newList.push_back(temp2[i]);

        }
    }
    Header* newHeader = new Header(newList);
    return std::pair<std::vector<std::pair<int,int>>,Header*>(removedIndexes,newHeader);

}
Relation* Interpreter::evaluatePredicate(Predicate* predicate) {

    Relation* relation = database->getRelation(predicate->getName());
    std::map<int, std::string> constants;
    std::map<int, std::string> variables;
    int j = 0;
    for(auto i : predicate->getParameters()){
        if(i->is_Constant()) {
            constants.insert(std::pair<int,std::string>(j,i->getValue()));
        }
        else{
            variables.insert(std::pair<int,std::string>(j,i->getValue()));
        }
        j++;
    };
    //Variable Select
    std::vector<int> recordedVariables;
    for(unsigned int i = 0; i < predicate->getParameters().size(); i++) {
        for(auto j : variables){
            bool found = false;
          if(predicate->getParameters()[i]->getValue() == j.second){
              relation = relation->select(relation,j.first,i);
              //if j.first is already in recordedVariables don't push, else push
              for(auto k : recordedVariables) {
                  if(j.first == k) {
                      found = true;
                      break;
                  }
              }
              if(!found){
                  recordedVariables.push_back(j.first);
              };
              break;
          }
        }
    }

    //Constant Select
    for(auto i: constants){
        relation = relation->select(relation,i.second,i.first);
    }

    //RENAME

    relation = relation->rename(relation,variables);

    //PROJECT
    relation = relation->project(relation,recordedVariables,variables);



//    if(relation->getSize() > 0) {
//        std::cout << predicate->To_String() + "? Yes(" + std::to_string(relation->getSize()) + ")" << std::endl;
//    }
//    else
//        std::cout << predicate->To_String() + "? No" << std::endl;
//relation->toString(variables.size());
    return relation;
}

#endif //PROJECT1_INTERPRETER_H
