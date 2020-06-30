//
// Created by Huan wu on 6/25/20.
//

#include "KripkeStructure.h"


KripkeStructure::KripkeStructure(string kripkeStructureDefinition) : KripkeStructure()
{
    list<string> parsedStructure;
    replace_by_empty(kripkeStructureDefinition, "\r\n");
    split_to_list(kripkeStructureDefinition, ";", parsedStructure);

	if (parsedStructure.size() == 0 || parsedStructure.size() != 4)
	{
		cout << "\nInput file does not contain appropriate segments to construct kripke structure." << endl;
		exit(0);
	}

    list<string> stateNames;
    string src_0 = parsedStructure.front();
    replace_by_empty(src_0, "\r\n");
    split_to_list(src_0, ",", stateNames);
    parsedStructure.pop_front();

    list<string> transitions;
    string src_1 = parsedStructure.front();
    replace_by_empty(src_1, "\r\n");
    split_to_list(src_1, ",", transitions);
    parsedStructure.pop_front();

    list<string> stateAtomStructures;
    string src_2 = parsedStructure.front();
    split_to_list(src_2, ",", stateAtomStructures);

    list<string>::iterator iter_state_names;
    for (iter_state_names = stateNames.begin(); iter_state_names != stateNames.end(); iter_state_names++)
    {
        State state = State(*iter_state_names);
        if (!check_list_contain_state(States, state))
        {
            States.push_back(State(*iter_state_names));
        }
		else
		{
			cout << "\nState " << *iter_state_names << "is defined more than once.";
			exit(0);
		}
    }

    //load transitions
    for (string transition : transitions)
    {
        //            list<string> parsedTransition = transition.Split(new char[]{':'}).ToList();
        list<string> parsedTransition;
        split_to_list(transition, ":", parsedTransition);

		if (parsedTransition.size() == 0 || parsedTransition.size() != 2)
		{
			cout << "\nTransition is not in the valid format";
			exit(0);
		}

        string transitionName = parsedTransition.front();
        parsedTransition.pop_front();
        list<string> parsedFromToStates;
        string parsedTransition_1 = parsedTransition.front();
        split_to_list(parsedTransition_1, "-", parsedFromToStates);

		if (parsedFromToStates.size() == 0 || parsedFromToStates.size() != 2)
		{
			cout << "\nTransition " << transitionName << " is not in[from state] - [to state] format ";
			exit(0);
		}
			

        string fromStateName = parsedFromToStates.front();
        parsedFromToStates.pop_front();
        string toStateName = parsedFromToStates.front();

		list<State>::iterator iter_fromState, iter_toState;
        iter_fromState = FindStateByName(fromStateName);
		iter_toState = FindStateByName(toStateName);

		if (iter_fromState == States.end() || iter_toState == States.end())
		{
			cout << "\nInvalid state is detected in transition " << transitionName;
			exit(0);
		}
			
		
// 		State fromState = *iter_fromState;
// 		State toState = *iter_toState;

        Transition transitionObj = Transition(transitionName, *iter_fromState, *iter_toState);

        if (!check_list_contain_transition(Transitions, transitionObj))
            Transitions.push_back(transitionObj);
        else
        {
            cout << "\nTransitions from state " << fromStateName << " to state " << toStateName << " are defined more than once";
			exit(0);
        }
    }

    //load atoms
    for (string stateAtomStructure : stateAtomStructures)
    {
        list<string> parsedStateAtomStructure;
        split_to_list(stateAtomStructure, ":", parsedStateAtomStructure);

		if (parsedStateAtomStructure.size() == 0 || parsedStateAtomStructure.size() != 2)
		{
			cout << "\n" << stateAtomStructure << " is not a valid state: atoms definition";
			exit(0);
		}
            

        string stateName;
        string parsedStateAtomStructure_0 = parsedStateAtomStructure.front();
        replace_by_empty(parsedStateAtomStructure_0, " ");
        stateName = parsedStateAtomStructure_0;
        parsedStateAtomStructure.pop_front();

        string atomNames;
        string parsedStateAtomStructure_1 = parsedStateAtomStructure.front();
        atomNames = trim(parsedStateAtomStructure_1);

        list<string> parsedAtoms;

        for (char atom : atomNames)
            parsedAtoms.push_back(string(1, atom));

        list<string> stateAtoms = list<string>();
        for (string atom : parsedAtoms)
        {
            if (atom == "")
            {
            }
            else if (!check_list_contain_string(stateAtoms, atom))
                stateAtoms.push_back(atom);
			else
			{
				cout << "\nAtom " << atom << " is defined more than once for state " << stateName;
				exit(0);
			}
				
        }


		list<State>::iterator iter_stateObj = FindStateByName(stateName);

		if (iter_stateObj == States.end())
		{
			cout << "\n" << "State " << stateName << " is not defined";
			exit(0);
		}	
		iter_stateObj->Atom = stateAtoms;

        for (string atom : stateAtoms)
        {
            if (!check_list_contain_string(Atoms, atom))
                Atoms.push_back(atom);
        }
    }
}

list<State>::iterator KripkeStructure::FindStateByName(const string stateName)
{
    list<State>::iterator it;
    for (it = States.begin(); it != States.end(); it++)
    {
		if (it->StateName == stateName)
			return it;
    }
	return States.end();
}

string KripkeStructure::ToString()
{
    string sb;

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

string KripkeStructure::StatesToString()
{
    string sb; // = new StringBuilder();

    list<string> stateStrings; // = new list<string>();
    for (State state : States)
    {
        string atomNames;
        list<string>::iterator iter_atoms;
        for (iter_atoms = state.Atom.begin(); iter_atoms != state.Atom.end(); iter_atoms++)
        {
            atomNames.append(*iter_atoms);
        }

        stateStrings.push_back(state.StateName + "(" + atomNames + ")");
    }

    list<string>::iterator iter_stateString;
    for (iter_stateString = stateStrings.begin(); iter_stateString != stateStrings.end(); iter_stateString++)
    {
        sb.append(*iter_stateString);
    }
    return sb;
}

string KripkeStructure::TransitionsToString()
{
    string sb;

    list<string> transitionString; // = new list<string>();
    for (Transition transition : Transitions)
    {
        transitionString.push_back(transition.TransitionName + "(" + transition.FromState.StateName + " -> " + transition.ToState.StateName + ")");
    }

    list<string>::iterator iter_transitionString;
    for (iter_transitionString = transitionString.begin(); iter_transitionString != transitionString.end(); iter_transitionString++)
    {
        sb.append(*iter_transitionString);
    }
    return sb;
}

void KripkeStructure::replace_by_empty(string& src, string before)
{
    while (src.find(before) != string::npos)
    {
        src.erase(src.find(before), 2);
    }
}

void KripkeStructure::split_to_list(const string& src, const string& separator, list<string>& dest)
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
string KripkeStructure::ltrim(const string& s)
{
    size_t start = s.find_first_not_of(" \n\r\t\f\v");
    return (start == string::npos) ? "" : s.substr(start);
}

string KripkeStructure::rtrim(const string& s)
{
    size_t end = s.find_last_not_of(" \n\r\t\f\v");
    return (end == string::npos) ? "" : s.substr(0, end + 1);
}

string KripkeStructure::trim(const string& s)
{
    return rtrim(ltrim(s));
}

bool KripkeStructure::check_list_contain_string(list<string>& dest, string src)
{
	return std::find(dest.begin(), dest.end(), src) != dest.end() ? true : false;
}

bool KripkeStructure::check_list_contain_transition(list<Transition>& dest, Transition src)
{
	return std::find(dest.begin(), dest.end(), src) != dest.end() ? true : false;
}

bool KripkeStructure::check_list_contain_state(list<State>& dest, State src)
{
	return std::find(dest.begin(), dest.end(), src) != dest.end() ? true : false;
}