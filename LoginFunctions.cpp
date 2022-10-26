// Program Name: LoginApp.cpp
// Last Modification Date: 16/05/2022
// Purpose: Login Application Function Header File

#include "LoginFunctions.h"
// -----------------------------------validation----------------------------
bool email_validation(string& s){
    string tmp = "[(a-zA-z0-9)(# ! % $ ‘ & + * – / = ? ^ _` { | } ~)]+";
    regex state(tmp + "(.)?" + tmp + "[@][a-z]+(.com)");
    return regex_match(s,state);
}

bool phone_number_validation(string &s){
    regex state("(01)[0|1|2|5][\\d]{8}");
    return regex_match(s,state);
}

bool user_name_validation(string& s){
    regex state("[a-zA-Z(_)]+");
    return regex_match(s,state);
}

bool password_validation(string& s){
    if(s.size() < 12){
        return 0;
    }
    int digits = 0, alphas = 0, symbols = 0;
    for (int i = 0; i < s.size(); ++i) {
        if(isalpha(s[i])){
            alphas ++;
        }else if(isdigit(s[i])){
            digits ++;
        }else{
            symbols ++;
        }
    }
    return digits * alphas * symbols;
}
void separated_line(string paragraph){
    // centre each paragraph with (-) in each part of this paragraph
    int width = 80;
    char symbol = '-';
    int part = (width/2) - ((int)paragraph.size()/2);
    cout << string(part,symbol) + paragraph + string(part,symbol) << '\n';
}

void email_format(){
    separated_line("");
    cout << "Email must be in the format\n";
    cout << "alphabets or digit characters followed by @ then domain name and in the end (.com)\n";
}

void phone_number_format(){
    separated_line("");
    cout << "Phone number must be in the format\n";
    cout << "(010) or (011) or (012) or (015) + (8)digits\n";
}

void user_name_format(){
    separated_line("");
    cout << "User name must be in the format\n";
    cout << "only alphabetic letters or underscore(_)\n";
}

void password_format(){
    separated_line("");
    cout << "Password must be in the format\n";
    cout << "must contain at least one alphabetic character and one digit and one symbol\n";
    cout << "the length of the password must be at least 12 letters\n";
}

void hidden_password(string& s, string paragraph){
    char c;
    cout << paragraph;
    while (true) {
        c = getch();
        if(c == 13){
            // if the user press enter, break the loop
            break;
        }else if(c == 8 && s.size()){
            // if user press backspace
            s.pop_back();
            wcout << '\b' << ' ' << '\b';
        }else if(c != 8){
            cout << '*';
            s.push_back(c);
        }
    }
    cout << '\n';
}
//--------------------------------user information----------------------------
// cipher user password
void cipher(string& password){
    for (int i = 0; i < password.size(); ++i) {
        password[i] =  char(93 - (password[i] -'A') +'A');
    }
}

// user class that has all user details

user::user(string& n, string& pass, string& e, string& ph_num) {
    // fill user information in his class
    name = n;
    password = pass;
    email = e;
    phone_number = ph_num;
    all_passwords.insert(pass);
}


string user::copy_information() {
    cipher(password);
    return name + ' ' + password + ' ' + email + ' ' + phone_number + '\n';
}

user::user() {
    cout << "";
}

string user::copy_previous_passwords() {
    string tmp = "";
    for(auto val : all_passwords){
        cipher(val);
        tmp += ' ' +  val;
    }
    return tmp + '\n';
}


map<string, user>users_list; // the key is the user ID
set<string>emails;

void load_file_information(){
    // fill users information in user list map
    fstream users_information;
    users_information.open("UsersInfo.txt",ios::in);
    string line;
    while (getline(users_information, line)){
        // get each line from the file
        istringstream sep_line(line);
        vector<string>info;
        string tmp;
        // separate each string in the line and store it in vector to build a user class
        // [0] ID, [1] name, [2] password, [3] email, [4] phone number
        while (sep_line >> tmp){
            info.push_back(tmp);
        }
        // create a user class of his information
        // decipher password
        cipher(info[2]);
        user user_class(info[1],info[2],info[3],info[4]);
        // store each user information in this map
        users_list.insert({info[0],user_class});
        // store each email in the emails set
        emails.insert(info[3]);
    }
    users_information.close();
    //-----------------------------------------------------------------------------------------
    //fill each user passwords set with previous passwords
    fstream  previous_passwords;
    previous_passwords.open("UserPasswords.txt", ios::in);
    while (getline(previous_passwords,line)){
        // this tmp vector contain user ID in the first index and other indexes are user previous passwords
        vector<string>tmp;
        string word;
        istringstream sep_words(line);
        while (sep_words >> word){
            tmp.push_back(word);
        }
        // fill user id with his passwords
        for (int i = 1; i < tmp.size(); ++i) {
            cipher(tmp[i]);
            users_list[tmp[0]].all_passwords.insert(tmp[i]);
        }
    }
    previous_passwords.close();
}

void save_information(){
    // open users information, all emails and previous passwords, delete its content, and append new one
    fstream users_information,previous_passwords;
    users_information.open("UsersInfo.txt",ios::out);
    previous_passwords.open("UserPasswords.txt",ios::out);
    for(auto& val : users_list){
        // line will contain each user information
        string line ="";
        line += val.first + ' ';
        line += val.second.copy_information();
        // append each line information into the user file
        users_information << line;
        // append user id and his previous passwords
        line = val.first + val.second.copy_previous_passwords();
        previous_passwords << line;
    }
    previous_passwords.close();
    users_information.close();
}
//---------------------------------------login--------------------------------
void inquire_id(){
    separated_line("");
    cout << "Enter your ID: ";
    cin >> ID;
    if(!users_list.count(ID)){
        separated_line(" Please enter correct user ID ");
        inquire_id();
        return;
    }
}
void inquire_password(){
    separated_line("");
    if(!trials){
        separated_line(" Failed login, try again ");
        trials = 3;
        return;
    }
    string pass = "";
    hidden_password(pass, "Enter your password: ");
    if(users_list[ID].password != pass){
        separated_line(" You entered wrong password ");
        trials--;
        inquire_password();
        return;
    }
    correct_user = true;
}

bool login_menu(){
    inquire_id();
    inquire_password();
    if(correct_user){
        separated_line(" Successful login, welcome " + users_list[ID].name + ' ');
        return 1;
    }else{
        return 0;
    }
}
//----------------------------change password-------------------------------
void changed_password(){
    string password = "", check_password = "";
    password_format();
    hidden_password(password, "Enter your new password: ");
    if(users_list[ID].all_passwords.count(password)){
        separated_line(" This password was taken before ");
        changed_password();
        return;
    }
    if(!password_validation(password)){
        separated_line(" Enter correct password format ");
        changed_password();
        return;
    }
    hidden_password(check_password, "Enter your new password again: ");
    if(password != check_password){
        separated_line(" please enter the same password ");
        changed_password();
        return;
    }
    users_list[ID].password = password;
    users_list[ID].all_passwords.insert(password);
}


void change_password_menu(){
    login_menu();
    changed_password();
}
//-----------------------registration------------------------
void enter_id(){
    separated_line("");
    cout << "Enter your new ID: ";
    cin >> new_ID;
    if(users_list.count(new_ID)){
        separated_line(" This ID was used before ");
        enter_id();
        return;
    }
}

void enter_user_name(){
    user_name_format();
    string tmp;
    cout << "Enter your name: ";
    cin >> tmp;
    if(!user_name_validation(tmp)){
        separated_line(" Please enter correct user name format ");
        enter_user_name();
        return;
    }
    new_user.name = tmp;
}

void enter_password(){
    password_format();
    string password = "", check_password = "";
    hidden_password(password, "Enter your password: ");
    if(!password_validation(password)){
        separated_line(" Enter correct password format ");
        enter_password();
        return;
    }
    hidden_password(check_password, "Enter your password again: ");
    if(password != check_password){
        separated_line(" Please enter the same password ");
        enter_password();
        return;
    }
    new_user.password = password;
    new_user.all_passwords.insert(password);
}

void enter_email(){
    email_format();
    string tmp;
    cout << "Enter your email: ";
    cin >> tmp;
    if(emails.count(tmp)){
        separated_line(" This emails was used before! ");
        enter_email();
        return;
    }
    if(!email_validation(tmp)){
        separated_line(" Enter correct email format ");
        enter_email();
        return;
    }
    new_user.email = tmp;
}

void enter_phone_number(){
    phone_number_format();
    string tmp;
    cout << "Enter your phone number\n";
    cin >> tmp;
    if(!phone_number_validation(tmp)){
        separated_line(" Enter correct phone number format ");
        enter_phone_number();
        return;
    }
    new_user.phone_number = tmp;
}

void update_users_list(){
    users_list.insert({new_ID,new_user});
    emails.insert(new_user.email);
}

void register_menu(){
    enter_id();
    enter_user_name();
    enter_password();
    enter_email();
    enter_phone_number();
    update_users_list();
}


void fill_space(string paragraph, int paragraph_width){
    int width = 180;
    cout << string((width/2) - (paragraph_width/2),' ')<< paragraph;
}

void show_menu(){
    // store all paragraphs in a vector
    vector<string>menu_choices(5);
    menu_choices[0] = "Choose what you want from this menu";
    menu_choices[1] = "1- Register";
    menu_choices[2] = "2- Login |";
    menu_choices[3] = "3- Change password";
    menu_choices[4] = "4- Exit";
    // get max width of this paragraphs
    int max_paragraph_width = 0;
    for(auto& val : menu_choices){
        max_paragraph_width = max(max_paragraph_width, (int)val.size());
    }
    // center welcome paragraph
    fill_space("Welcome",-(max_paragraph_width/2) + 20);
    cout << '\n';
    // print the first (-) line in the shape
    fill_space(string(max_paragraph_width + 5,'-'),max_paragraph_width-1);
    cout << '\n';
    // print shape content and its sides
    for(auto& val : menu_choices){
        fill_space("|  " + val,max_paragraph_width);
        cout << string(max_paragraph_width - (int)val.size(),' ') << " |\n";
    }
    // print the last (-) line in the shape
    fill_space(string(max_paragraph_width + 5,'-'),max_paragraph_width-1);
    cout << '\n';
}

void take_choice(){
    cout << "Enter your choice: ";
    cin >> choose;
    if(choose < 0 || choose > 4){
        separated_line(" Enter correct choice ");
        take_choice();
        return;
    }
    switch (choose) {
        case 1:
            register_menu();
            break;
        case 2:
            if(!login_menu()){
                show_menu();
                take_choice();
                return;
            }
            break;
        case 3:
            change_password_menu();
            break;
        case 4:
            return;
        default:
            break;
    }
}