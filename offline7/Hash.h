#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cstring>
using namespace std;

bool isPrime(long long num);
long long nextPrime(long long num);

class Hash{
protected:
    long long N, size, collisions, probes;

    class Node{
        string key;
        long long value;
        Node* next;

    public:
        void setKey(string key) {this->key = key;}
        string getKey() {return key;}
        void setValue(long long value) {this->value = value;}
        long long getValue() {return value;}
        void setNext(Node* next) {this->next = next;}
        Node* getNext() {return next;}

        Node(string key = "", long long value = -1, Node* next = nullptr) {
            this->key = key;
            this->value = value;
            this->next = next;
        }
    };

    virtual void insert(string key, long long value) = 0;
    virtual void remove(string key) = 0;
    virtual long long find(string key) = 0;

    // Use folding on a string, summed 4 bytes at a time
    long long h1(string k) {
        char* key = (char *)k.c_str();
        unsigned int *lkey = (unsigned int *)key;
        long long intlength = k.size() / 4;
        unsigned int sum = 0;

        for(long long i = 0; i < intlength; i++)
            sum += lkey[i];

        // Now deal with the extra chars at the end
        long long extra = k.size() - intlength * 4;
        char temp[4];
        lkey = (unsigned int*)temp;
        lkey[0] = 0;

        for(long long i = 0; i < extra; i++)
            temp[i] = key[intlength * 4 + i];

        sum += lkey[0];

        return sum % N;
    }

    long long h2(string key){
        long long hashVal = 0;

        for(long long i = 0; i < key.length(); i++)
            hashVal = 37 * hashVal + key[i];

        hashVal %= N;

        if(hashVal < 0)
            hashVal += N;

        return hashVal;
    }

    long long auxHash(string key){
        long long i, sum;
        for(i = 0, sum = 0; i < key.length(); i++)
            sum += key[i];
        return sum % N;
    }

public:
    Hash(long long N) {
        this->N = N;
        size = 0;
        collisions = 0;
        probes = 1;
    };

    Hash(const Hash& other) {
        // Copy the basic members
        N = other.N;
        size = other.size;
        collisions = other.collisions;
        probes = other.probes;

        // You need to implement copying of the hashTable data if necessary
        // It depends on how your hashTable is structured
        // Make sure you deep copy any dynamically allocated memory
    }

    ~Hash() {
        N = 0;
        size = 0;
        collisions = 0;
        probes = 1;
    };

    void resetCollisions() { collisions = 0; }
    long long getCollisions() { return collisions; }
    void resetProbes() { probes = 1; }
    long long getProbes() { return probes; }
    long long getSize() { return size; }
};

#endif
