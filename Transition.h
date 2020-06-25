//
// Created by Huan wu on 6/25/20.
//

#ifndef SATPROJECT_TRANSITION_H
#define SATPROJECT_TRANSITION_H

#include "CtlFormula.h"
#include "KripkeStructure.h"
#include "State.h"

#include <string>

using namespace std;

class Transition
{
public:
    string TransitionName;
    State FromState;
    State ToState;

    Transition() {}

    Transition(State fromState, State toState)
    {
        TransitionName = "";
        FromState = fromState;
        ToState = toState;
    }

    Transition(string transitionName, State fromState, State toState)
    {
        TransitionName = transitionName;
        FromState = fromState;
        ToState = toState;
    }

    bool Equals(Transition other)
    {
        if (this.FromState.Equals(other.FromState) && this.ToState.Equals(other.ToState))
            return true;

        return false;
    }

};

#endif //SATPROJECT_TRANSITION_H
