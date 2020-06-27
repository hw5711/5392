//
// Created by Huan wu on 6/25/20. !!!
//

#ifndef SATPROJECT_CTLFORMULA_H
#define SATPROJECT_CTLFORMULA_H

#include "KripkeStructure.h"
#include "State.h"
#include "Transition.h"

#include <string>
#include <list>
#include <map>
#include <algorithm>

using namespace std;

enum TypeSAT
{
    UnKnown,
    AllTrue,
    AllFalse,
    Atomic,
    Not,
    And,
    Or,
    Implies,
    AX,
    EX,
    AU,
    EU,
    EF,
    EG,
    AF,
    AG
};

class CtlFormula
{
public:
    KripkeStructure _kripke;
    State _state;
    string _expression;
    // modify
    map<string, string> _convertionString;

    //constrctor
    CtlFormula(string expression, State state, KripkeStructure& kripke);
    void ConvertToSystemFormula(string& expression);
    bool IsSatisfy();

private:
    TypeSAT GetTypeSAT(string expression, string& leftExpression, string& rightExpression);
    // Determine states that satisfy given expression
    list<State> SAT(string expression);
    //Handling EX
    list<State> SAT_EX(string expression);

    // Handling EU
    list<State> SAT_EU(string leftExpression, string rightExpression);
    // Handling AF
    list<State> SAT_AF(string expression);
    // PreE
    list<State> PreE(list<State>& y);

    // PreA
    list<State> PreA(list<State>& y);

    // Determine whether the list contain same set of states
    bool ArelistStatesEqual(list<State>& list1, list<State>& list2);

    // Determine whether this is an atom
    bool IsAtomic(string expression);

    // Determine whether given expression contains binary operation for the next checking
    bool IsBinaryOp(string expression, string sym, string& leftExpression, string& rightExpression);

    // Removing extra brackets
    string RemoveExtraBrackets(string expression);

    //add function to check contain situation in a list for state
    bool check_list_contain_state(list<State>& dest, State src);

    //add remove an element function to list<State>
    void remove_from_list(list<State>& dest, State src);

    //add function to check contain situation in a list for Transition
    bool check_list_contain_transition(list<Transition>& dest, Transition src);

    //add function to check contain situation in a list for string
    bool check_list_contain_string(list<string>& dest, string src);
};

#endif //SATPROJECT_CTLFORMULA_H
