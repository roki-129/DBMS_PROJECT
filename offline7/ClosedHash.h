#ifndef CLOSED_HASH_H
#define CLOSED_HASH_H

#include <iostream>
#include "Hash.h"

using namespace std;

class ClosedHash : public Hash{
    enum ProbingType {DOUBLE, CUSTOM};
    long long probingType, hashFunction;
    const long long c1 = 37, c2 = 41;
    Node *hashTable;

    long long quadraticProbedHash(string key, long long i);
    long long doubleHash(string key, long long i);
    void reHash(long long n);

public:
    ClosedHash(long long N, long long hashFunction, long long probingType);
    ClosedHash(const ClosedHash& other);
    ~ClosedHash();
    void insert(string key, long long value);
    long long find(string key);
    void remove(string key);
};

ClosedHash::ClosedHash(long long N, long long hashFunction, long long probingType) : Hash(nextPrime(N)) {
    N = nextPrime(N);
    this->hashFunction = hashFunction;
    this->probingType = probingType;
    hashTable = new Node[N];
}

ClosedHash::ClosedHash(const ClosedHash& other) : Hash(other.N) {
    hashFunction = other.hashFunction;
    probingType = other.probingType;

    hashTable = new Node[N];
    for (long long i = 0; i < N; ++i) {
        hashTable[i] = other.hashTable[i];
    }
}

ClosedHash::~ClosedHash() {
    delete[] hashTable;
}

void ClosedHash::insert(string key, long long value) {
    if((size / N > 0.5) || (size > 100 && probes / (0.1 * N) > 2)) reHash(1);

    long long i = 0, h, prev, offset = 0;

    switch(probingType){
        case DOUBLE: h = doubleHash(key, i);
        case CUSTOM: h = quadraticProbedHash(key, i);
    }

    while (hashTable[h].getKey() != "") {
        prev = h;

        switch(probingType){
            case DOUBLE: h = (offset + doubleHash(key, ++i)) % N;
            case CUSTOM: h = (offset + quadraticProbedHash(key, ++i)) % N;
        }

        if(h == prev) {
            offset++;
            i = 0;
        }

        collisions++;
    }

    hashTable[h] = Node(key, value);

    size++;
}

long long ClosedHash::find(string key) {
    long long i = 0, h, prev, offset = 0;

    switch(probingType){
        case DOUBLE: h = doubleHash(key, i);
        case CUSTOM: h = quadraticProbedHash(key, i);
    }

    while (hashTable[h].getKey() != "" && i < N) {
        prev = h;
        if (hashTable[h].getKey() == key) {
            probes += 1;
            return hashTable[h].getValue();
        }

        switch(probingType){
            case DOUBLE: h = (offset + doubleHash(key, ++i)) % N;
            case CUSTOM: h = (offset + quadraticProbedHash(key, ++i)) % N;
        }

        if(h == prev) {
            offset++;
            i = 0;
        }
        else probes++;
    }

    return -1;
}

void ClosedHash::remove(string key) {
    if(size / N < 0.1) reHash(2);

    long long i = 0, h, prev, offset = 0;

    switch(probingType){
        case DOUBLE: h = doubleHash(key, i);
        case CUSTOM: h = quadraticProbedHash(key, i);
    }

    while (hashTable[h].getKey() != "" && i < N) {
        prev = h;
        if (hashTable[h].getKey() == key) {
            hashTable[h] = Node("", -1);
            size--;
            return;
        }

        switch(probingType){
            case CUSTOM: h = (offset + quadraticProbedHash(key, ++i)) % N;
            case DOUBLE: h = (offset + doubleHash(key, ++i)) % N;
        }

        if(h == prev) {
            offset++;
            i = 0;
        }
        else probes++;
    }
}

void ClosedHash::reHash(long long n) {
    long long oldN = N, oldCollisions = collisions;
    if(n == 1) N = nextPrime(N * 2);
    else if(n == 2) nextPrime(N / 2);
    Node *oldHashTable = hashTable;
    hashTable = new Node[N];
    size = 0;

    for (long long i = 0; i < oldN; i++)
        if (oldHashTable[i].getKey() != "")
            insert(oldHashTable[i].getKey(), oldHashTable[i].getValue());

    // collisions = oldCollisions;
    delete[] oldHashTable;
}

long long ClosedHash::quadraticProbedHash(string key, long long i) {
    return ((hashFunction == 1 ? h1(key) : h2(key)) + c1 * i * auxHash(key) + c2 * i * i) % N;
}


long long ClosedHash::doubleHash(string key, long long i) {
    return ((hashFunction == 1 ? h1(key) : h2(key)) + i * auxHash(key)) % N;
}

#endif
