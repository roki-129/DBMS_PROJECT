#ifndef OPEN_HASH_H
#define OPEN_HASH_H

#include <iostream>
#include "Hash.h"

using namespace std;

class OpenHash : public Hash {
    Node **hashTable;
    long long hashFunction;

    void reHash(long long n);

public:
    OpenHash(long long N, long long hashFunction);
    OpenHash(const OpenHash& other);
    ~OpenHash();
    void insert(string key, long long value);
    long long find(string key);
    void remove(string key);
};

OpenHash::OpenHash(long long N, long long hashFunction) : Hash(nextPrime(N)) {
    N = nextPrime(N);
    this->hashFunction = hashFunction;
    hashTable = new Node* [N];
    for (long long i = 0; i < N; i++)
        hashTable[i] = nullptr;
}

OpenHash::OpenHash(const OpenHash& other) : Hash(other.N) {
    hashFunction = other.hashFunction;
    hashTable = new Node*[N];

    for (long long i = 0; i < N; ++i) {
        if (other.hashTable[i] != nullptr)
            hashTable[i] = new Node(*(other.hashTable[i]));
        else hashTable[i] = nullptr;
    }
}

OpenHash::~OpenHash() {
    for (long long i = 0; i < N; i++) {
        Node *temp = hashTable[i];
        while (temp != nullptr) {
            Node *temp2 = temp;
            temp = temp->getNext();
            delete temp2;
        }
    }

    delete[] hashTable;
}

void OpenHash::insert(string key, long long value) {
    if((size / N > 0.5) || (size > 100 && probes / (0.1 * N) > 2)) reHash(1);

    long long i = hashFunction == 1 ? h1(key) : h2(key);

    if (hashTable[i] == nullptr)
        hashTable[i] = new Node(key, value);
    else {
        collisions++;
        Node *temp = hashTable[i];

        while (temp->getNext() != nullptr)
            temp = temp->getNext();

        temp->setNext(new Node(key, value));
    }

    size++;
}

long long OpenHash::find(string key) {
    long long i = hashFunction == 1 ? h1(key) : h2(key);

    if (hashTable[i] == nullptr)
        return -1;

    else {
        Node *temp = hashTable[i];
        probes++;
        while (true) {
            if (temp->getKey() == key)
                return temp->getValue();

            if (temp->getNext() == nullptr)
                return -1;

            temp = temp->getNext();
            probes++;
        }
    }
}

void OpenHash::remove(string key) {
    if(size / N < 0.1) reHash(2);

    long long i = hashFunction == 1 ? h1(key) : h2(key);

    if (hashTable[i] == nullptr)
        return;

    else {
        Node *temp = hashTable[i];

        if (temp->getKey() == key) {
            hashTable[i] = temp->getNext();
            delete temp;
            size--;
            return;
        }

        while (temp->getNext() != nullptr) {
            if (temp->getNext()->getKey() == key) {
                Node *temp2 = temp->getNext();
                temp->setNext(temp->getNext()->getNext());
                delete temp2;
                size--;
                return;
            }

            temp = temp->getNext();
        }
    }
}

void OpenHash::reHash(long long n){
    long long oldN = N, oldCollisions = collisions;
    if(n == 1) N = nextPrime(N * 2);
    else if(n == 2) nextPrime(N / 2);
    Node **oldHashTable = hashTable;
    hashTable = new Node* [N];

    for (long long i = 0; i < N; i++)
        hashTable[i] = nullptr;

    for (long long i = 0; i < oldN; i++) {
        Node *temp = oldHashTable[i];
        while (temp != nullptr) {
            insert(temp->getKey(), temp->getValue());
            temp = temp->getNext();
        }
    }

    for (long long i = 0; i < oldN; i++) {
        Node *temp = oldHashTable[i];
        while (temp != nullptr) {
            Node *temp2 = temp;
            temp = temp->getNext();
            delete temp2;
        }
    }

    // collisions = oldCollisions;
    delete[] oldHashTable;
}

#endif
