//
// Created by Huan wu on 6/25/20.
//test!!!!

#ifndef SATPROJECT_KRIPKESTRUCTURE_H
#define SATPROJECT_KRIPKESTRUCTURE_H

#include "CtlFormula.h"
#include "State.h"
#include "Transition.h"

#include <list>
#include <string>

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

    KripkeStructure(string kripkeStructureDefinition) : KripkeStructure()
    {

        list<string> parsedStructure = kripkeStructureDefinition
                                           .Replace(Environment.NewLine, string.Empty)
                                           .Split(new char[]{';'})
                                           .ToList();

        if (parsedStructure == null || parsedStructure.Count != 4)
            //throw new FormatException("Input file does not contain appropriate segments to construct kripke structure");
            cout << "Input file does not contain appropriate segments to construct kripke structure." << endl;
        list<string> stateNames = parsedStructure[0]
                                      .Replace(" ", string.Empty)
                                      .Split(new char[]{','})
                                      .ToList();
        list<string> transitions = parsedStructure[1]
                                       .Replace(" ", string.Empty)
                                       .Split(new char[]{','})
                                       .ToList();
        list<string> stateAtomStructures = parsedStructure[2]
                                               .Split(new char[]{','})
                                               .ToList();

        //load states
        foreach (string stateName in stateNames)
        {
            State state = new State(stateName);
            if (!States.Contains(state))
                States.Add(new State(stateName));
            else
                throw new FormatException(string.Format("State {0} is defined more than once", stateName));
        }

        //load transitions
        foreach (string transition in transitions)
        {
            list<string> parsedTransition = transition.Split(new char[]{':'}).ToList();

            if (parsedTransition == null || parsedTransition.Count != 2)
                throw new FormatException("Transition is not in the valid format");

            string transitionName = parsedTransition[0];
            list<string> parsedFromToStates = parsedTransition[1].Split(new char[]{'-'}).ToList();

            if (parsedFromToStates == null || parsedFromToStates.Count != 2)
                throw new FormatException(string.Format("Transition {0} is not in [from state] - [to state] format", transitionName));

            string fromStateName = parsedFromToStates[0];
            string toStateName = parsedFromToStates[1];
            State fromState = FindStateByName(fromStateName);
            State toState = FindStateByName(toStateName);

            if (fromState == null || toState == null)
                throw new FormatException(string.Format("Invalid state is detected in transition {0}", transitionName));

            Transition transitionObj = new Transition(transitionName, fromState, toState);
            if (!Transitions.Contains(transitionObj))
                Transitions.Add(transitionObj);
            else
            {
                throw new FormatException(string.Format("Transitions from state {0} to state {1} are defined more than once", fromStateName, toStateName));
            }
        }

        //load atoms
        foreach (string stateAtomStructure in stateAtomStructures)
        {
            list<string> parsedStateAtomStructure = stateAtomStructure.Split(new char[]{':'}).ToList();

            if (parsedStateAtomStructure == null || parsedStateAtomStructure.Count != 2)
                throw new FormatException(string.Format("{0} is not a valid state: atoms definition", stateAtomStructure));
            string stateName = parsedStateAtomStructure[0].Replace(" ", string.Empty);
            string atomNames = parsedStateAtomStructure[1].Trim();
            list<string> parsedAtoms = atomNames.Split(new char[]{' '}).ToList();

            list<string> stateAtoms = new list<string>();
            foreach (string atom in parsedAtoms)
            {
                if (string.IsNullOrEmpty(atom))
                {
                }
                else if (!stateAtoms.Contains(atom))
                    stateAtoms.Add(atom);
                else
                    throw new FormatException(string.Format("Atom {0} is defined more than once for state {1}", atom, stateName));
            }

            State stateObj = FindStateByName(stateName);
            if (stateObj == null)
                throw new FormatException(string.Format("State {0} is not defined", stateName));
            stateObj.Atoms = stateAtoms;

            //load to list of atoms
            foreach (string atom in stateAtoms)
            {
                if (!Atoms.Contains(atom))
                    Atoms.Add(atom);
            }
        }
    }

    State FindStateByName(string stateName)
    {
        foreach (State state in States)
        {
            if (state.StateName.Equals(stateName))
                return state;
        }

        return null;
    }

    string ToString()
    {
        StringBuilder sb = new StringBuilder();

        sb.AppendLine("STATES");
        sb.AppendLine("-----------");
        sb.Append(StatesToString());
        sb.AppendLine();
        sb.AppendLine();
        sb.AppendLine("TRANSITIONS");
        sb.AppendLine("-------------------");
        sb.Append(TransitionsToString());

        return sb.ToString();
    }

    string StatesToString()
    {
        StringBuilder sb = new StringBuilder();

        list<string> stateStrings = new list<string>();
        foreach (State state in States)
        {
            string atomNames = string.Join(", ", state.Atoms.ToArray());
            stateStrings.Add(string.Format("{0}({1})", state.StateName, atomNames));
        }

        sb.Append(string.Join(", ", stateStrings.ToArray()));
        return sb.ToString();
    }

    string TransitionsToString()
    {
        StringBuilder sb = new StringBuilder();

        list<string> transitionString = new list<string>();
        foreach (Transition transition in Transitions)
        {
            transitionString.Add(string.Format("{0}({1} -> {2})", transition.TransitionName, transition.FromState.StateName, transition.ToState.StateName));
        }

        sb.Append(string.Join(", ", transitionString.ToArray()));
        return sb.ToString();
    }
};

#endif //SATPROJECT_KRIPKESTRUCTURE_H
