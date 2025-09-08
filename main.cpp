#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
using namespace std;

struct User {
    string username;
    string password;
    double balance;
};

// Load all users from file (line by line)
vector<User> loadUsers() {
    vector<User> users;
    ifstream infile("normaluser.dat");
    User user;
    while (getline(infile, user.username) &&
           getline(infile, user.password) &&
           (infile >> user.balance)) {
        infile.ignore(numeric_limits<streamsize>::max(), '\n'); 
        users.push_back(user);
    }
    infile.close();
    return users;
}

// Save all users back to file (overwrite)
void saveUsers(const vector<User>& users) {
    ofstream outfile("normaluser.dat");
    for (const auto& user : users) {
        outfile << user.username << endl;
        outfile << user.password << endl;
        outfile << user.balance << endl;
    }
    outfile.close();
}

// User login
string normaluserlogin(vector<User>& users, User& loggedInUser) {
    string username, password;
    cout << "***welcome to fintech bank***" << endl;
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;

    for (auto& user : users) {
        if (user.username == username && user.password == password) {
            cout << "Login successful!" << endl;
            loggedInUser = user;
            return username;
        }
    }
    cerr << "Invalid username or password." << endl;
    return "";
}

// Change user password
void userchangepassword(vector<User>& users, User& loggedInUser) {
    string newpassword;
    cout << "Enter your new password: ";
    cin >> newpassword;
    if (newpassword.length() < 8) {
        cerr << "Password must be at least 8 characters long." << endl;
        return;
    }
    for (auto& user : users) {
        if (user.username == loggedInUser.username) {
            user.password = newpassword;
            loggedInUser.password = newpassword;
            saveUsers(users);
            cout << "Password changed successfully!" << endl;
            return;
        }
    }
    cerr << "User not found." << endl;
}

// Withdraw money
void withdrawmoney(vector<User>& users, User& loggedInUser) {
    double amount;
    cout << "Enter the amount to withdraw: ";
    cin >> amount;
    if (amount <= 0) {
        cerr << "Invalid withdrawal amount." << endl;
        return;
    }
    for (auto& user : users) {
        if (user.username == loggedInUser.username) {
            if (user.balance >= amount) {
                user.balance -= amount;
                loggedInUser.balance = user.balance;
                saveUsers(users);
                cout << "Withdrawal of " << amount << " successful!" << endl;
            } else {
                cerr << "Insufficient balance." << endl;
            }
            return;
        }
    }
    cerr << "User not found." << endl;
}

// Check balance
void checkbalance(const User& loggedInUser) {
    cout << "Your current balance is: " << loggedInUser.balance << endl;
}

// Deposit money
void depositmoney(vector<User>& users, User& loggedInUser) {
    double amount;
    cout << "Enter the amount to deposit: ";
    cin >> amount;
    if (amount <= 0) {
        cerr << "Invalid deposit amount." << endl;
        return;
    }
    for (auto& user : users) {
        if (user.username == loggedInUser.username) {
            user.balance += amount;
            loggedInUser.balance = user.balance;
            saveUsers(users);
            cout << "Deposit of " << amount << " successful!" << endl;
            return;
        }
    }
    cerr << "User not found." << endl;
}

// Register new user
void registernewuser(vector<User>& users) {
    string newusername, newpassword;
    cout << "Enter the new user's username: ";
    cin >> newusername;
    cout << "Enter the new user's password: ";
    cin >> newpassword;

    if (newpassword.length() < 8) {
        cerr << "Password must be at least 8 characters long." << endl;
        return;
    }
    for (const auto& user : users) {
        if (user.username == newusername) {
            cerr << "Username already exists." << endl;
            return;
        }
    }
    users.push_back({newusername, newpassword, 0.0});
    saveUsers(users);
    cout << "New user registered successfully!" << endl;
}

// Admin login
string adminlogin() {
    string adminusername, adminpassword;
    cout << "***welcome to fintech bank***" << endl;
    cout << "Enter your admin username: ";
    cin >> adminusername;
    cout << "Enter your admin password: ";
    cin >> adminpassword;

    ifstream infile("admin.dat");
    if (!infile.is_open()) {
        cerr << "Error opening admin data file." << endl;
        return "";
    }

    string file_username, file_password;
    getline(infile, file_username);
    getline(infile, file_password);
    infile.close();

    if (adminusername == file_username && adminpassword == file_password) {
        cout << "Admin login successful!" << endl;
        return adminusername;
    } else {
        cerr << "Invalid admin username or password." << endl;
        return "";
    }
}

// Change admin password
void adminchangepassword() {
    string newpassword;
    cout << "Enter your new password: ";
    cin >> newpassword;
    if (newpassword.length() < 8) {
        cerr << "Password must be at least 8 characters long." << endl;
        return;
    }
    ofstream outfile("admin.dat");
    if (!outfile.is_open()) {
        cerr << "Error opening admin data file for writing." << endl;
        return;
    }
    outfile << "admin" << endl;
    outfile << newpassword << endl;
    outfile.close();
    cout << "Password changed successfully!" << endl;
}

// Main program
int main() {
    int choice;
    vector<User> users = loadUsers();

    cout << "***welcome to the banking system***\n";
    cout << "1. Admin Login\n";
    cout << "2. User Login\n";
    cout << "3. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1: {
            string adminusername = adminlogin();
            if (adminusername != "") {
                int adminchoice;
                do {
                    cout << "1. Register New User\n";
                    cout << "2. Change Admin Password\n";
                    cout << "3. Exit\n";
                    cout << "Enter your choice: ";
                    cin >> adminchoice;
                    switch (adminchoice) {
                        case 1:
                            registernewuser(users);
                            break;
                        case 2:
                            adminchangepassword();
                            break;
                        case 3:
                            cout << "Exiting admin menu..." << endl;
                            break;
                        default:
                            cerr << "Invalid choice." << endl;
                    }
                } while (adminchoice != 3);
            }
            break;
        }
        case 2: {
            User loggedInUser;
            string username = normaluserlogin(users, loggedInUser);
            if (username != "") {
                int userchoice;
                do {
                    cout << "1. Check Balance\n";
                    cout << "2. Withdraw Money\n";
                    cout << "3. Deposit Money\n";
                    cout << "4. Change Password\n";
                    cout << "5. Exit\n";
                    cout << "Enter your choice: ";
                    cin >> userchoice;
                    switch (userchoice) {
                        case 1:
                            checkbalance(loggedInUser);
                            break;
                        case 2:
                            withdrawmoney(users, loggedInUser);
                            break;
                        case 3:
                            depositmoney(users, loggedInUser);
                            break;
                        case 4:
                            userchangepassword(users, loggedInUser);
                            break;
                        case 5:
                            cout << "Exiting user menu..." << endl;
                            break;
                        default:
                            cerr << "Invalid choice." << endl;
                    }
                } while (userchoice != 5);
            }
            break;
        }
        case 3:
            cout << "Exiting..." << endl;
            break;
        default:
            cerr << "Invalid choice." << endl;
    }
}
  