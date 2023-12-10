# cpp-password-manager
# Password manager
The application is a simple password manager which keeps the reords in a txt encrypted file.

## Description

The project is written in C++ 20. It allows a user to perform the actions which are basic for a password manager application such as adding a record, finding a record etc.
The encryption works this way - it performs a simple addition operation where the ASCII value of the character in the key string is added to the ASCII value of the corresponding character in the input string. 


## Installation

The project was written in the CLion IDE, however you could use any other IDE which supports C++ 20.

If you are a macOS user and haven't worked with c++, you may need to check whether you have a gcc compiler installed by executing this in your cmd:

```bash
gcc --version

```
If this does not return a version, you may need to follow [this link](https://osxdaily.com/2023/05/02/how-install-gcc-mac/) to proceed
## Usage

Here is a brief overview of the functions provided:
```cpp

# generate a random password based on the specified criteria
auto generatePassword(int length, bool includeUppercase, bool includeLowercase, bool includeSpecialChars) -> std::string;

# checks if the given password is used in any of the password entries
auto isUsed(const std::vector<std::string>& entries, const std::string& password) -> int;

# write entries to a file with encryption and timestamp
auto writeEntries(const std::filesystem::path &path, const std::string &key, std::vector<std::string>& entries,
                  const std::string& timestamp) -> void;

# encrypts a string using a key
auto encrypt(const std::string &string, const std::string &key) -> std::string;

# decrypts an encrypted string using a key.
auto decrypt(const std::string &string, const std::string &key) -> std::string;

# add a new password entry to the password manager.
auto addPassword(const std::filesystem::path &path, const std::string &key, std::vector<std::string>& entries,
                 const std::string& timestamp) -> void;

# performs a security check on the given password and returns its security percentage
auto securityCheck(const std::string& password) -> double;

# search for passwords based on user input.
auto searchPasswords(const std::vector<std::string>& entries) -> std::vector<std::string>;

# sorts the vector of entries based on the specified criteria.
auto sortPass(const std::vector<std::string>& entries) -> std::vector<std::string>;

# edits a password entry in the specified file.
auto editPassword(const std::filesystem::path &path, const std::string &key, std::vector<std::string>& entries,
                  const std::string& timestamp) -> void;

# delete a password entry from the vector and write the updated entries to a file.
auto deletePassword(std::vector<std::string>& entries,const std::filesystem::path &path, const std::string &key,
                    const std::string& timestamp) -> void;

#  adds a new category to the list of categories.
auto addCategory() -> void;

# deletes a category and its associated entries.
auto deleteCategory(std::vector<std::string>& entries,const std::filesystem::path &path, const std::string &key,
                    const std::string& timestamp) -> void;

```

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

[MIT](https://choosealicense.com/licenses/mit/)
