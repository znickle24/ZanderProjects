//
//  blockCypher.cpp
//  ass1BlockCypher
//
//  Created by Zander Nickle on 1/12/18.
//  Copyright © 2018 Zander Nickle. All rights reserved.
//

#include <iostream>
#include <array>
#include <vector>

using subTable = std::array<uint8_t, 256>;
using keys = std::array<uint8_t, 8>;

std::array<subTable, 8> tables;
std::array<subTable, 8> inverseTables;
subTable sub;

keys computeSectretKey (std::string password) {
    keys key;
    for (int i = 0; i < 8; i++) {
        key[i] = 0;
    }
    for (int i = 0; i < password.length(); i++) {
        key[i%8] = key[i%8] ^ password[i];
    }
    return key;
}
subTable shuffleTable(subTable sub) {
    for (int i = 0; i < sub.size()-2; i++) {
        int j = rand()%sub.size();
        std::swap(sub[i], sub[j]);
    }
    return sub;
}
//could do a lookup from the subtable
uint8_t byteSub (const uint8_t val, subTable table) {
    return table[val];
}

std::vector<uint8_t> encrypt(keys key, std::vector<uint8_t> message) {
    //do the rest 16 times per assignement
    for (int j = 0; j < 16; j++) {
    //xoring the message with the key
        for (int i = 0; i < message.size(); i++) {
            message[i] = key[i%8]^message[i];
        }

        for (int i = 0; i < 8; i ++) {
            message[i] = byteSub(message[i], tables[i]);
        }

        //shift this baby left 1
        uint8_t tempByte = message[0];
        for (int i = 0; i < message.size()-1; i++) {
            uint8_t leftmost = message[i+1] >> 7;
            message[i] = message[i] << 1;
            message[i] = message[i] | leftmost;
        }
        //manually putting the first bit at the end;
        tempByte = tempByte >> 7;
        message[message.size()-1] = message[message.size()-1] << 1;
        message[message.size()-1] = message[message.size()-1] | tempByte;
    }
    return message;
}
std::vector<uint8_t> decrypt(keys key, std::vector<uint8_t> message) {
    //running 16 times per assignment
    for (int i = 0; i < 16; i++) {
        //rotate right -- need to start from the right and go backwards
        uint8_t tempByte = message[message.size()-1];
        for (long j = message.size()-1; j > 0; j--) {
            uint8_t rightMost = message[j-1] << 7;
            message[j] = message[j] >> 1;
            message[j] = message[j] | rightMost;
        }
        //manually putting the first bit at the end;
        tempByte = tempByte << 7;
        message[0] = message[0] >> 1;
        message[0] = message[0] | tempByte;

        //sub through table
        for (int j = 0; j < message.size(); j++) {
            message[j] = byteSub(message[j], inverseTables[j]);
        }

        //apply the key
        for (int j = 0; j < message.size(); j++) {
            message[j] = key[j%8]^message[j];
        }
    }
    return message;
}
int main(int argc, const char * argv[]) {
    //testing the compute secret key method
    std::string password = "password";
    //needed to pass into the encryption function
    std::array<uint8_t, 8> key = computeSectretKey(password);
//    for (int i = 0; i < key.size(); i++) {
//        std::bitset<8> keyBits(key[i]);
//        std::cout << keyBits << std::endl;
//    }
    //converting the original message to a vector of unsigned ints
    std::string originalMessage = "absolute";
    std::vector<uint8_t> message;
    for (int i = 0; i < originalMessage.size(); i++) {
        message.push_back(originalMessage[i]);
    }
    //initializing the sub to the identity
    for (int i = 0; i < 256; i++) {
        sub[i] = i;
    }
    //setting the original table and the inverse tables
    subTable invTable;
    for (int i = 0; i < 8; i++) {
        sub = shuffleTable(sub);
        for (int j = 0; j < 256; j++) {
            invTable[sub[j]] = j;
        }
        inverseTables[i] = invTable;
        tables[i] = sub;
    }
    //setting message to the encrypted message
    message = encrypt(key, message);
    for (char c: message) {
        std::cout << c ;
    }
    std::cout << "\n";
    message = decrypt(key, message);
    for (char c : message) {
        std::cout << c ;
    }
    std::cout << "\n";
    return 0;
}
