#include "CtlFormula.h"
#include "KripkeStructure.h"
#include "State.h"
#include "Transition.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//Need to update
string GetMessage(bool isSatisfy, string expression, string stateID)
{
    string message = "xx";
    /*** need to check ***/
    //              string.Format("Property {0} {1} in state {2}"
    //            , expression
    //            , isSatisfy ? "holds" : "does not hold"
    //            , stateID);

    return message;
}

int main()
{

    KripkeStructure _kripke;
    //get model file
    cout << "Enter file name (kripke model): " << endl;
    ifstream _kripke_file;
    string input_kripke_file_name, input_kripke_model;
    cin >> input_kripke_file_name;
    string kripke_content;
    _kripke_file.open(input_kripke_file_name);

    if (_kripke_file.is_open())
    {
        while (!_kripke_file.eof())
        {
            _kripke_file >> kripke_content;
            input_kripke_model += kripke_content;
            //cout << kripke_content;
        }
    }
    else
    {
        cout << "File can't open ";
    }
    _kripke_file.close();

    //get state
    string input_state;
    cout << "Enter state: " << endl;
    cin >> input_state;

    //get CTL formular
    string input_ctl_formula;
    cout << "Enter CTL formula: " << endl;
    cin >> input_ctl_formula;

    //for testing part
    cout << "File (Kripke model): " << input_kripke_model << endl;
    cout << "State: " << input_state << endl;
    cout << "CTL formula: " << input_ctl_formula;

    State checkedState = State(input_state);
    CtlFormula ctlFormula = CtlFormula(input_kripke_model, checkedState, _kripke);
    bool isSatisfy = ctlFormula.IsSatisfy();
    string message = GetMessage(isSatisfy, input_ctl_formula, input_state);
    cout << "Result is: " << message << endl;

    return 0;
}
