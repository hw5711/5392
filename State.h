//
// Created by Huan wu on 6/25/20.
//

#ifndef SATPROJECT_STATE_H
#define SATPROJECT_STATE_H

//#include "CtlFormula.h"
//#include "KripkeStructure.h"
//#include "Transition.h"

#include <list>
#include <string>

using namespace std;

class State
{
public:
    string StateName;
    list<string> Atom;

    State()
    {
        //this.Atom = new list<string>;
    }

    State(string stateName)
    {
        StateName = stateName;
    }

    bool Equals(State other)
    {
        return StateName == other.StateName;
    }
};

#endif //SATPROJECT_STATE_H
