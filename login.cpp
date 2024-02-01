
/**
 * A command line login/signup form that compares 
 * user inputs to the shadow.txt file which stores 
 * usernames and password hashes, and displayes
 * whether or not the login/signup was successful.
 * 
 * @file login.cpp
 * @author Aidin Lehrman
 * @version 1/16/2024 
 */

#include <iostream>
#include <fstream> // Read and write to/from files
#include <functional> // Hasher
#include <limits>

bool isValidUsername(const std::string& input) {
    std::string validChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";

    for (char ch : input) {
        if (validChars.find(ch) == std::string::npos) {
            std::cout << "\rInput Error: Please Enter an Alphanumeric Username." << std::endl;
            return false;
        }
    }
    std::string line;
    return true;
}

std::pair<std::string, std::string> getUsernameAndPasswordFromUser() {
    std::cout << "===== User Login (Username has to be Alphanumeric) =====" << std::endl;

    std::string username;
    do {
        std::cout << "Enter Username: ";
        getline(std::cin, username);
    } while (!isValidUsername(username));

    std::string password;
    std::cout << "Enter Password: ";
    getline(std::cin, password);
    
    return {username, password};
}

std::pair<std::string, std::string> getNewUserCredentials() {
    std::cout << "===== New User Sign Up (Username has to be Alphanumeric) =====" << std::endl;

    std::string username;
    do {
        std::cout << "Enter New Username: ";
        getline(std::cin, username);
    } while (!isValidUsername(username));

    std::string password1;
    std::cout << "Enter Password: ";
    getline(std::cin, password1);

    std::string password2;
    std::cout << "Enter Password Again: ";
    getline(std::cin, password2);

    bool passwordDoNotMatch = password1 != password2;
    if (passwordDoNotMatch) {
        std::cout << "Passwords Do Not Match: Please Try Again.";
        return getNewUserCredentials();
    }

    return {username, password1};
}

std::pair<std::string, std::string> parseShadowLine(std::string line) {
    
    std::size_t usernameStartPos = line.find(':', 0) + 1;
    std::size_t usernameEndPos = line.find(':', usernameStartPos + 1);
    std::string username = line.substr(usernameStartPos, usernameEndPos - usernameStartPos);

    std::size_t passwordStartPos = usernameEndPos + 2;
    std::size_t passwordEndPos = line.find(':', passwordStartPos + 1);
    std::string password = line.substr(passwordStartPos, passwordEndPos - passwordStartPos);

    return {username, password};
}

std::size_t hashPassword(const std::string password) {
    std::hash<std::string> hasher;
    return hasher(password);
}

bool isComment(std::string line) {
    int i = 0;
    while (line.at(i) == ' ') {
        i++;
    }
    i++;
    bool isLineComment = line.at(i) == '#';
    return isLineComment;
}

bool loginCredentialsAreCorrect(std::string enteredUsername, std::string enteredPassword, std::fstream& file) {
    std::string line;
    while (getline(file, line)) {
        if (isComment(line)) {
            continue;
        }

        std::pair<std::string, std::string> storedUsernameAndPasswordHash = parseShadowLine(line);

        std::string storedUsername = storedUsernameAndPasswordHash.first;
        std::string storedPasswordHash = storedUsernameAndPasswordHash.second;

        bool usernameCorrect = storedUsername == enteredUsername;
        if (!usernameCorrect) {
            continue;
        }

        std::string enteredPasswordHash = std::to_string(hashPassword(enteredPassword));

        bool passwordCorrect = storedPasswordHash == enteredPasswordHash;
        if (passwordCorrect) {
            return true;
        }
    }

    return false;
}

bool login(std::fstream& file) {
    std::pair<std::string, std::string> enteredCredentials = getUsernameAndPasswordFromUser();
    std::string enteredUsername = enteredCredentials.first;
    std::string enteredPassword = enteredCredentials.second;

    bool loginCorrect = loginCredentialsAreCorrect(enteredUsername, enteredPassword, file);

    if (loginCorrect) {
        return true;
    } else {
        return false;
    }
}

bool signup(std::fstream& file) {
    std::pair<std::string, std::string> newUserCredentials = getNewUserCredentials();

    std::string username = newUserCredentials.first;
    std::string password = newUserCredentials.second;

    std::string line;
    while (getline(file, line)) {
        std::string storedUsername = parseShadowLine(line).first;
        if (storedUsername == username) {
            return false;
        }
    }

    std::string comment = "# :" + username + "::" + password + ":\n";
    file << comment;

    std::string hashedPassword = std::to_string(hashPassword(password));
    std::string newShadowFileLine = ":" + username + "::" + hashedPassword + ":\n";
    file << newShadowFileLine;

    return true;
}

int main() {
    std::fstream file;
    file.open("shadow.txt", std::ios::app);

    char option;
    std::cout << "Login (1) or Signup (2)? ";
    std::cin >> option;
    std::cout << std::endl;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear input buffer

    bool isValidOption = option == '1' || option == '2';
    if (!isValidOption) {
        std::cout << "Invalid Option: Please Enter \'1\' or \'2\'" << std::endl;
        file.close();
        main();
        return 0;
    }

    bool userWantsToLogin = option == '1';
    if (userWantsToLogin) {
        bool loginSuccessful = login(file);
        if(!loginSuccessful) {
            std::cout << "Login Incorrect: Please Try Again" << std::endl;
            file.close();
            main();
            return 0;
        };
        std::cout << "Login Correct!" << std::endl;
    }

    bool userWantsToSignup = option == '2';
    if (userWantsToSignup) {
        bool signupSuccessful = signup(file);
        if (!signupSuccessful) {
            std::cout << "Signup Failed: Username Already Exists.";
            file.close();
            main();
            return 0;
        }
        std::cout << "Signup Successful!" << std::endl;
    }
    
    // std::cout << "Username: " << enteredUsername << "\n";
    // std::cout << "Password: " << enteredPassword << "\n";
    // std::cout << "Hash: " << hashPassword(enteredPassword) << std::endl;

    file.close();
    return 0;
}
