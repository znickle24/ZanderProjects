//
//  main.cpp
//  ass04DiffieHellman
//
//  Created by Zander Nickle on 1/26/18.
//  Copyright © 2018 Zander Nickle. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <assert.h>
//#include <CommonCrypto/CommonDigest.h>


struct diffieHellman {
    uint64_t g;
    uint64_t n;
    uint64_t sharedSecret;
    uint64_t privateKey;
    //initializing the object (constructor)
    diffieHellman() {
        g = 0;
        n = 0;
        sharedSecret = 0;
        privateKey = 0;
    }
};
//k is the key
//n is normally a big prime number
//g^k % n
uint64_t fastExpModN(uint64_t g, uint64_t k, uint64_t n) {
    //find leftmost 1 bit and loop from there down
    uint64_t init = g;
    int index = 0;
    for (int i = 63; i >= 0; i--) {
        //look at the bit at 63 to 0 and once it equals 1, start the calculation through the end
        //you only need to start the calc once we hit a 1
        if (k >> i ==1) {
            index = i;
            break;
        }
    }
    for (int j = index; j >= 0; j--){
        if (j != index) {
            //each time we need to at least do x^2
            g *= g;
            //need to mask the values
            
            if (((k >> j)&1ul) == 1) {
                //each time that the bit value is 1, we need to do an extra multiplication by x
                g*=init;
            }
        }
        g%=n;
    }
    return g;
}
//set g and n
uint64_t computePublicKey (diffieHellman &participant, uint64_t g, uint64_t n) {
    participant.g = g;
    participant.n = n;
    participant.privateKey = arc4random();
    uint64_t pubKey = fastExpModN(g, participant.privateKey, n);
    return pubKey;
}
void computeSharedSecret (diffieHellman &participant, uint64_t pubKey) {
    participant.sharedSecret = fastExpModN(pubKey, participant.privateKey, participant.n);
}

int main(int argc, const char * argv[]) {
    struct diffieHellman alice;
    struct diffieHellman bob;
    const uint64_t g = 1907;
    const uint64_t n = 784313;
    uint64_t alicePubKey = computePublicKey (alice, g, n);
    uint64_t bobPubKey = computePublicKey(bob, g, n);
    std::cout << alicePubKey << std::endl;
    std::cout << bobPubKey << std::endl;
    computeSharedSecret(alice, bobPubKey);
    computeSharedSecret(bob, alicePubKey);
    std::cout << "Alice's shared secret: " << alice.sharedSecret << std::endl;
    std::cout << "Bob's shared secret: " << bob.sharedSecret << std::endl;
    //Bob&Alice should always have the same sharedSecret via this program
    assert(alice.sharedSecret == bob.sharedSecret);
    return 0;
}
