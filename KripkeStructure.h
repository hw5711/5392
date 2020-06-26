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
            State state = State(stateName);
                          //if (!States.Contains(state))
                          if (std::find(std::begin(States), std::end(States), state) == std::end(States))
                          //States.Add(new State(stateName));
                          States.push_back(State(stateName));
            else
                cout << "State {0} is defined more than once " << stateName;
        }

        //load transitions
        for (string transition : transitions)
        {
            //            list<string> parsedTransition = transition.Split(new char[]{':'}).ToList();
            list<string> parsedTransition;
            split_to_list(transition, ":", parsedTransition);

            if (parsedTransition.size() == 0 || parsedTransition.size() != 2)
                cout << "Transition is not in the valid format";

            //          string transitionName = parsedTransition[0];
            //          list<string> parsedFromToStates = parsedTransition[1].Split(new char[]{'-'}).ToList();
            string transitionName = parsedTransition.front();
            parsedTransition.pop_front();
            list<string> parsedFromToStates;
            string parsedTransition_1;
            split_to_list(parsedTransition_1, "-", parsedFromToStates);

            if (parsedFromToStates.size() == 0 || parsedFromToStates.size() != 2)
                cout << "Transition {0} is not in [from state] - [to state] format " << transitionName;

            //            string fromStateName = parsedFromToStates[0];
            //            string toStateName = parsedFromToStates[1];
            string fromStateName = parsedFromToStates.front();
            parsedFromToStates.pop_front();
            string toStateName = parsedFromToStates.front();

            State fromState = FindStateByName(fromStateName);
            State toState = FindStateByName(toStateName);

            if (&fromState == nullptr || &toState == nullptr)
                cout << "Invalid state is detected in transition " << transitionName;

            Transition transitionObj = Transition(transitionName, fromState, toState);
            //            if (!Transitions.Contains(transitionObj))
            if (check_list_contain_transition(Transitions, transitionObj))
                Transitions.push_back(transitionObj);
            else
            {
                cout << "Transitions from state " << fromStateName << " to state " << toStateName << " are defined more than once";
            }
        }

        //load atoms
        for (string stateAtomStructure : stateAtomStructures)
        {
            //            list<string> parsedStateAtomStructure = stateAtomStructure.Split(new char[]{':'}).ToList();
            list<string> parsedStateAtomStructure;
            split_to_list(stateAtomStructure, ":", parsedStateAtomStructure);

            if (parsedStateAtomStructure.size() == 0 || parsedStateAtomStructure.size() != 2)
                cout << stateAtomStructure << " is not a valid state: atoms definition";
            //           string stateName = parsedStateAtomStructure[0].Replace(" ", string.Empty);
            string stateName;
            string parsedStateAtomStructure_0 = parsedStateAtomStructure.front();
            replace_by_empty(parsedStateAtomStructure_0, " ");
            stateName = parsedStateAtomStructure_0;
            parsedStateAtomStructure.pop_front();

            //           string atomNames = parsedStateAtomStructure[1].Trim();
            string atomNames;
            string parsedStateAtomStructure_1 = parsedStateAtomStructure.front();
            atomNames = trim(parsedStateAtomStructure_1);

            //            list<string> parsedAtoms = atomNames.Split(new char[]{' '}).ToList();
            list<string> parsedAtoms;
            split_to_list(atomNames, " ", parsedAtoms);

            list<string> stateAtoms = list<string>();
            for (string atom : parsedAtoms)
            {
                //                if (string.IsNullOrEmpty(atom))
                if (atom == "")
                {
                }
                else if (!check_list_contain_string(stateAtoms, atom))
                    stateAtoms.push_back(atom);
                else
                    cout << "Atom " << atom << " is defined more than once for state " << stateName;
            }

            State stateObj = FindStateByName(stateName);
            if (&stateObj == NULL)
                cout << "State " << stateName << " is not defined";
            stateObj.Atom = stateAtoms;

            //load to list of atoms
            for (string atom : stateAtoms)
            {
                if (!check_list_contain_string(Atoms, atom))
                    Atoms.push_back(atom);
            }
        }
    }

    State FindStateByName(string stateName)
    {
        //        for (State state : States)
        //        {
        //            if (state.StateName == stateName)
        //                return state;
        //        }

        list<State>::iterator it;
        for (it = States.begin(); it != States.end(); it++)
        {
            if (it->StateName == stateName)
                return *it;
        }

        //        return null;
        return State(nullptr); // need to test
    }

    string ToString()
    {
        //        StringBuilder sb = new StringBuilder();
        string sb;

        //        sb.append("STATES");
        //        sb.AppendLine("-----------");
        //        sb.Append(StatesToString());
        //        sb.AppendLine();
        //        sb.AppendLine();
        //        sb.AppendLine("TRANSITIONS");
        //        sb.AppendLine("-------------------");
        //        sb.Append(TransitionsToString());

        sb.append("STATES\r\n");
        sb.append("-----------\r\n");
        sb.append(StatesToString());
        sb.append("\r\n");
        sb.append("\r\n");
        sb.append("\r\n");
        sb.append("TRANSITIONS\r\n");
        sb.append("-------------------\r\n");
        sb.append(TransitionsToString());
        sb.append("\r\n");

        return sb;
    }

    string StatesToString()
    {
        string sb; // = new StringBuilder();

        list<string> stateStrings; // = new list<string>();
        for (State state : States)
        {
            //            string atomNames = string.Join(", ", state.Atoms.ToArray());
            string atomNames;
            list<string>::iterator iter_atoms;
            for (iter_atoms = state.Atom.begin(); iter_atoms != state.Atom.end(); iter_atoms++)
            {
                atomNames.append(*iter_atoms);
            }

            //          stateStrings.push_back(string.Format("{0}({1})", state.StateName, atomNames));
            stateStrings.push_back(state.StateName + "(" + atomNames + ")");
        }

        //        sb.Append(string.Join(", ", stateStrings.ToArray()));
        list<string>::iterator iter_stateString;
        for (iter_stateString = stateStrings.begin(); iter_stateString != stateStrings.end(); iter_stateString++)
        {
            sb.append(*iter_stateString);
        }
        return sb;
    }

    string TransitionsToString()
    {
        string sb;

        list<string> transitionString; // = new list<string>();
        for (Transition transition : Transitions)
        {
            //            transitionString.push_back(string.Format("{0}({1} -> {2})", transition.TransitionName, transition.FromState.StateName, transition.ToState.StateName));
            transitionString.push_back(transition.TransitionName + "(" + transition.FromState.StateName + " -> " + transition.ToState.StateName + ")");
        }

        //        sb.Append(string.Join(", ", transitionString.ToArray()));
        list<string>::iterator iter_transitionString;
        for (iter_transitionString = transitionString.begin(); iter_transitionString != transitionString.end(); iter_transitionString++)
        {
            sb.append(*iter_transitionString);
        }
        return sb;
    }

    //add a replace function for handle string
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

    //add trim functions for trim string
    string ltrim(const string &s)
    {
        size_t start = s.find_first_not_of(" \n\r\t\f\v");
        return (start == string::npos) ? "" : s.substr(start);
    }

    string rtrim(const string &s)
    {
        size_t end = s.find_last_not_of(" \n\r\t\f\v");
        return (end == string::npos) ? "" : s.substr(0, end + 1);
    }

    string trim(const string &s)
    {
        return rtrim(ltrim(s));
    }

    //add function to check contain situation in a list for string
    bool check_list_contain_string(list<string> &dest, string src)
    {
        list<string>::iterator iter_check_list;
        for (iter_check_list = dest.begin(); iter_check_list != dest.end(); iter_check_list++)
        {
            if (*iter_check_list == src)
                return true;
        }
        return false;
    }

    //add function to check contain situation in a list for Transition
    bool check_list_contain_transition(list<Transition> &dest, Transition src)
    {
        list<Transition>::iterator iter_check_list;
        for (iter_check_list = dest.begin(); iter_check_list != dest.end(); iter_check_list++)
        {
            if (iter_check_list->Equals(src))
                return true;
        }
        return false;
    }
};

#endif //SATPROJECT_KRIPKESTRUCTURE_H
