#include <iostream>
#include "CtlFormular.h"
#include "KripkeStructure.h"
#include "State.h"
#include "Transition.h"

using namespace std;

int main(){
    //get model file
     cout << "Enter file name (kripke model): " << endl;
     ifstream _kripke;
     string input_kripke_file_name, input_kripke_model;
     cin >> input_kripke_file_name;
     string kripke_content;
     _kripke.open(input_kripke_file_name);

     if(_kripke.is_open()){
         while(!_kripke.eof()){
             _kripke >> kripke_content;
             input_kripke_model += kripke_content;
             //cout << kripke_content;
         }
     }
     else{
         cout<<"File can't open ";
     }
     _kripke.close();

     //get state
     string input_state;
     cout << "Enter state: " << endl;
     cin >> input_state;

     //get CTL formular
     string input_ctl_formular;
     cout << "Enter CTL formular: " << endl;
     cin >> input_ctl_formular;

     //for testing part
     cout << "File (Kripke model): " << input_kripke_model << endl;
     cout << "State : " << input_state << endl;
     cout << "CTL formular: " << input_ctl_formular;
    
    return 0;
}
