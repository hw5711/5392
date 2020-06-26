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
        _expression = ConvertToSystemFormula(expression);
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
        list<State> states = SAT(_expression);
        if (states.Contains(_state))
            return true;
        return false;
    }

private:
    TypeSAT GetTypeSAT(string expression, ref string leftExpression, ref string rightExpression)
    {
        //remove extra brackets
        expression = RemoveExtraBrackets(expression);

        //look for binary implies
        if (expression.Contains(">"))
        {
            if (IsBinaryOp(expression, ">", ref leftExpression, ref rightExpression))
                return Implies;
        }
        //look for binary and
        if (expression.Contains("&"))
        {
            if (IsBinaryOp(expression, "&", ref leftExpression, ref rightExpression))
                return And;
        }
        //look for binary or
        if (expression.Contains("|"))
        {
            if (IsBinaryOp(expression, "|", ref leftExpression, ref rightExpression))
                return Or;
        }
        //look for binary AU
        if (expression.StartsWith("A("))
        {
            string strippedExpression = expression.Substring(2, expression.Length - 3);
            if (IsBinaryOp(strippedExpression, "U", ref leftExpression, ref rightExpression))
                return AU;
        }
        //look for binary EU
        if (expression.StartsWith("E("))
        {
            string strippedExpression = expression.Substring(2, expression.Length - 3);
            if (IsBinaryOp(strippedExpression, "U", ref leftExpression, ref rightExpression))
                return EU;
        }

        //look for unary T, F, !, AX, EX, AG, EG, AF, EF, atomic
        if (expression.Equals("T"))
        {
            leftExpression = expression;
            return AllTrue;
        }
        if (expression.Equals("F"))
        {
            leftExpression = expression;
            return AllFalse;
        }
        if (IsAtomic(expression))
        {
            leftExpression = expression;
            return Atomic;
        }
        if (expression.StartsWith("!"))
        {
            leftExpression = expression.Substring(1, expression.Length - 1);
            return Not;
        }
        if (expression.StartsWith("AX"))
        {
            leftExpression = expression.Substring(2, expression.Length - 2);
            return AX;
        }
        if (expression.StartsWith("EX"))
        {
            leftExpression = expression.Substring(2, expression.Length - 2);
            return EX;
        }
        if (expression.StartsWith("EF"))
        {
            leftExpression = expression.Substring(2, expression.Length - 2);
            return EF;
        }
        if (expression.StartsWith("EG"))
        {
            leftExpression = expression.Substring(2, expression.Length - 2);
            return EG;
        }
        if (expression.StartsWith("AF"))
        {
            leftExpression = expression.Substring(2, expression.Length - 2);
            return AF;
        }
        if (expression.StartsWith("AG"))
        {
            leftExpression = expression.Substring(2, expression.Length - 2);
            return AG;
        }

        return Unknown;
    }

    // Determine states that satisfy given expression
    list<State> SAT(string expression)
    {
        System.Diagnostics.Debug.WriteLine(string.Format("Original Expression: {0}", expression));
        list<State> states = new list<State>();

        //from Logic in Computer Science, page 227
        string leftExpression = string.Empty, rightExpression = string.Empty;

        //TypeSAT typeSAT = DetermineTypeSAT(expression, ref leftExpression, ref rightExpression);
        TypeSAT typeSAT = GetTypeSAT(expression, ref leftExpression, ref rightExpression);

        System.Diagnostics.Debug.WriteLine(string.Format("Type SAT: {0}", ToString()));
        System.Diagnostics.Debug.WriteLine(string.Format("Left Expression: {0}", leftExpression));
        System.Diagnostics.Debug.WriteLine(string.Format("Right Expression: {0}", rightExpression));
        System.Diagnostics.Debug.WriteLine("------------------------------------");

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
            string impliesFormula = string.Concat("!", leftExpression, "|", rightExpression);
            states = SAT(impliesFormula);
            break;
        case AX:
            //SAT (¬EX¬φ1)
            //TODO: reevaluate axFormula
            string axFormula = string.Concat("!", "EX", "!", leftExpression);
            states = SAT(axFormula);

            //check if states actually has link to next state
            list<State> tempStates = new list<State>();
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
            StringBuilder auFormulaBuilder = new StringBuilder();
            auFormulaBuilder.Append("!(E(!");
            auFormulaBuilder.Append(rightExpression);
            auFormulaBuilder.Append("U(!");
            auFormulaBuilder.Append(leftExpression);
            auFormulaBuilder.Append("&!");
            auFormulaBuilder.Append(rightExpression);
            auFormulaBuilder.Append("))|(EG!");
            auFormulaBuilder.Append(rightExpression);
            auFormulaBuilder.Append("))");
            states = SAT(auFormulaBuilder.ToString());
            break;
        case EU:
            //SATEU(φ1, φ2)
            //TODO: reevaluate euFormula
            states = SAT_EU(leftExpression, rightExpression);
            break;
        case EF:
            //SAT (E( U φ1))
            //TODO: reevaluate efFormula
            string efFormula = string.Concat("E(TU", leftExpression, ")");
            states = SAT(efFormula);
            break;
        case EG:
            //SAT(¬AF¬φ1)
            //TODO: reevaulate egFormula
            string egFormula = string.Concat("!AF!", leftExpression);
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
            string agFormula = string.Concat("!EF!", leftExpression);
            states = SAT(agFormula);
            break;
        case Unknown:
            throw new FormatException("Invalid CTL expression");
        }

        return states;
    }

    //Handling EX
    list<State> SAT_EX(string expression)
    {
        //X := SAT (φ);
        //Y := pre∃(X);
        //return Y
        list<State> x = new list<State>();
        list<State> y = new list<State>();
        x = SAT(expression);
        y = PreE(x);
        return y;
    }

    // Handling EU
    list<State> SAT_EU(string leftExpression, string rightExpression)
    {
        list<State> w = new list<State>();
        list<State> x = new list<State>();
        list<State> y = new list<State>();

        w = SAT(leftExpression);
        x.AddRange(_kripke.States.ToArray());
        y = SAT(rightExpression);

        while (!ArelistStatesEqual(x, y))
        {
            x = y;
            list<State> newY = new list<State>();
            list<State> preEStates = PreE(y);

            newY.AddRange(y.ToArray());
            list<State> wAndPreE = new list<State>();
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
        list<State> x;// = new list<State>();
        x.assign( _kripke.States.begin(), _kripke.States.end());
        list<State> y;// = new list<State>();
        y = SAT(expression);

        while (!ArelistStatesEqual(x, y))
        {
            x = y;
            list<State> newY;// = new list<State>();
            list<State> preAStates = PreA(y);
            newY.assign( y.begin(), y.end());

            for (State state : preAStates)
            {
                if ( newY.find(state) == newY.length())
                    newY.push_back(state);
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
                Transition myTransition;// = new Transition(sourceState, destState);
                for( x : _kripke.Transitions)
	                if ( x.find(myTransition) != string::npos)
    	            {
        	            if (!states.Contains(sourceState))
            	            states.push_back(sourceState);
                	}
            }
        }

        return states;
    }

    // PreA
    list<State> PreA(list<State> y)
    {
        //preâˆ€(Y ) = preâˆƒy âˆ’ preâˆƒ(S âˆ’ Y)
        list<State> PreEY = PreE(y);

        list<State> S_Minus_Y;// = new list<State>();
        for( State x : _kripke.States)
			S_Minus_Y.push_back(x);

        for (State state : y)
        {
        	for( State x : S_Minus_Y)
            	if ( x.find(state))
                	S_Minus_Y.Remove(x);
        }

        list<State> PreE_S_Minus_Y = PreE(S_Minus_Y);

        //PreEY - PreE(S-Y)
        for (State state : PreE_S_Minus_Y)
        {
        	for( State x in PreEY)
	            if ( x.find(state))
    	            PreEY.remove(x);
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

};

#endif //SATPROJECT_CTLFORMULA_H
