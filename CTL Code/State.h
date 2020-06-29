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
        Atom = list<string>();
    }

    State(string stateName) : State()
    {
        StateName = stateName;
    }

    bool Equals(const State other) const
    {
        return StateName == other.StateName;
    }

	friend bool operator==(const State& lhs, const State& rhs)
	{
		return lhs.Equals(rhs);
	}
};

#endif //SATPROJECT_STATE_H
