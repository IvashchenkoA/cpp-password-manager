#include "Methods.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <random>
#include <regex>
#include <algorithm>
#include <cctype>

auto encrypt(const std::string& string, const std::string& key) -> std::string {
    auto result = std::string();
    char ch = '%';
    for(int i = 0, j = 0; i < string.size(); i++, j++){
        if(j >= key.size()){
            result.push_back(string[i] + ch);
            j = 0;
        }
        else {
            result.push_back(string[i] + key[j]);
        }
    }
    return result;
}
auto decrypt(const std::string& string, const std::string& key) -> std::string {
    auto result = std::string();
    char ch = '%';
    for(int i = 0, j = 0; i < string.size(); i++, j++){
        if(j >= key.size()){
            result.push_back(string[i] - ch);
            j = 0;
        }
        else {
            result.push_back(string[i] - key[j]);
        }
    }
    return result;
}
auto printVec(const std::vector<std::string>& vec) -> void{
    for(auto &elem : vec){
        auto entry = split(elem,';');
        auto parts = split(entry[1],'.');
        if(parts[1] == "" && parts[2] == ""){
            std::cout << "Category: "<< entry[0] << "; name: " << parts[0] << "; password: " << entry[2] << std::endl;
        }
        else if(parts[1] != "" && parts[2] == ""){
            std::cout << "Category: "<< entry[0] << "; name: " << parts[0] << "; website: "
                        << parts[1] <<"; password: " << entry[2] << std::endl;
        }
        else if(parts[1] == "" && parts[2] != ""){
            std::cout << "Category: "<< entry[0] << "; name: " << parts[0] << "; login: "
                      << parts[2] <<"; password: " << entry[2] << std::endl;
        }
        else {
            std::cout << "Category: "<< entry[0] << "; name: " << parts[0] << "; website: "
                      << parts[1] <<"; login: " << parts[2] << "; password: " << entry[2] << std::endl;
        }
    }
}
auto addCategory() -> void {
    std::ifstream file("categories.txt");

    std::vector<std::string> categories;
    std::string line;

    while (std::getline(file, line)) {
        categories.push_back(line);
    }
    std::string newCat = "";
    std::cout << "Enter category:\n";
    std::cin >> newCat;
    for(auto &elem : categories){
        if(newCat == elem){
            std::cout << "Such category already exists.\n";
            return;
        }
    }
    std::ofstream resFile("categories.txt", std::ios::app);
    resFile <<"\n" << newCat;
    std::cout << "Category added\n";
}
auto deleteCategory(std::vector<std::string>& entries,const std::filesystem::path &path, const std::string &key,
                    const std::string& timestamp) -> void{
    std::cout << "Choose a category: \n";
    std::ifstream file("categories.txt");
    std::vector<std::string> toDelete;
    std::vector<std::string> categories;
    std::string line;
    std::string category;

    while (std::getline(file, line)) {
        categories.push_back(line);
    }
    for (size_t i = 0; i < categories.size(); ++i) {
        std::cout << i + 1 << ". " << categories[i] << '\n';
    }
    std::string a = "";
    std::cin >> a;
    for(auto &s : a) {
        if(!std::isdigit(s)){
            try {
                throw std::runtime_error("Invalid input.");
            }
            catch(const std::exception& e){
                std::cerr << e.what() << std::endl;
            }
            return;
        }
    }
    if(std::stoi(a) > categories.size() || std::stoi(a) <= 0){
        try {
            throw std::runtime_error("Invalid input.");
        }
        catch(const std::exception& e){
            std::cerr << e.what() << std::endl;
        }
        return;
    }
    for(size_t i = 0; i < categories.size(); i++){
        if(std::stoi(a) == i + 1) {
            category = categories[i];
            categories.erase(categories.begin()+i);
            break;
        }
    }
    for(size_t i = 0; i < entries.size(); i++){
        auto vec = split(entries[i],';');
        if(vec[0] == category){
            toDelete.push_back(entries[i]);
        }
    }
    for(size_t i = 0; i < toDelete.size(); i++){
        for(size_t j = 0; j < entries.size(); j++) {
            if (entries[j] == toDelete[i]) {
                entries.erase(entries.begin() + j);
                break;
            }
        }
    }
    std::ofstream resFile("categories.txt", std::ios::out);
    for(auto &elem : categories){
        resFile << elem << "\n";
    }
    std::cout << "Category deleted\n";
    writeEntries(path,key,entries,timestamp);
}
auto sortPass(const std::vector<std::string>& entries) -> std::vector<std::string>{
    std::string h = "";
    std::vector<std::string> sorted = entries;
    std::cout << "How do you you want to sort?\n"
                 "\t (1) - By category\n"
                 "\t (2) - By name\n"
                 "\t (3) - By website\n"
                 "\t (4) - By login\n"
                 "\t (5) - By name and category\n"
                 "\t (6) - By website and category\n"
                 "\t (7) - By login and category\n";
    std::cin >> h;
    while(h != "1" && h != "2" && h != "3" &&h != "4" && h != "5" && h != "6" && h != "7"){
        try {
            throw std::runtime_error("Invalid input.\n\tTry again:");
        }
        catch(const std::exception& e){
            std::cerr << e.what() << std::endl;
        }
        std::cin >> h;
    }
    auto comparator = [&](const std::string& entry1, const std::string& entry2) {
        std::vector<std::string> parts1 = split(entry1, ';');
        std::vector<std::string> parts2 = split(entry2, ';');
        auto small1 = split(parts1[1],'.');
        auto small2 = split(parts2[1],'.');
        if (h == "1") {
            return parts1[0] < parts2[0];
        }
        else if (h == "2") {
            return small1[0] < small2[0];
        }
        else if (h == "3"){
            return small1[1] < small2[1];
        }
        else if(h == "4"){
            return small1[2] < small2[2];
        }
        else if (h == "5") {
            if (small1[0] != small2[0]){
                return small1[0] < small2[0];
            }
            else {
                return parts1[0] < parts2[0];
            }
        }
        else if (h == "6") {
            if (small1[1] != small2[1]) {
                return small1[1] < small2[1];
            } else {
                return parts1[0] < parts2[0];
            }
        }
        else if (h == "7") {
            if (small1[2] != small2[2]){
                return small1[2] < small2[2];
            }
            else {
                return parts1[0] < parts2[0];
            }
        }
        else {
            return false;
        }
    };

    std::sort(sorted.begin(), sorted.end(), comparator);
    return sorted;
}
auto searchPasswords(const std::vector<std::string>& entries) -> std::vector<std::string>{
    std::string h = "";
    std::vector<std::string> result;
    std::cout << "Search by:\n"
                 "\t (1) - category\n"
                 "\t (2) - name\n"
                 "\t (3) - website\n"
                 "\t (4) - login\n";
    std::cin >> h;
    while(h != "1" && h != "2" && h != "3" && h != "4" ){
        try {
            throw std::runtime_error("Invalid input.\n\tTry again:");
        }
        catch(const std::exception& e){
            std::cerr << e.what() << std::endl;
        }
        std::cin >> h;
    }
    if(h == "1"){
        std::string category;
        std::cout << "Choose a category: \n";
        std::ifstream file("categories.txt");

        std::vector<std::string> categories;
        std::string line;

        while (std::getline(file, line)) {
            categories.push_back(line);
        }
        for (size_t i = 0; i < categories.size(); ++i) {
            std::cout << i + 1 << ". " << categories[i] << '\n';
        }
        std::string a;
        std::cin >> a;
        for(auto &s : a) {
            if(!std::isdigit(s)){
                return std::vector<std::string>();
            }
        }
        if(std::stoi(a) > categories.size() || std::stoi(a) <= 0){
            return std::vector<std::string>();
        }
        for(size_t i = 0; i < categories.size(); i++){
            if(std::stoi(a) == i + 1)
                category = categories[i];
        }
        for(auto &entry : entries){
            auto parts = split(entry, ';');
            if(parts[0] == category){
                result.push_back(entry);
            }
        }
    }
    else if(h == "2"){
        std::string name;
        std::cout << "Enter a name:\n";
        std::cin >> name;
        for(auto &entry : entries){
            auto parts = split(entry, ';');
            auto small = split(parts[1],'.');
            if(small[0] == name){
                result.push_back(entry);
            }
        }
    }
    else if(h == "3"){
        std::string website;
        std::cout << "Enter a website:\n";
        std::cin >> website;
        for(auto &entry : entries){
            auto parts = split(entry, ';');
            auto small = split(parts[1],'.');
            if(small[1] == website){
                result.push_back(entry);
            }
        }
    }
    else if(h == "4"){
        std::string login;
        std::cout << "Enter a login:\n";
        std::cin >> login;
        for(auto &entry : entries){
            auto parts = split(entry, ';');
            auto small = split(parts[1],'.');
            if(small[2] == login){
                result.push_back(entry);
            }
        }
    }
    return result;
}
auto addPassword(const std::filesystem::path &path, const std::string &key, std::vector<std::string>& entries,
                 const std::string& timestamp) -> void {
    if(fileIsEmpty("categories.txt")){
        std::cout << "You must create at least one category first.\n";
        return;
    }
    else{
        std::string category, password, name, website = "", login = "";
        std::cout << "Choose a category: \n";
        std::ifstream file("categories.txt");

        std::vector<std::string> categories;
        std::string line;

        while (std::getline(file, line)) {
            categories.push_back(line);
        }
        for (size_t i = 0; i < categories.size(); ++i) {
            std::cout << i + 1 << ". " << categories[i] << '\n';
        }
        std::string a = "";
        std::cin >> a;
        for(auto &s : a) {
            if(!std::isdigit(s)){
                try {
                    throw std::runtime_error("Invalid input.");
                }
                catch(const std::exception& e){
                    std::cerr << e.what() << std::endl;
                }
                return;
            }
        }
        if(std::stoi(a) > categories.size() || std::stoi(a) <= 0){
            try {
                throw std::runtime_error("Invalid input.");
            }
            catch(const std::exception& e){
                std::cerr << e.what() << std::endl;
            }
            return;
        }
        for(size_t i = 0; i < categories.size(); i++){
            if(std::stoi(a) == i + 1) {
                category = categories[i];
                break;
            }
        }
        std::cout << "Enter name of password: \n";
        std::cin >> name;
        std::cout << "Do you want to include website?(y/n)\n";
        std::cin >> a;
        while(a != "y" && a != "n"){
            try {
                throw std::runtime_error("Invalid input.\n\tTry again:");
            }
            catch(const std::exception& e){
                std::cerr << e.what() << std::endl;
            }
            std::cin >> a;
        }
        if(a == "y"){
            std::cout << "Enter website:\n";
            std::cin >> website;
        }
        std::cout << "Do you want to include login?(y/n)\n";
        std::cin >> a;
        while(a != "y" && a != "n"){
            try {
                throw std::runtime_error("Invalid input.\n\tTry again:");
            }
            catch(const std::exception& e){
                std::cerr << e.what() << std::endl;
            }
            std::cin >> a;
        }
        if(a == "y"){
            std::cout << "Enter login:\n";
            std::cin >> login;
        }
        std::cout << "Do you want to generate a random password? (y/n): ";
        std::string choice;
        std::cin >> choice;
        if (choice == "y" || choice == "Y") {
            std::string length;
            bool uppercase, lowercase , specialChars;
            std::cout << "Enter the length of the password: ";
            std::cin >> length;
            for(auto &s : length) {
                if(!std::isdigit(s)){
                    try {
                        throw std::runtime_error("Invalid input.");
                    }
                    catch(const std::exception& e){
                        std::cerr << e.what() << std::endl;
                    }
                    return;
                }
            }
            std::cout << "Include lowercase letters? (y/n): ";
            std::cin >> choice;
            if(choice != "y" && choice != "Y" && choice != "N"&& choice != "n"){
                try {
                    throw std::runtime_error("Invalid input.");
                }
                catch(const std::exception& e){
                    std::cerr << e.what() << std::endl;
                }
                return;
            }
            else if (choice == "N" || choice == "n"){
                lowercase = false;
            }
            else lowercase = true;
            std::cout << "Include uppercase letters? (y/n): ";
            std::cin >> choice;
            if(choice != "y" && choice != "Y" && choice != "N"&& choice != "n"){
                try {
                    throw std::runtime_error("Invalid input.");
                }
                catch(const std::exception& e){
                    std::cerr << e.what() << std::endl;
                }
                return;
            }
            else if (choice == "N" || choice == "n"){
                uppercase = false;
            }
            else uppercase = true;
            std::cout << "Include special characters? (y/n): ";
            std::cin >> choice;
            if(choice != "y" && choice != "Y" && choice != "N"&& choice != "n"){
                try {
                    throw std::runtime_error("Invalid input.");
                }
                catch(const std::exception& e){
                    std::cerr << e.what() << std::endl;
                }
                return;
            }
            else if (choice == "N" || choice == "n"){
                specialChars = false;
            }
            else specialChars = true;
            password = generatePassword(std::stoi(length), uppercase, lowercase,
                                        specialChars);
        } else if (choice == "n" || choice == "N") {
            std::cout << "Enter the password: ";
            std::cin >> password;
        }
        else{
            try {
                throw std::runtime_error("Invalid input.");
            }
            catch(const std::exception& e){
                std::cerr << e.what() << std::endl;
            }
            return;
        }
        int occurs = isUsed(entries, password);
        if(occurs > 0)std::cout << "This password was used " << occurs << " times.\n";
        double perc = securityCheck(password);
        std::cout << "Your password is " << perc << "% secure\n";
        std::string entry = category + ";" + name +"." + website + "." + login + ";" + password;
        entries.push_back(entry);
        writeEntries(path, key, entries,timestamp);
        std::cout<< "Password added successfully\n";
    }
}
auto generatePassword(int length, bool includeUppercase, bool includeLowercase,
                      bool includeSpecialChars) -> std::string{
    std::string chars = "1234567890";
    if (includeUppercase)
        chars += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (includeLowercase)
        chars += "abcdefghijklmnopqrstuvwxyz";
    if (includeSpecialChars)
        chars += "!@#$%^&*()";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(chars.begin(), chars.end(), gen);

    return chars.substr(0, length);
}
auto editPassword(const std::filesystem::path &path, const std::string &key, std::vector<std::string>& entries,
                  const std::string& timestamp) -> void{
    std::cout <<"Choose the password you'd like to edit. \n";
    auto found = searchPasswords(entries);
    if(found.size() == 0){
        std::cout<< "No results found.\n";
        return;
    }
    std::cout << "Found: \n";
    for(int i = 0; i < found.size(); i++){
        std::cout << i + 1 << ". " << found[i] << std::endl;
    }
    std::string c = "";
    std::cin >> c;
    for(auto &s : c) {
        if(!std::isdigit(s)){
            try {
                throw std::runtime_error("Invalid input.");
            }
            catch(const std::exception& e){
                std::cerr << e.what() << std::endl;
            }
            return;
        }
    }
    if(std::stoi(c) > found.size() || std::stoi(c) <= 0){
        try {
            throw std::runtime_error("Invalid input.");
        }
        catch(const std::exception& e){
            std::cerr << e.what() << std::endl;
        }
        return;
    }
    std::string chosen;
    std::string* entry;
    for(size_t i = 0; i < found.size(); i++){
        if(std::stoi(c) == i + 1){
            chosen = found[i];
            break;
        }
    }
    for(auto &elem : entries){
        if(elem == chosen){
            entry = &elem;
            break;
        }
    }
    std::cout << "What would you like to edit?\n"
                 "\t1 - category\n"
                 "\t2 - name\n"
                 "\t3 - password\n"
                 "\t4 - website\n"
                 "\t5 - login\n";
    std::cin >> c;
    while(c != "1" && c != "2" && c != "3" && c != "4" && c != "5") {
        try {
            throw std::runtime_error("Invalid input.\n\tTry again:");
        }
        catch(const std::exception& e){
            std::cerr << e.what() << std::endl;
        }
        std::cin >> c;
    }
    auto miniVec = split(*entry,';');
    auto partName = split(miniVec[1],'.');
    std::string edited;
    if(c == "1"){
        std::string cat;
        std::cout << "Choose a new category: \n";
        std::ifstream file("categories.txt");

        std::vector<std::string> categories;
        std::string line;

        while (std::getline(file, line)) {
            categories.push_back(line);
        }
        for (size_t i = 0; i < categories.size(); ++i) {
            std::cout << i + 1 << ". " << categories[i] << '\n';
        }
        std::cin >> c;
        for(auto &s : c) {
            if(!std::isdigit(s)){
                try {
                    throw std::runtime_error("Invalid input.");
                }
                catch(const std::exception& e){
                    std::cerr << e.what() << std::endl;
                }
                return;
            }
        }
        if(std::stoi(c) > categories.size() || std::stoi(c) <= 0){
            try {
                throw std::runtime_error("Invalid input.");
            }
            catch(const std::exception& e){
                std::cerr << e.what() << std::endl;
            }
            return;
        }
        for(size_t i = 0; i < categories.size(); i++){
            if(std::stoi(c) == i + 1) {
                cat = categories[i];
                break;
            }
        }
        edited = cat + ";" + miniVec[1] + ";" + miniVec[2];
    }
    else if (c == "2"){
        std::string name;
        std::cout << "Enter a new name:\n";
        std::cin >> name;
        partName.at(0) = name;
        edited = miniVec[0] + ";" + partName[0]+ "." + partName[1] + "." + partName[2] + ";" + miniVec[2];
    }
    else if(c == "3"){
        std::string password;
        std::cout << "Do you want to generate a random password? (y/n): ";
        std::string choice;
        std::cin >> choice;
        if (choice == "y" || choice == "Y") {
            std::string length;
            bool uppercase = false, lowercase = false , specialChars = false;
            std::cout << "Enter the length of the password: ";
            std::cin >> length;
            for(auto &s : length) {
                if(!std::isdigit(s)){
                    try {
                        throw std::runtime_error("Invalid input.");
                    }
                    catch(const std::exception& e){
                        std::cerr << e.what() << std::endl;
                    }
                    return;
                }
            }
            std::cout << "Include lowercase letters? (y/n): ";
            std::cin >> choice;
            if(choice != "y" && choice != "Y" && choice != "N"&& choice != "n"){
                try {
                    throw std::runtime_error("Invalid input.");
                }
                catch(const std::exception& e){
                    std::cerr << e.what() << std::endl;
                }
                return;
            }
            if (choice == "y" || choice == "Y"){
                lowercase = true;
            }
            std::cout << "Include uppercase letters? (y/n): ";
            std::cin >> choice;
            if(choice != "y" && choice != "Y" && choice != "N"&& choice != "n"){
                try {
                    throw std::runtime_error("Invalid input.");
                }
                catch(const std::exception& e){
                    std::cerr << e.what() << std::endl;
                }
                return;
            }
            if (choice == "Y" || choice == "y"){
                uppercase = true;
            }
            std::cout << "Include special characters? (y/n): ";
            std::cin >> choice;
            if(choice != "y" && choice != "Y" && choice != "N"&& choice != "n"){
                try {
                    throw std::runtime_error("Invalid input.");
                }
                catch(const std::exception& e){
                    std::cerr << e.what() << std::endl;
                }
                return;
            }
            if (choice == "Y" || choice == "y"){
                specialChars = true;
            }
            password = generatePassword(std::stoi(length), uppercase, lowercase,
                                        specialChars);
        } else if (choice == "n" || choice == "N") {
            std::cout << "Enter the password: ";
            std::cin >> password;
        }
        else{
            try {
                throw std::runtime_error("Invalid input.");
            }
            catch(const std::exception& e){
                std::cerr << e.what() << std::endl;
            }
            return;
        }
        int occurs = isUsed(entries, password);
        if(occurs > 0)std::cout << "This password was used " << occurs << " times.\n";
        double perc = securityCheck(password);
        std::cout << "Your password is " << perc << "% secure\n";
        edited = miniVec[0] + ";" + miniVec[1] + ";" + password;
    }
    else if(c == "4"){
        std::string website;
        std::cout << "Enter a new website:\n";
        std::cin >> website;
        partName.at(1) = website;
        edited = miniVec[0] + ";" + partName[0]+ "." + partName[1] + "." + partName[2] + ";" + miniVec[2];
    }
    else if(c == "5"){
        std::string login;
        std::cout << "Enter a new login:\n";
        std::cin >> login;
        partName.at(2) = login;
        edited = miniVec[0] + ";" + partName[0]+ "." + partName[1] + "." + partName[2] + ";" + miniVec[2];
    }
    *entry = edited;
    writeEntries(path,key,entries,timestamp);
    std::cout<< "Password edited successfully\n";
}
auto applyCipher(const std::string& cat, const std::string& key) -> std::string{
    std::string cutKey = key;
    std::string ciphCat = cat;
    while(cutKey.length() > ciphCat.length()){
        for(size_t i = 0; i < cutKey.length()/2 - 1;i++){
            cutKey[i] += cutKey[cutKey.length() - 1 - i];
            cutKey.erase(cutKey.length() - 1 - i);
        }
    }
    for(size_t i = 0, j = 0; i < ciphCat.length() - 1; i++, j++){
        if(j >= cutKey.length())j = 0;
        ciphCat[i] += cutKey[j];
    }
    return ciphCat;
}
auto unapplyCipher(const std::string& cat, const std::string& key) -> std::string{
    std::string cutKey = key;
    std::string ciphCat = cat;
    while(cutKey.length() > ciphCat.length()){
        for(size_t i = 0; i < cutKey.length()/2 - 1;i++){
            cutKey[i] += cutKey[cutKey.length() - 1 - i];
            cutKey.erase(cutKey.length() - 1 - i);
        }
    }
    for(size_t i = 0, j = 0; i < ciphCat.length() - 1; i++, j++){
        if(j >= cutKey.length())j = 0;
        ciphCat[i] -= cutKey[j];
    }
    return ciphCat;
}
auto isUsed(const std::vector<std::string>& entries, const std::string& password) -> int {
    auto count = std::ranges::count_if(entries, [&password](const std::string& entry) {
        auto parts = split(entry, ';');
        if (parts.size() >= 3) {
            return parts[2] == password;
        }
        return false;
    });
    return count;
}
auto securityCheck(const std::string& password) -> double{
    std::regex uppercaseRegex("[A-Z]");
    std::regex lowercaseRegex("[a-z]");
    std::regex digitRegex("[0-9]");
    std::regex specialCharRegex("[^A-Za-z0-9]");
    bool hasUppercase = std::regex_search(password, uppercaseRegex);
    bool hasLowercase = std::regex_search(password, lowercaseRegex);
    bool hasDigit = std::regex_search(password, digitRegex);
    bool hasSpecialChar = std::regex_search(password, specialCharRegex);

    int numCharacterTypes = hasUppercase + hasLowercase + hasDigit + hasSpecialChar;

    double securityPercentage = (static_cast<double>(numCharacterTypes) / 4.0) * 100.0;
    return securityPercentage;
}
auto fileIsEmpty(const std::filesystem::path& filePath) -> bool{
    std::ifstream file(filePath.string());
    return file.peek() == std::ifstream::traits_type::eof();
}
auto split(const std::string& input, char delimiter) -> std::vector<std::string> {
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        result.push_back(token);
    }
    return result;
}
auto writeEntries(const std::filesystem::path &path, const std::string &key, std::vector<std::string>& entries,
                  const std::string& timestamp) -> void{
    std::ofstream file(path, std::ios::out);
    auto splitted = split(timestamp, ' ');
    for(int i = 0; i < entries.size(); i++){
        auto vec = split(entries[i],';');
        for(int j = 0; j < vec.size(); j++){
            if(i == 0){
                if(j == 0) {
                    file << encrypt(applyCipher(vec[j], key), key) <<splitted[0]<< "\n";
                }
                else if(j == 1){
                    file << splitted[1] << encrypt(vec[j],key)<<"\n";
                }
                else if(j == 2){
                    file << encrypt(vec[j],key) << splitted[2] << "\n";
                }
            }
            else file << encrypt(vec[j],key) <<"\n";
        }
    }
}
auto deletePassword(std::vector<std::string>& entries,const std::filesystem::path &path, const std::string &key,
                    const std::string& timestamp) -> void {
    auto chosen = std::vector<std::string>();
    for (bool running = true; running;) {
        std::cout << "Choose the password you'd like to delete. \n";
        auto found = searchPasswords(entries);
        if (found.size() == 0) {
            std::cout << "No results found.\n";
            return;
        }
        std::cout << "Found: \n";
        for (int i = 0; i < found.size(); i++) {
            std::cout << i + 1 << ". " << found[i] << std::endl;
        }
        std::string c = "";
        std::cin >> c;
        for(auto &s : c) {
            if(!std::isdigit(s)){
                try {
                    throw std::runtime_error("Invalid input.");
                }
                catch(const std::exception& e){
                    std::cerr << e.what() << std::endl;
                }
                return;
            }
        }
        if(std::stoi(c) > found.size() || std::stoi(c) <= 0){
            try {
                throw std::runtime_error("Invalid input.");
            }
            catch(const std::exception& e){
                std::cerr << e.what() << std::endl;
            }
            return;
        }
        for (size_t i = 0; i < found.size(); i++) {
            if (std::stoi(c) == i + 1) {
                chosen.push_back(found[i]);
                break;
            }
        }
        std::cout<< "Choose one more password?\n"
                    "\t(1) - yes\n"
                    "\t(2) - no\n";
        std::cin>>c;
        while(c != "1" && c != "2"){
            try {
                throw std::runtime_error("Invalid input.\n\tTry again:");
            }
            catch(const std::exception& e){
                std::cerr << e.what() << std::endl;
            }
            std::cin >> c;
        }
        if(c == "2")running = false;
    }
    for(size_t i = 0; i < chosen.size(); i++){
        for(size_t j = 0; j < entries.size(); j++) {
            if (entries[j] == chosen[i]) {
                entries.erase(entries.begin() + j);
                break;
            }
        }
    }
    writeEntries(path,key,entries,timestamp);
    std::cout<<chosen.size() << " password(s) deleted successfully.\n";
}
auto writePassAttempt(const std::filesystem::path &path, const std::string& timestamp,const std::vector<std::string>& entries)-> void {
    std::ifstream file(path);
    std::string category, name, password;
    std::vector<std::string> toWrite;
    int counter = 1;
    auto splitted = split(timestamp,' ');
    while (std::getline(file, category) && std::getline(file, name) && std::getline(file, password)) {
        if(counter == 1){
            category = category.substr(0,category.length()-7);
            name = name.substr(5,name.length());
            password = password.substr(0,password.length()-5);
            toWrite.push_back(category + splitted[0]);
            toWrite.push_back(splitted[1] + name);
            toWrite.push_back(password + splitted[2]);
            counter+=1;
        }
        else {
            toWrite.push_back(category);
            toWrite.push_back(name);
            toWrite.push_back(password);
        }
    }
    std::ofstream fileIn(path,std::ios::out);
    for(auto &elem : toWrite){
        fileIn << elem << "\n";
    }
}