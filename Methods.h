#pragma once

#include <string>
#include <filesystem>
#include <vector>


/**
 Generate a random password based on the specified criteria.
 *
 * @param length The length of the password.
 * @param includeUppercase Whether to include uppercase letters in the password.
 * @param includeLowercase Whether to include lowercase letters in the password.
 * @param includeSpecialChars Whether to include special characters in the password.
 * @return The generated random password.
 */
auto generatePassword(int length, bool includeUppercase, bool includeLowercase, bool includeSpecialChars) -> std::string;



/**
 * Checks if the given password is used in any of the password entries.
 *
 * @param entries The vector of password entries.
 * @param password The password to be checked.
 * @return The number of occurrences of the password.
 */
auto isUsed(const std::vector<std::string>& entries, const std::string& password) -> int;



/**
 * Write entries to a file with encryption and timestamp.
 * @param path The path to the output file.
 * @param key The encryption key.
 * @param entries The vector of entries to write.
 * @param timestamp The timestamp string to append.
 */
auto writeEntries(const std::filesystem::path &path, const std::string &key, std::vector<std::string>& entries,
                  const std::string& timestamp) -> void;


/**
 * Split a string into a vector of substrings based on a delimiter.
 * @param input The input string to split.
 * @param delimiter The delimiter character.
 * @return A vector of substrings.
 */
auto split(const std::string& input, char delimiter) -> std::vector<std::string>;


/**
 * @brief Encrypts a string using a key.
 *
 * @param string The string to be encrypted.
 * @param key The encryption key.
 * @return The encrypted string.
 */
auto encrypt(const std::string &string, const std::string &key) -> std::string;


/**
 @brief Decrypts an encrypted string using a key.
 *
 * @param string The encrypted string to be decrypted.
 * @param key The encryption key.
 * @return The decrypted string.
 */
auto decrypt(const std::string &string, const std::string &key) -> std::string;


/*** Reverses the cipher applied to the given category string using the provided key.
 *
 * @param cat The category string to be decrypted.
 * @param key The encryption key used for the cipher.
 * @return The decrypted category string.
 */
auto unapplyCipher(const std::string& cat, const std::string& key) -> std::string;


/**Add a new password entry to the password manager.
 *
 * @param path The path to the password file.
 * @param key The encryption key for the password file.
 * @param entries The vector of password entries.
 * @param timestamp The timestamp for the password entry.
 */
auto addPassword(const std::filesystem::path &path, const std::string &key, std::vector<std::string>& entries,
                 const std::string& timestamp) -> void;


/**
 * Check if a file is empty.
 * @param filePath The path to the file.
 * @return True if the file is empty, false otherwise.
 */
auto fileIsEmpty(const std::filesystem::path& filePath) -> bool;


/**
 * Applies a cipher to the given category string using the provided key.
 *
 * @param cat The category string to be encrypted.
 * @param key The encryption key used for the cipher.
 * @return The encrypted category string.
 */
auto applyCipher(const std::string& cat, const std::string& key) -> std::string;


/**
 * Performs a security check on the given password and returns its security percentage.
 *
 * @param password The password to be checked.
 * @return The security percentage of the password.
 */
auto securityCheck(const std::string& password) -> double;


/**Search for passwords based on user input.
 *
 * @param entries The vector of password entries.
 * @return A vector of matching password entries.
 */
auto searchPasswords(const std::vector<std::string>& entries) -> std::vector<std::string>;


/**@brief Prints the vector of entries in a specific format.
 *
 * @param vec The vector of entries to be printed.
 */
auto printVec(const std::vector<std::string>& vec) -> void;



/**
 * @brief Sorts the vector of entries based on the specified criteria.
 *
 * @param entries The vector of entries to be sorted.
 * @return The sorted vector of entries.
 */
auto sortPass(const std::vector<std::string>& entries) -> std::vector<std::string>;



/*** Edits a password entry in the specified file.
 *
 * @param path The path to the file containing the passwords.
 * @param key The encryption key used for password encryption.
 * @param entries The vector of password entries.
 * @param timestamp The timestamp of the password entries.
 */
auto editPassword(const std::filesystem::path &path, const std::string &key, std::vector<std::string>& entries,
                  const std::string& timestamp) -> void;



/**
 * Delete a password entry from the vector and write the updated entries to a file.
 * @param entries The vector of password entries.
 * @param path The path to the output file.
 * @param key The encryption key.
 * @param timestamp The timestamp string.
 */
auto deletePassword(std::vector<std::string>& entries,const std::filesystem::path &path, const std::string &key,
                    const std::string& timestamp) -> void;


/*** @brief Adds a new category to the list of categories.
 */
auto addCategory() -> void;


/**
 * @brief Deletes a category and its associated entries.
 *
 * @param entries The vector of entries.
 * @param path The file path.
 * @param key The encryption key.
 * @param timestamp The timestamp for the entries.
 */
auto deleteCategory(std::vector<std::string>& entries,const std::filesystem::path &path, const std::string &key,
                    const std::string& timestamp) -> void;


/**
 * Write password attempts to a file with modified timestamps.
 * @param path The path to the output file.
 * @param timestamp The timestamp string to modify.
 * @param entries The vector of password entries.
 */
auto writePassAttempt(const std::filesystem::path &path, const std::string& timestamp,const std::vector<std::string>& entries)-> void;