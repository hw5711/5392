//
// Created by Huan wu on 6/25/20.

#ifndef SATPROJECT_KRIPKESTRUCTURE_H
#define SATPROJECT_KRIPKESTRUCTURE_H

#include "State.h"
#include "Transition.h"

#include <list>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

class KripkeStructure
{
public:
    list<Transition> Transitions; // not add getter and setter
    list<State> States;           // not add getter and setter
    list<string> Atoms;           // not add getter and setter

    KripkeStructure()
    {
        Transitions = list<Transition>();
        States = list<State>();
        Atoms = list<string>();
    }

    KripkeStructure(string kripkeStructureDefinition);// : KripkeStructure();

    void FindStateByName(const string stateName, State& state);

    string ToString();
    string StatesToString();
    string TransitionsToString();

    //add a replace function for handle string
    void replace_by_empty(string& src, string before);

    //add a split function for handle string
    void split_to_list(const string& src, const string& separator, list<string>& dest);

    string ltrim(const string& s);
    string rtrim(const string& s);
    string trim(const string& s);

    //add function to check contain situation in a list for string
    bool check_list_contain_string(list<string>& dest, string src);

    //add function to check contain situation in a list for Transition
    bool check_list_contain_transition(list<Transition>& dest, Transition src);

    //add function to check contain situation in a list for state
    bool check_list_contain_state(list<State>& dest, State src);
};

#endif //SATPROJECT_KRIPKESTRUCTURE_H
