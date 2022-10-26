// Program Name: LoginApp.cpp
// Last Modification Date: 16/05/2022
// Purpose: Login Application Header File

#ifndef FINAL_LOGIN_LOGINFUNCTIONS_H
#define FINAL_LOGIN_LOGINFUNCTIONS_H

#include <bits/stdc++.h>
#include<conio.h>

using namespace std;

string ID;
int trials = 3;
int choose;
bool correct_user = false;
string new_ID;
struct user{
    string name, password, email, phone_number;
    set<string>all_passwords;
    user(string& n, string& pass, string& e, string& ph_num);
    user();
    string copy_information();
    string copy_previous_passwords();
};
user new_user;

void load_file_information();
void show_menu();
void take_choice();
void save_information();

#endif //FINAL_LOGIN_LOGINFUNCTIONS_H
