
/**
 * @file login.cpp
 * @author Aidin Lehrman
 * @version 1/16/2024 
 */

#include <iostream>
#include <fstream>
#include <functional>

bool isValidUsername(const std::string& input) {
    std::string validChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";

    for (char ch : input) {
        if (validChars.find(ch) == std::string::npos) {
            std::cout << "\rEntered Username is not Alphanumeric.\n";
            std::cout << "Please Enter an Alphanumeric Username." << std::endl;
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
        std::cout << "\rUsername: ";
        getline(std::cin, username);
    } while (!isValidUsername(username));

    std::string password;
    std::cout << "\rPassword: ";
    getline(std::cin, password);
    
    return {username, password};
}

std::pair<std::string, std::string> parseShadowLine(std::string line) {
    /*if (line.at(0) != ':') {
        throw "Tried Parsing a Comment";
    }*/
    
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
    return line.at(i) == '#';
}

bool loginCredentialsAreCorrect(std::string enteredUsername, std::string enteredPassword, std::ifstream& file) {
    std::string line;
    while (getline(file, line)) {
        if (isComment(line)) {
            continue;
        }

        std::pair<std::string, std::string> storedUsernameAndPasswordHash = parseShadowLine(line);

        std::string storedUsername = storedUsernameAndPasswordHash.first;
        std::string storedPasswordHash = storedUsernameAndPasswordHash.second;

        if (storedUsername != enteredUsername) {
            continue;
        }

        std::string enteredPasswordHash = std::to_string(hashPassword(enteredPassword));
        if (storedPasswordHash == enteredPasswordHash) {
            return true;
        }
    }

    return false;
}

int main() {
    std::ifstream file("shadow.txt");

    std::pair<std::string, std::string> enteredCredentials = getUsernameAndPasswordFromUser();
    std::string enteredUsername = enteredCredentials.first;
    std::string enteredPassword = enteredCredentials.second;

    bool loginCorrect = loginCredentialsAreCorrect(enteredUsername, enteredPassword, file);

    std::cout << loginCorrect << std::endl;
    
    // std::cout << "Username: " << enteredCredentials.first << "\n";
    // std::cout << "Password: " << enteredCredentials.second << "\n";
    // std::cout << "Hash: " << hashPassword(enteredCredentials.second) << std::endl;

    file.close();
    return 0;
}
