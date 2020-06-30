Project: CTL model checker

Course: CS5392
Professor: Rodion Podorozhny
Semester: Summer 1, 2020
Team member(SM2): Kai Qi, Kaiping Xiong, Tianyi Gao, Huan Wu

Introduction 

We developed this project in C++ that implements a model checking analysis tool for verification of properties defined in the CTL temporal logic. 

Purpose

This program is able to ask user to input a test file name with Kripke structure that need to be analyzed , a state ID for which the property should be checked and
a CTL formula that defines the property. The output will show if the state with the CTL formula is hold or not hold in the given kripke model.
					
Overall Description

We built this program based on the C# example program. We also used GitHub to perform version control. Here is the UML class diagram of out project. 
The program contain four classes and an extra main class. The main class will pop up requests in the console and ask the user to input data. Kripke Structure will 
do syntax checking and parse model and provide meaningful error messages. CtlFormula class will handle model check after the kripkestructure part is passed. 
State class is the object of state. Transition class is the object of the graph edges.
 
