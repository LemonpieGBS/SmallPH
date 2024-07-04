#include <string>
#include <ctime>

#include "sha256/SHA256.h"
#include "csprng.cpp"

struct PASSWORD {
    std::string hash;
    std::string salt;
};

std::string createSalt() {
    duthomhas::csprng rng;
    double s = rng();
    srand(s);

    char all[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890-=!.,";
    int character_amount = 62;

    std::string returner;
    int amount = 32;

    for(int i = 0; i < amount; i++) {
        returner += all[rand() % character_amount];
    }

    return returner;
}

std::string genHash(std::string arg) {
    SHA256 hasher;
    hasher.update(arg);
    std::array<uint8_t, 32> digest = hasher.digest();
    return SHA256::toString(digest);
}

PASSWORD createPassword(std::string input) {
    PASSWORD return_password;
    return_password.salt = createSalt();
    return_password.hash = genHash(return_password.salt + input);

    return return_password;
}

bool validatePassword(std::string input, PASSWORD pass) {
    return (genHash(pass.salt + input) == pass.hash);
}