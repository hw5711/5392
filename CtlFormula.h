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
    CtlFormula(string expression, State state, KripkeStructure kripke)
    {
//         _convertionString = new Dictionary<string, string>();
//         _convertionString.Add("and", "&");
//         _convertionString.Add("or", "|");
//         _convertionString.Add("->", ">");
//         _convertionString.Add("not", "!");

        // modify: consert dictionary to map
        _convertionString["and"] = "&";
        _convertionString["or"] = "|";
        _convertionString["->"] = ">";
        _convertionString["not"] = "!";

        _kripke = kripke;
        _state = state;
        _expression = ConvertToSystemFormula(expression); //parse the input Ctl check formula
    }
    string ConvertToSystemFormula(string expression)
    {
//         foreach (KeyValuePair<string, string> entry in _convertionString)
//         {
//             expression = expression.Replace(entry.Key.ToString(), entry.Value.ToString());
//         }

        // modify
        for (auto entry : _convertionString)
        {
            size_t pos = expression.find(entry.first);
            while (pos != string::npos)
            {
                expression.replace(pos, entry.first.length(), entry.second);
                pos = expression.find(entry.first, pos + entry.first.length() + 1);
            }
        }

        return expression;
    }
    bool IsSatisfy()
    {
        list<State> states = SAT(_expression); //get all states from the input ctl formula
//        if (states.Contains(_state))
        if(check_list_contain_state(states, _state)) //check all contents inside of state
            return true;
        return false;
    }

private:
    TypeSAT GetTypeSAT(string expression, string &leftExpression, string &rightExpression)
    {
        //remove extra brackets
        expression = RemoveExtraBrackets(expression);

        //look for binary implies
//        if (expression.Contains(">"))
        if (expression.find(">") != expression.npos)
        {
            if (IsBinaryOp(expression, ">", leftExpression, rightExpression))
            return Implies;
        }
        //look for binary and
//        if (expression.Contains("&"))
        if (expression.find("&") != expression.npos)
        {
            if (IsBinaryOp(expression, "&", leftExpression, rightExpression))
            return And;
        }
        //look for binary or
//        if (expression.Contains("|"))
        if (expression.find("|") != expression.npos)
        {
            if (IsBinaryOp(expression, "|", leftExpression, rightExpression))
            return Or;
        }
        //look for binary AU
//        if (expression.StartsWith("A("))
        if(expression.find("A(") == 0)
        {
            string strippedExpression = expression.Substring(2, expression.Length - 3);
            if (IsBinaryOp(strippedExpression, "U", leftExpression, rightExpression))
            return AU;
        }
        //look for binary EU
//        if (expression.StartsWith("E("))
        if(expression.find("E(") == 0)
        {
            string strippedExpression = expression.Substring(2, expression.Length - 3);
            if (IsBinaryOp(strippedExpression, "U", leftExpression, rightExpression))
            return EU;
        }

        //look for unary T, F, !, AX, EX, AG, EG, AF, EF, atomic
        if (expression == "T")
        {
            leftExpression = expression;
            return AllTrue;
        }
        if (expression == "F")
        {
            leftExpression = expression;
            return AllFalse;
        }
        if (IsAtomic(expression))
        {
            leftExpression = expression;
            return Atomic;
        }
//        if (expression.StartsWith("!"))
        if(expression.find("!") == 0)
        {
            leftExpression = expression.Substring(1, expression.Length - 1);
            return Not;
        }
//        if (expression.StartsWith("AX"))
        if(expression.find("AX") == 0)
        {
            leftExpression = expression.Substring(2, expression.Length - 2);
            return AX;
        }
//        if (expression.StartsWith("EX"))
        if(expression.find("EX") == 0)
        {
            leftExpression = expression.Substring(2, expression.Length - 2);
            return EX;
        }
//        if (expression.StartsWith("EF"))
        if(expression.find("EF") == 0)
        {
            leftExpression = expression.Substring(2, expression.Length - 2);
            return EF;
        }
//        if (expression.StartsWith("EG"))
        if(expression.find("EG") == 0)
        {
            leftExpression = expression.Substring(2, expression.Length - 2);
            return EG;
        }
//        if (expression.StartsWith("AF"))
        if(expression.find("AF") == 0)

        {
            leftExpression = expression.Substring(2, expression.Length - 2);
            return AF;
        }
//        if (expression.StartsWith("AG"))
        if(expression.find("AG") == 0)
        {
            leftExpression = expression.Substring(2, expression.Length - 2);
            return AG;
        }

        return Unknown;
    }

    // Determine states that satisfy given expression
    list<State> SAT(string expression)
    {
//        System.Diagnostics.Debug.WriteLine(string.Format("Original Expression: {0}", expression));
        cout << "Original Expression: " << expression;
        list<State> states = list<State>();

        //from Logic in Computer Science, page 227
        string leftExpression = "", rightExpression = "";

        //TypeSAT typeSAT = DetermineTypeSAT(expression, ref leftExpression, ref rightExpression);
        TypeSAT typeSAT = GetTypeSAT(expression, leftExpression, rightExpression);

//        System.Diagnostics.Debug.WriteLine(string.Format("Type SAT: {0}", ToString()));
//        System.Diagnostics.Debug.WriteLine(string.Format("Left Expression: {0}", leftExpression));
//        System.Diagnostics.Debug.WriteLine(string.Format("Right Expression: {0}", rightExpression));
//        System.Diagnostics.Debug.WriteLine("------------------------------------");
        cout << "Type SAT: " << _kripke.ToString() << endl;
        cout << "Left Expression: " << leftExpression << endl;
        cout << "Right Expression: " << rightExpression << endl;

        switch (typeSAT)
        {
            case AllTrue:
                //all states
                states.AddRange(_kripke.States.ToArray());
                break;
            case AllFalse:
                //empty
                break;
            case Atomic:
                foreach (State state in _kripke.States)
                {
                    if (state.Atoms.Contains(leftExpression))
                        states.Add(state);
                }
                break;
            case Not:
                //S − SAT (φ1)
                states.AddRange(_kripke.States.ToArray());
                list<State> f1States = SAT(leftExpression);

                foreach (State state in f1States)
                {
                    if (states.Contains(state))
                        states.Remove(state);
                }
                break;
            case And:
                //SAT (φ1) ∩ SAT (φ2)
                list<State> andF1States = SAT(leftExpression);
                list<State> andF2States = SAT(rightExpression);

                foreach (State state in andF1States)
                {
                    if (andF2States.Contains(state))
                        states.Add(state);
                }
                break;
            case Or:
                //SAT (φ1) ∪ SAT (φ2)
                list<State> orF1States = SAT(leftExpression);
                list<State> orF2States = SAT(rightExpression);

                states = orF1States;
                foreach (State state in orF2States)
                {
                    if (!states.Contains(state))
                        states.Add(state);
                }
                break;
            case Implies:
                //SAT (¬φ1 ∨ φ2)
                //TODO: reevaluate impliesFormula
//                string impliesFormula = string.Concat("!", leftExpression, "|", rightExpression);
                string impliesFormula = "!" + leftExpression +  "|" + rightExpression;
                states = SAT(impliesFormula);
                break;
            case AX:
                //SAT (¬EX¬φ1)
                //TODO: reevaluate axFormula
//                string axFormula = string.Concat("!", "EX", "!", leftExpression);
                string axFormula ="!EX!" + leftExpression;
                states = SAT(axFormula);

                //check if states actually has link to next state
                list<State> tempStates = list<State>();
                foreach (State sourceState in states)
                {
                    foreach (Transition transition in _kripke.Transitions)
                    {
                        if (sourceState.Equals(transition.FromState))
                        {
                            tempStates.Add(sourceState);
                            break;
                        }
                    }
                }
                states = tempStates;
                break;
            case EX:
                //SATEX(φ1)
                //TODO: reevaluate exFormula
                string exFormula = leftExpression;
                states = SAT_EX(exFormula);
                break;
            case AU:
                //A[φ1 U φ2]
                //SAT(¬(E[¬φ2 U (¬φ1 ∧¬φ2)] ∨ EG¬φ2))
                //TODO: reevaluate auFormulaBuilder
//                StringBuilder auFormulaBuilder = new StringBuilder();
                string auFormulaBuilder;
                auFormulaBuilder.append("!(E(!");
                auFormulaBuilder.append(rightExpression);
                auFormulaBuilder.append("U(!");
                auFormulaBuilder.append(leftExpression);
                auFormulaBuilder.append("&!");
                auFormulaBuilder.append(rightExpression);
                auFormulaBuilder.append("))|(EG!");
                auFormulaBuilder.append(rightExpression);
                auFormulaBuilder.append("))");
                states = SAT(auFormulaBuilder);
                break;
            case EU:
                //SATEU(φ1, φ2)
                //TODO: reevaluate euFormula
                states = SAT_EU(leftExpression, rightExpression);
                break;
            case EF:
                //SAT (E( U φ1))
                //TODO: reevaluate efFormula
//                string efFormula = string.Concat("E(TU", leftExpression, ")");
                string efFormula = "E(TU" +  leftExpression + ")";
                states = SAT(efFormula);
                break;
            case EG:
                //SAT(¬AF¬φ1)
                //TODO: reevaulate egFormula
//                string egFormula = string.Concat("!AF!", leftExpression);
                string egFormula = "!AF!" + leftExpression;
                states = SAT(egFormula);
                break;
            case AF:
                //SATAF (φ1)
                //TODO: reevaluate afFormula
                string afFormula = leftExpression;
                states = SAT_AF(afFormula);
                break;
            case AG:
                //SAT (¬EF¬φ1)
                //TODO: reevaluate agFormula
//                string agFormula = string.Concat("!EF!", leftExpression);
                string agFormula = "!EF!" + leftExpression;
                states = SAT(agFormula);
                break;
            case Unknown:
                cout << "Invalid CTL expression";
        }

        return states;
    }

    //Handling EX
    list<State> SAT_EX(string expression)
    {
        //X := SAT (φ);
        //Y := pre∃(X);
        //return Y
        list<State> x = list<State>();
        list<State> y = list<State>();
        x = SAT(expression);
        y = PreE(x);
        return y;
    }

    // Handling EU
    list<State> SAT_EU(string leftExpression, string rightExpression)
    {
        list<State> w = list<State>();
        list<State> x = list<State>();
        list<State> y = list<State>();

        w = SAT(leftExpression);
        x.AddRange(_kripke.States.ToArray());
        y = SAT(rightExpression);

        while (!ArelistStatesEqual(x, y))
        {
            x = y;
            list<State> newY = list<State>();
            list<State> preEStates = PreE(y);

            newY.AddRange(y.ToArray());
            list<State> wAndPreE = list<State>();
            foreach (State state in w)
            {
                if (preEStates.Contains(state))
                    wAndPreE.Add(state);
            }

            foreach (State state in wAndPreE)
            {
                if (!newY.Contains(state))
                    newY.Add(state);
            }
            y = newY;
        }

        return y;
    }

    // Handling AF
    list<State> SAT_AF(string expression)
    {
        list<State> x = list<State>();
        x.AddRange(_kripke.States.ToArray());
        list<State> y = list<State>();
        y = SAT(expression);

        while (!ArelistStatesEqual(x, y))
        {
            x = y;
            list<State> newY = list<State>();
            list<State> preAStates = PreA(y);
            newY.AddRange(y.ToArray());

            foreach (State state in preAStates)
            {
                if (!newY.Contains(state))
                    newY.Add(state);
            }

            y = newY;
        }

        return y;
    }

    // PreE
    list<State> PreE(list<State> y)
    {
        //{s âˆˆ S | exists s, (s â†’ s and s âˆˆ Y )}
        list<State> states;// = new list<State>();

        list<Transition> transitions;// = new list<Transition>();
        for (State sourceState : _kripke.States)
        {
            for (State destState : y)
            {
//                Transition myTransition;// = new Transition(sourceState, destState);
                Transition myTransition =  Transition(sourceState, destState);
                for( x : _kripke.Transitions)
                    if ( x.find(myTransition) != string::npos)
                    {
//                        if (!states.Contains(sourceState))
                        if(!check_list_contain_state(states, sourceState))
                            states.push_back(sourceState);
                    }
            }
        }

        return states;
    }

    // PreA
    list<State> PreA(list<State> y)
    {
        //pre∀(Y ) = pre∃y − pre∃(S − Y)
        list<State> PreEY = PreE(y);

        list<State> S_Minus_Y = list<State>();
        S_Minus_Y.AddRange(_kripke.States.ToArray());

        foreach (State state in y)
        {
            if (S_Minus_Y.Contains(state))
                S_Minus_Y.Remove(state);
        }

        list<State> PreE_S_Minus_Y = PreE(S_Minus_Y);

        //PreEY - PreE(S-Y)
        foreach (State state in PreE_S_Minus_Y)
        {
            if (PreEY.Contains(state))
                PreEY.Remove(state);
        }

        return PreEY;
    }

    // Determine whether the list contain same set of states
    bool ArelistStatesEqual(list<State> list1, list<State> list2)
    {
        if (list1.size() != list2.size())
            return false;

        for (State state : list1)
        {
            if ( list2.find(state) == string::npos)
                return false;
        }

        return true;
    }

    // Determine whether this is an atom
    bool IsAtomic(string expression)
    {
        for ( string x: _kripke.Atoms)
        {
            if ( x.find(expression) != string::npos)
                return true;
        }
        return false;
    }

    // Determine whether given expression contains binary operation for the next checking
    bool IsBinaryOp(string expression, string sym, string &leftExpression, string &rightExpression)
    {
        bool isBinaryOp = false;
        int len = expression.length();
        char symbol = sym[0];
        if (expression.find(symbol) != string::npos)
        {
            int openParanthesisCount = 0;
            int closeParanthesisCount = 0;

            for (int i = 0; i < len; i++)
            {
                char currentChar = expression[i];
                if (currentChar == symbol && openParanthesisCount == closeParanthesisCount)
                {
                    leftExpression = expression.substr(0, i);
                    rightExpression = expression.substr(i + 1, len - i - 1);
                    isBinaryOp = true;
                    break;
                }
                else if ( currentChar == '(')
                {
                    openParanthesisCount++;
                }
                else if (currentChar == ')')
                {
                    closeParanthesisCount++;
                }
            }
        }
        return isBinaryOp;
    }

    // Removing extra brackets
    string RemoveExtraBrackets(string expression)
    {
        string newExpression = expression;
        int openParanthesis = 0;
        int closeParanthesis = 0;
        int len = expression.length();

        if (expression[0] == '(' && expression[len-1] == ')')
        {
            for (int i = 0; i < len - 1; i++)
            {
                char charExpression = expression[i];

                if (charExpression == '(')
                    openParanthesis++;
                else if (charExpression == ')')
                    closeParanthesis++;
            }

            if (openParanthesis - 1 == closeParanthesis)
                newExpression = expression.substr(1, len - 2);
        }
        return newExpression;
    }

    //add function to check contain situation in a list for state
    bool check_list_contain_state(list<State> &dest, State src)
    {
        list<State>::iterator iter_check_list;
        for (iter_check_list = dest.begin(); iter_check_list != dest.end(); iter_check_list++)
        {
            if ( iter_check_list->StateName == src.StateName && &iter_check_list->Atom == &src.Atom)
                return true;
        }
        return false;
    }

};

#endif //SATPROJECT_CTLFORMULA_H
