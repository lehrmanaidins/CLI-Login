
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
            std::cout << "\rEntered Username is not Alphanumeric." << std::endl;
            return false;
        }
    }
    std::string line;
    return true;
}

std::pair<std::string, std::string> getUsernameAndPassword() {
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
    if (line.at(0) != ':') {
        throw "ValueError: Shadow File Corrupted";
    }
    
    std::size_t usernameStartPos = line.find(':', 1);
    std::size_t usernameEndPos = line.find(':', usernameStartPos + 1);
    
    std::string username = line.substr(usernameStartPos + 1, usernameEndPos - usernameStartPos - 1);
    std::string password = line.substr(usernameEndPos + 1, line.size() - username.size() - 3);

    return {username, password};
}

std::size_t hashPassword(const std::string password) {
    std::hash<std::string> hasher;
    return hasher(password);
}

int main() {
    
    std::ifstream file("shadow.txt");
    std::string line;
    getline(file, line);

    std::pair<std::string, std::string> credentials = parseShadowLine(line);
    
    std::cout << "Username: " << credentials.first << "\n";
    std::cout << "Password: " << credentials.second << "\n";
    std::cout << "Hash: " << hashPassword(credentials.second) << std::endl;

    file.close();
    return 0;
}
