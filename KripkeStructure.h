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

    KripkeStructure(string kripkeStructureDefinition) : KripkeStructure()
    {

        //        list<string> parsedStructure = kripkeStructureDefinition
        //                .Replace(Environment.NewLine, string.Empty) //replace "\r\n"  by ""
        //                .Split(new char[]{';'}) //split each ; into elements, use manually added split function
        //                .ToList(); // store elements into a list

        list<string> parsedStructure;
        replace_by_empty(kripkeStructureDefinition, "\r\n");
        split_to_list(kripkeStructureDefinition, ";", parsedStructure);

        if (parsedStructure.size() == 0 || parsedStructure.size() != 4)
            //throw new FormatException("Input file does not contain appropriate segments to construct kripke structure");
            cout << "Input file does not contain appropriate segments to construct kripke structure." << endl;
        //        list<string> stateNames = parsedStructure[0]
        //                .Replace(" ", string.Empty)
        //                .Split(new char[]{','})
        //                .ToList();
        list<string> stateNames;
        string src_0 = parsedStructure.front();
        replace_by_empty(src_0, "\r\n");
        split_to_list(src_0, ",", stateNames);
        parsedStructure.pop_front();

        //        list<string> transitions = parsedStructure[1]
        //                .Replace(" ", string.Empty)
        //                .Split(new char[]{','})
        //                .ToList();
        list<string> transitions;
        string src_1 = parsedStructure.front();
        replace_by_empty(src_1, "\r\n");
        split_to_list(src_1, ",", transitions);
        parsedStructure.pop_front();

        //        list<string> stateAtomStructures = parsedStructure[2]
        //                .Split(new char[]{','})
        //                .ToList();
        list<string> stateAtomStructures;
        string src_2 = parsedStructure.front();
        split_to_list(src_2, ",", stateAtomStructures);

        //load states
        //foreach (string stateName in stateNames)
        for (auto stateName : stateNames)
        {
            //State state = new State(stateName);
            State state;
            //if (!States.Contains(state)) 
            if(std::find(std::begin(States), std::end(my_list), state) == std::end(state))
                //States.Add(new State(stateName));
                States.push_back(State(stateName));
            else
                throw new FormatException(string.Format("State {0} is defined more than once", stateName));
        }

        //load transitions
        for (string transition : transitions)
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
        for (string stateAtomStructure : stateAtomStructures)
        {
            list<string> parsedStateAtomStructure = stateAtomStructure.Split(new char[]{':'}).ToList();

            if (parsedStateAtomStructure == null || parsedStateAtomStructure.Count != 2)
                throw new FormatException(string.Format("{0} is not a valid state: atoms definition", stateAtomStructure));
            string stateName = parsedStateAtomStructure[0].Replace(" ", string.Empty);
            string atomNames = parsedStateAtomStructure[1].Trim();
            list<string> parsedAtoms = atomNames.Split(new char[]{' '}).ToList();

            list<string> stateAtoms = new list<string>();
            for (string atom : parsedAtoms)
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
            for (string atom : stateAtoms)
            {
                if (!Atoms.Contains(atom))
                    Atoms.Add(atom);
            }
        }
    }

    State FindStateByName(string stateName)
    {
        for (State state : States)
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
        StringBuilder sb; // = new StringBuilder();

        list<string> stateStrings; // = new list<string>();
        for (State state : States)
        {
            string atomNames = string.Join(", ", state.Atoms.ToArray());
            stateStrings.push_back(string.Format("{0}({1})", state.StateName, atomNames));
        }

        sb.Append(string.Join(", ", stateStrings.ToArray()));
        return sb.ToString();
    }

    string TransitionsToString()
    {
        StringBuilder sb = new StringBuilder();

        list<string> transitionString; // = new list<string>();
        for (Transition transition : Transitions)
        {
            transitionString.push_back(string.Format("{0}({1} -> {2})", transition.TransitionName, transition.FromState.StateName, transition.ToState.StateName));
        }

        sb.Append(string.Join(", ", transitionString.ToArray()));
        return sb.ToString();
    }

    void replace_by_empty(string &src, string before)
    {
        while (src.find(before) != string::npos)
        {
            src.erase(src.find(before), 2);
        }
    }

    //add a split function for handle string
    void split_to_list(const string &src, const string &separator, list<string> &dest)
    {
        string str = src;
        string substring;
        string::size_type start = 0, index;
        dest.clear();
        index = str.find_first_of(separator, start);
        do
        {
            if (index != string::npos)
            {
                substring = str.substr(start, index - start);
                dest.push_back(substring);
                start = index + separator.size();
                index = str.find(separator, start);
                if (start == string::npos)
                    break;
            }
        } while (index != string::npos);

        //the last part
        substring = str.substr(start);
        dest.push_back(substring);
    }
};

#endif //SATPROJECT_KRIPKESTRUCTURE_H
