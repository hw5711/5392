//
// Created by Huan wu on 6/25/20.
//

#include "CtlFormula.h"

CtlFormula::CtlFormula(string expression, State state, KripkeStructure& kripke)
{

    // modify: consert dictionary to map
    _convertionString["and"] = "&";
    _convertionString["or"] = "|";
    _convertionString["->"] = ">";
    _convertionString["not"] = "!";

    _kripke = kripke;
    _state = state;
    _expression = ConvertToSystemFormula(expression); //parse the input Ctl check formula
}

string CtlFormula::ConvertToSystemFormula(string expression)
{

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

bool CtlFormula::IsSatisfy()
{
    list<State> states = SAT(_expression);        //get all states from the input ctl formula
                                                  //        if (states.Contains(_state))
    if (check_list_contain_state(states, _state)) //check all contents inside of state
        return true;
    return false;
}

TypeSAT CtlFormula::GetTypeSAT(string expression, string& leftExpression, string& rightExpression)
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
    if (expression.find("A(") == 0)
    {
        //            string strippedExpression = expression.Substring(2, expression.Length - 3);
        string strippedExpression = expression.substr(2, expression.length() - 3);
        if (IsBinaryOp(strippedExpression, "U", leftExpression, rightExpression))
            return AU;
    }
    //look for binary EU
    //        if (expression.StartsWith("E("))
    if (expression.find("E(") == 0)
    {
        //            string strippedExpression = expression.Substring(2, expression.Length - 3);
        string strippedExpression = expression.substr(2, expression.length() - 3);
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
    if (expression.find("!") == 0)
    {
        //            leftExpression = expression.Substring(1, expression.Length - 1);
        leftExpression = expression.substr(1, expression.length() - 1);
        return Not;
    }
    //        if (expression.StartsWith("AX"))
    if (expression.find("AX") == 0)
    {
        //            leftExpression = expression.Substring(2, expression.Length - 2);
        leftExpression = expression.substr(2, expression.length() - 2);
        return AX;
    }
    //        if (expression.StartsWith("EX"))
    if (expression.find("EX") == 0)
    {
        //            leftExpression = expression.Substring(2, expression.Length - 2);
        leftExpression = expression.substr(2, expression.length() - 2);
        return EX;
    }
    //        if (expression.StartsWith("EF"))
    if (expression.find("EF") == 0)
    {
        //            leftExpression = expression.Substring(2, expression.Length - 2);
        leftExpression = expression.substr(2, expression.length() - 2);
        return EF;
    }
    //        if (expression.StartsWith("EG"))
    if (expression.find("EG") == 0)
    {
        //            leftExpression = expression.Substring(2, expression.Length - 2);
        leftExpression = expression.substr(2, expression.length() - 2);
        return EG;
    }
    //        if (expression.StartsWith("AF"))
    if (expression.find("AF") == 0)

    {
        //            leftExpression = expression.Substring(2, expression.Length - 2);
        leftExpression = expression.substr(2, expression.length() - 2);
        return AF;
    }
    //        if (expression.StartsWith("AG"))
    if (expression.find("AG") == 0)
    {
        //            leftExpression = expression.Substring(2, expression.Length - 2);
        leftExpression = expression.substr(2, expression.length() - 2);
        return AG;
    }

    return UnKnown;
}

list<State> CtlFormula::SAT(string expression)
{
    cout << "Original Expression: " << expression;
    list<State> states = list<State>();

    //from Logic in Computer Science, page 227
    string leftExpression = "", rightExpression = "";

    //TypeSAT typeSAT = DetermineTypeSAT(expression, ref leftExpression, ref rightExpression);
    TypeSAT typeSAT = GetTypeSAT(expression, leftExpression, rightExpression);

    cout << "Type SAT: " << _kripke.ToString() << endl;
    cout << "Left Expression: " << leftExpression << endl;
    cout << "Right Expression: " << rightExpression << endl;

    switch (typeSAT)
    {
    case AllTrue:
    {
        //all states
        list<State>::iterator iter_state_all_true;
        for (iter_state_all_true = _kripke.States.begin();
            iter_state_all_true != _kripke.States.end(); iter_state_all_true++)
        {
            states.push_back(*iter_state_all_true);
        }
        break;
    }
    case AllFalse:
    {
        //empty
        break;
    }
    case Atomic:
    {
        list<State>::iterator iter_atomic;
        for (iter_atomic = _kripke.States.begin(); iter_atomic != _kripke.States.end(); iter_atomic++)
        {
            if (check_list_contain_string(iter_atomic->Atom, leftExpression))
            {
                states.push_back(*iter_atomic);
            }
        }
        break;
    }
    case Not:
    {
        //S − SAT (φ1)
        list<State>::iterator iter_state_not;
        for (iter_state_not = _kripke.States.begin(); iter_state_not != _kripke.States.end(); iter_state_not++)
        {
            states.push_back(*iter_state_not);
        }
        list<State> f1States = SAT(leftExpression);

        list<State>::iterator iter_f1States;
        for (iter_f1States = f1States.begin(); iter_f1States != f1States.end(); iter_f1States++)
        {
            if (check_list_contain_state(states, *iter_f1States))
            {
                remove_from_list(states, *iter_f1States);
            }
        }
        break;
    }
    case And:
    {
        //SAT (φ1) ∩ SAT (φ2)
        list<State> andF1States = SAT(leftExpression);
        list<State> andF2States = SAT(rightExpression);

        list<State>::iterator iter_and;
        for (iter_and = andF1States.begin(); iter_and != andF1States.end(); iter_and++)
        {
            if (check_list_contain_state(andF2States, *iter_and))
            {
                states.push_back(*iter_and);
            }
        }
        break;
    }
    case Or:
    {
        //SAT (φ1) ∪ SAT (φ2)
        list<State> orF1States = SAT(leftExpression);
        list<State> orF2States = SAT(rightExpression);

        states = orF1States;
        list<State>::iterator iter_or;
        for (iter_or = orF2States.begin(); iter_or != orF2States.end(); iter_or++)
        {
            if (!check_list_contain_state(states, *iter_or))
            {
                states.push_back(*iter_or);
            }
        }
        break;
    }
    case Implies:
    {
        //SAT (¬φ1 ∨ φ2)
        //TODO: reevaluate impliesFormula
        string impliesFormula = "!" + leftExpression + "|" + rightExpression;
        states = SAT(impliesFormula);
        break;
    }
    case AX:
    {
        //SAT (¬EX¬φ1)
        //TODO: reevaluate axFormula
        string axFormula = "!EX!" + leftExpression;
        states = SAT(axFormula);

        //check if states actually has link to next state
        list<State> tempStates = list<State>();
        list<State>::iterator iter_sourceState;
        for (iter_sourceState = states.begin(); iter_sourceState != states.end(); iter_sourceState++)
        {
            list<Transition>::iterator iter_transition;
            for (iter_transition = _kripke.Transitions.begin();
                iter_transition != _kripke.Transitions.end(); iter_transition++)
            {
                if (iter_sourceState->Equals(iter_transition->FromState))
                {
                    tempStates.push_back(*iter_sourceState);
                    break;
                }
            }
        }
        states = tempStates;
        break;
    }
    case EX:
    {
        //SATEX(φ1)
        //TODO: reevaluate exFormula
        string exFormula = leftExpression;
        states = SAT_EX(exFormula);
        break;
    }
    case AU:
    {
        //A[φ1 U φ2]
        //SAT(¬(E[¬φ2 U (¬φ1 ∧¬φ2)] ∨ EG¬φ2))
        //TODO: reevaluate auFormulaBuilder
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
    }
    case EU:
    {
        //SATEU(φ1, φ2)
        //TODO: reevaluate euFormula
        states = SAT_EU(leftExpression, rightExpression);
        break;
    }
    case EF:
    {
        //SAT (E( U φ1))
        //TODO: reevaluate efFormula
        string efFormula = "E(TU" + leftExpression + ")";
        states = SAT(efFormula);
        break;
    }
    case EG:
    {
        //SAT(¬AF¬φ1)
        //TODO: reevaulate egFormula
        string egFormula = "!AF!" + leftExpression;
        states = SAT(egFormula);
        break;
    }
    case AF:
    {
        //SATAF (φ1)
        //TODO: reevaluate afFormula
        string afFormula = leftExpression;
        states = SAT_AF(afFormula);
        break;
    }
    case AG:
    {
        //SAT (¬EF¬φ1)
        //TODO: reevaluate agFormula
        string agFormula = "!EF!" + leftExpression;
        states = SAT(agFormula);
        break;
    }
    case UnKnown:
        cout << "Invalid CTL expression";
    }

    return states;
}

list<State> CtlFormula::SAT_EX(string expression)
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

list<State> CtlFormula::SAT_EU(string leftExpression, string rightExpression)
{
    list<State> w = list<State>();
    list<State> x = list<State>();
    list<State> y = list<State>();

    w = SAT(leftExpression);
    list<State>::iterator iter_state_x;
    for (iter_state_x = _kripke.States.begin(); iter_state_x != _kripke.States.end(); iter_state_x++)
    {
        x.push_back(*iter_state_x);
    }
    y = SAT(rightExpression);

    while (!ArelistStatesEqual(x, y))
    {
        x = y;
        list<State> newY = list<State>();
        list<State> preEStates = PreE(y);

        list<State>::iterator iter_state_newY;
        for (iter_state_newY = y.begin(); iter_state_newY != y.end(); iter_state_newY++)
        {
            newY.push_back(*iter_state_newY);
        }

        list<State> wAndPreE = list<State>();
        list<State>::iterator iter_state_w;
        for (iter_state_w = w.begin(); iter_state_w != w.end(); iter_state_w++)
        {
            if (check_list_contain_state(preEStates, *iter_state_w))
            {
                wAndPreE.push_back(*iter_state_w);
            }
        }

        list<State>::iterator iter_wAndPreE;
        for (iter_wAndPreE = wAndPreE.begin(); iter_wAndPreE != wAndPreE.end(); iter_wAndPreE++)
        {
            if (!check_list_contain_state(newY, *iter_wAndPreE))
            {
                newY.push_back(*iter_wAndPreE);
            }
        }
        y = newY;
    }

    return y;
}

list<State> CtlFormula::SAT_AF(string expression)
{
    list<State> x = list<State>();
    list<State>::iterator iter_state_x;
    for (iter_state_x = _kripke.States.begin(); iter_state_x != _kripke.States.end(); iter_state_x++)
    {
        x.push_back(*iter_state_x);
    }
    list<State> y = list<State>();
    y = SAT(expression);

    while (!ArelistStatesEqual(x, y))
    {
        x = y;
        list<State> newY = list<State>();
        list<State> preAStates = PreA(y);
        list<State>::iterator iter_state_newY;
        for (iter_state_newY = y.begin(); iter_state_newY != y.end(); iter_state_newY++)
        {
            newY.push_back(*iter_state_newY);
        }

        list<State>::iterator iter_preAStates;
        for (iter_preAStates = preAStates.begin(); iter_preAStates != preAStates.end(); iter_preAStates++)
        {
            if (!check_list_contain_state(newY, *iter_preAStates))
            {
                newY.push_back(*iter_preAStates);
            }
        }
        y = newY;
    }

    return y;
}

list<State> CtlFormula::PreE(list<State> y)
{
    //{s âˆˆ S | exists s, (s â†’ s and s âˆˆ Y )}
    list<State> states; // = new list<State>();

    list<Transition> transitions = list<Transition>();

    list<State>::iterator iter_sourceState;
    for (iter_sourceState = _kripke.States.begin(); iter_sourceState != _kripke.States.end(); iter_sourceState++)
    {
        list<State>::iterator iter_destState;
        for (iter_destState = y.begin(); iter_destState != y.end(); iter_destState++)
        {
            Transition myTransition = Transition(*iter_sourceState, *iter_destState);
            if (check_list_contain_transition(_kripke.Transitions, myTransition))
            {
                if (!check_list_contain_state(states, *iter_sourceState))
                {
                    states.push_back(*iter_sourceState);
                }
            }
        }
    }

    return states;
}

list<State> CtlFormula::PreA(list<State> y)
{
    //pre∀(Y ) = pre∃y − pre∃(S − Y)
    list<State> PreEY = PreE(y);

    list<State> S_Minus_Y = list<State>();
    list<State>::iterator iter_state_S_Minus_Y;
    for (iter_state_S_Minus_Y = _kripke.States.begin(); iter_state_S_Minus_Y != _kripke.States.end(); iter_state_S_Minus_Y++)
    {
        S_Minus_Y.push_back(*iter_state_S_Minus_Y);
    }

    list<State>::iterator iter_y;
    for (iter_y = y.begin(); iter_y != y.end(); iter_y++)
    {
        if (check_list_contain_state(S_Minus_Y, *iter_y))
        {
            remove_from_list(S_Minus_Y, *iter_y);
        }
    }

    list<State> PreE_S_Minus_Y = PreE(S_Minus_Y);

    //PreEY - PreE(S-Y)
    list<State>::iterator iter_PreE_S_Minus_Y;
    for (iter_PreE_S_Minus_Y = PreE_S_Minus_Y.begin(); iter_PreE_S_Minus_Y != PreE_S_Minus_Y.end(); iter_PreE_S_Minus_Y++)
    {
        if (check_list_contain_state(PreEY, *iter_PreE_S_Minus_Y))
        {
            remove_from_list(PreEY, *iter_PreE_S_Minus_Y);
        }
    }

    return PreEY;
}

bool CtlFormula::ArelistStatesEqual(list<State> list1, list<State> list2)
{
    if (list1.size() != list2.size())
        return false;

    list<State>::iterator iter_list1;
    for (iter_list1 = list1.begin(); iter_list1 != list1.end(); iter_list1++)
    {
        if (!check_list_contain_state(list2, *iter_list1))
        {
            return false;
        }
    }

    return true;
}

bool CtlFormula::IsAtomic(string expression)
{
    if (check_list_contain_string(_kripke.Atoms, expression))
        return true;
    return false;
}

bool CtlFormula::IsBinaryOp(string expression, string sym, string& leftExpression, string& rightExpression)
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
            else if (currentChar == '(')
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

string CtlFormula::RemoveExtraBrackets(string expression)
{
    string newExpression = expression;
    int openParanthesis = 0;
    int closeParanthesis = 0;
    int len = expression.length();

    if (expression[0] == '(' && expression[len - 1] == ')')
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

bool CtlFormula::check_list_contain_state(list<State>& dest, State src)
{
    list<State>::iterator iter_check_list;
    for (iter_check_list = dest.begin(); iter_check_list != dest.end(); iter_check_list++)
    {
        if (iter_check_list->Equals(src))
            return true;
    }
    return false;
}

void CtlFormula::remove_from_list(list<State>& dest, State src)
{
    list<State>::iterator iter_dest;
    bool flag = false;
    for (iter_dest = dest.begin(); iter_dest != dest.end(); iter_dest++)
    {
        if (iter_dest->Equals(src))
        {
            flag = true;
            break;
        }
    }
    if (flag) dest.erase(iter_dest);
}

bool CtlFormula::check_list_contain_transition(list<Transition>& dest, Transition src)
{
    list<Transition>::iterator iter_check_list;
    for (iter_check_list = dest.begin(); iter_check_list != dest.end(); iter_check_list++)
    {
        if (iter_check_list->Equals(src))
            return true;
    }
    return false;
}

bool CtlFormula::check_list_contain_string(list<string>& dest, string src)
{
    list<string>::iterator iter_check_list;
    for (iter_check_list = dest.begin(); iter_check_list != dest.end(); iter_check_list++)
    {
        if (*iter_check_list == src)
            return true;
    }
    return false;
}