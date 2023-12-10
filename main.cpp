#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include "Methods.h"
#include <ctime>


/**@brief Prompts the user to choose a source file for the program.
 * Provides options to select an existing file, write an absolute path, or use a default file.
 * Invokes the selectAction function with the chosen file path.
 */
auto setSourceFile() -> void;

/**
 * @brief Prompts the user to select an action and performs it on the specified file.
 * @param filePath
 */
auto selectAction(const std::filesystem::path& filePath) -> void;

/**@brief Reads the entries from the specified file and decrypts them.
 *
 * @param filePath The path to the file containing the entries.
 * @param gotCategory The selected category.
 * @param key The decryption key.
 * @return A vector of strings representing the decrypted entries.
 */
auto readEntries(const std::filesystem::path& filePath,const std::string& gotCategory, const std::string& key) -> std::vector<std::string>;
int main() {
    setSourceFile();
    return 0;
}
auto readEntries(const std::filesystem::path& filePath,const std::string& gotCategory, const std::string& key) -> std::vector<std::string>{
    std::ifstream file(filePath);
    std::vector<std::string> entries;
    std::string category, name, password;
    int counter = 1;
    while (std::getline(file, category) && std::getline(file, name) && std::getline(file, password)) {
        if(counter == 1){
            name = name.substr(5,name.length());
            password = password.substr(0,password.length()-5);
            std::string entry = gotCategory + ";" + decrypt(name,key) +";"+ decrypt(password,key);
            entries.push_back(entry);
            counter+=1;
        }
        else {
            std::string entry = decrypt(category,key) + ";" + decrypt(name,key) +";"+ decrypt(password,key);;
            entries.push_back(entry);
        }
    }
    return entries;
}
auto setSourceFile() -> void{
    auto path = std::filesystem::path();
    for(bool run = true; run;) {
        std::string c = "";
        std::cout << "(1)-Choose an existing file from current directory\n"
                     "(2)-Write an absolute path to the file\n"
                     "(3)-Use default file\n"
                     "(0)-Exit\n";
        std::cin >> c;
        if (c == "1") {
            path = std::filesystem::path("/Users/anastasia/Desktop/PJATK/PJC/PasswordManager/cmake-build-debug/files");
            auto files = std::vector<std::filesystem::directory_entry>();
            auto iter = std::filesystem::recursive_directory_iterator(path);
            for (auto const &entry: iter) {
                if (entry.is_regular_file() && entry.path().extension() == ".txt")
                    files.push_back(entry);
            }
            if (files.empty()) {
                std::cout << "no such file in " << path << ", try again\n\n";
                continue;
            }
            std::cout << path << ":\n";
            for(int i = 0; i < files.size(); i++){
                std::cout << i + 1 << ". " << files[i].path().string().erase(0,path.string().size()+1) << "\n";
            }
            int fileIndex;
            std::cout << "Enter the index of the file you want to choose: \n";
            std::cin >> fileIndex;

            if (fileIndex < 1 || fileIndex > files.size()) {
                std::cout << "Invalid selection, please try again.\n\n";
                continue;
            }
            path = files[fileIndex - 1].path();
        } else if (c == "2") {
            std::string absolutePath;
            std::cout << "Enter the absolute path to the file: \n";
            std::cin >> absolutePath;
            path = std::filesystem::path(absolutePath);
        } else if (c == "3") {
            path = std::filesystem::path("/Users/anastasia/Desktop/PJATK/PJC/"
                                         "PasswordManager/cmake-build-debug/files/default.txt");
        } else if (c == "0"){
            run = false;
        }
        else {
            try {
                throw std::runtime_error("Invalid input.");
            }
            catch(const std::exception& e){
                std::cerr << e.what() << std::endl;
            }
            continue;
        }
        if(c != "0") selectAction(path);
    }
}
auto selectAction(const std::filesystem::path& filePath) -> void {
    std::string a;
    auto entries = std::vector<std::string>();
    for(bool running = true; running;) {
        std::string pass;
        auto currentTime = std::chrono::system_clock::now();
        std::time_t timestamp = std::chrono::system_clock::to_time_t(currentTime);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&timestamp), "%Y@%H %m@%M %d@%S");
        std::string timestampString = ss.str();
        if(fileIsEmpty(filePath)){
            std::cout << "You should set a password for this file: \n";
            std::cin >> pass;
            if(fileIsEmpty("categories.txt")){
                std::cout << "SELECT AN ACTION\n"
                            "(3) - Add a password\n"
                            "(6) - Add a category\n"
                            "(0) - Exit\n";
                std::cin >> a;
                if(a != "3" && a != "6" && a != "0"){
                    try {
                        throw std::runtime_error("Invalid input.");
                    }
                    catch(const std::exception& e){
                        std::cerr << e.what() << std::endl;
                    }
                    continue;
                }
            }
            else{
                std::cout << "SELECT AN ACTION\n"
                             "(3) - Add a password\n"
                             "(6) - Add a category\n"
                             "(7) - Delete a category\n"
                             "(0) - Exit\n";
                std::cin >> a;
                if(a != "3" && a != "6" && a != "7" && a != "0"){
                    try {
                        throw std::runtime_error("Invalid input.");
                    }
                    catch(const std::exception& e){
                        std::cerr << e.what() << std::endl;
                    }
                    continue;
                }
            }
        }
        else{
            std::cout << "Enter a password for this file: \n";
            std::cin >> pass;
            writePassAttempt(filePath,timestampString,entries);
            std::string first;
            std::ifstream file(filePath);
            std::getline(file, first);
            first = first.substr(0,first.length()-7);
            auto decryptedFirst = decrypt(first,pass);
            auto categoryGot = unapplyCipher(decryptedFirst,pass);
            std::ifstream fileCat("categories.txt");
            std::vector<std::string> categories;
            std::string line;
            while (std::getline(fileCat, line)) {
                categories.push_back(line);
            }
            bool check = false;
            for(auto &elem : categories){
                if(elem == categoryGot) {
                    check = true;
                    break;
                }
            }
            if(!check){
                try {
                    throw std::runtime_error("Wrong password.\n");
                }
                catch(const std::exception& e){
                    std::cerr << e.what() << std::endl;
                    std::cout << "Try again?\n"
                                 "\t1 - yes\n"
                                 "\t2 - exit\n";
                }
                std::cin >> a;
                if(a == "2") running = false;
                else if (a != "1"){
                    try {
                        throw std::runtime_error("Invalid input.");
                    }
                    catch(const std::exception& e){
                        std::cerr << "Exception caught: " << e.what() << std::endl;
                    }
                }
                continue;
            }
            entries = readEntries(filePath, categoryGot, pass);
            std::cout << "SELECT AN ACTION\n";
            std::cout << "(1) - Search for a password\n"
                         "(2) - Sort passwords\n"
                         "(3) - Add a password\n"
                         "(4) - Edit a password\n"
                         "(5) - Delete a password\n"
                         "(6) - Add a category\n"
                         "(7) - Delete a category\n"
                         "(0) - Exit\n";
            std::cin >> a;
        }
        if (a == "1") {
            auto result = searchPasswords(entries);
            if (result.size() == 0) {
                std::cout << "No results found." << std::endl;
            } else {
                std::cout << "Found:" << std::endl;
                printVec(result);
            }
        }
        else if (a == "2") {
            auto result = sortPass(entries);
            std::cout << "Sorted:" << std::endl;
            printVec(result);
        }
        else if (a == "3") {
            addPassword(filePath, pass, entries,timestampString);
        }
        else if (a == "4") {
            editPassword(filePath, pass, entries,timestampString);
        }
        else if (a == "5") {
            deletePassword(entries,filePath,pass,timestampString);
        }
        else if (a == "6"){
            addCategory();
        }
        else if (a == "7")deleteCategory(entries,filePath,pass,timestampString);
        else if (a == "0") {
            running = false;
        } else {
            try {
                throw std::runtime_error("Invalid input.");
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }
    }
}
