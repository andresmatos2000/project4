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
    for(auto i: datalog->Queries){
        evaluatePredicate(i);
    };
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
        };

    }

    //Constant Select
    for(auto i: constants){
        relation = relation->select(relation,i.second,i.first);
    }

    //RENAME

    relation = relation->rename(relation,variables);

    //PROJECT
    relation = relation->project(relation,recordedVariables,variables);



    if(relation->getSize() > 0) {
        std::cout << predicate->To_String() + "? Yes(" + std::to_string(relation->getSize()) + ")" << std::endl;
    }
    else
        std::cout << predicate->To_String() + "? No" << std::endl;
relation->toString(variables.size());
    return relation;
};
#endif //PROJECT1_INTERPRETER_H
