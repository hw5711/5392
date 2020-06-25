//
// Created by Huan wu on 6/25/20.
//

#ifndef SATPROJECT_CTLFORMULAR_H
#define SATPROJECT_CTLFORMULAR_H

#include "KripkeStructure.h"
#include "State.h"
#include "Transition.h"

#include <string>

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

class CtlFormular
{
public:
    //constrctor
    CtlFormular(string expression, State state, KripkeStructure kripke)
    {
    }
};

#endif //SATPROJECT_CTLFORMULAR_H
