#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <fstream>
#include <iomanip>

#include "OpenHash.h"
#include "ClosedHash.h"

#define seed1 2005
#define seed2 045

using namespace std;

bool isPrime(long long num) {
    if (num <= 1) return false;
    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0)
        return false;

    for (long long i = 5; i * i <= num; i += 6)
        if (num % i == 0 || num % (i + 2) == 0)
            return false;

    return true;
}

long long nextPrime(long long num) {
    while (true) {
        if (isPrime(num))
            return num;
        num++;
    }
}

string generateRandomWord(long long length) {
    static const string alphabet = "abcdefghijklmnopqrstuvwxyz";
    long long alphabet_size = 26;
    string word;

    for (long long i = 0; i < length; i++) {
        long long index = rand() % alphabet_size;
        word += alphabet[index];
    }

    return word;
}

void shuffleStrings(std::string *array, long long size) {
    mt19937 rng;
    rng.seed(seed2);

    for (long long i = size - 1; i > 0; i--) {
        long long j = uniform_int_distribution<long long>(0, i)(rng);
        swap(array[i], array[j]);
    }
}

const long long N = 10000, T = 0.1 * N;

int main(){
    srand(seed1);
    string *words = new string[N];
    string *testWords = new string[N];
    long long size[3] = {5000, 10000, 20000};
    double report [3][3][2][2];

    for (long long i = 0; i < N; i++) {
        long long length = 5 + rand() % 6;
        string word = generateRandomWord(length);
        words[i] = word;
        testWords[i] = word;
    }

    shuffleStrings(testWords, N);

    long long n = 0;

    for(long long i : size){
        for(long long j = 1; j <= 2; j++) {
            OpenHash *openHash = new OpenHash(i, j);

            for(long long m = 0; m < N; m++){
                if(openHash->find(testWords[m]) == -1)
                    openHash->insert(testWords[m], openHash->getSize() + 1);
            }

            openHash->resetProbes();

            for(long long m = 0; m < T; m++)
                openHash->find(testWords[m]);

            report[n][0][j - 1][0] = openHash->getCollisions();
            report[n][0][j - 1][1] = (double) openHash->getProbes() / T;

            delete openHash;
        }

        for(long long j = 1; j <= 2; j++) {
            for (long long k = 0; k < 2; k++) {
                ClosedHash *closedHash = new ClosedHash(i, j, k);

                for(long long m = 0; m < N; m++){
                    if(closedHash->find(words[m]) == -1)
                        closedHash->insert(words[m], closedHash->getSize() + 1);
                }

                closedHash->resetProbes();

                for(long long m = 0; m < T; m++)
                    closedHash->find(testWords[m]);

                report[n][k + 1][j - 1][0] = closedHash->getCollisions();
                report[n][k + 1][j - 1][1] = (double) closedHash->getProbes() / T;

                delete closedHash;
            }
        }
        n++;
    }

    ofstream file("report.txt");

    file << " " << setw(68) << setfill('_') << "" << endl;
    file << setfill(' ');
    file << "| Hash       | Collision         | Hash Function 1 | Hash Function 2 |" << endl;
    file << "| Table Size | Resolution Method | Colls. | Probes | Colls. | Probes |" << endl;
    file << "|" << setw(69) << setfill('_') << "|" << endl;
    file << setfill(' ');

    long long i = 0;

    for(long long n : size) {
        file << "| N = " << setw(7) << left << size[i];

        for(long long j = 0; j < 3; j++) {
            if(j == 0) file << "| Separate Chaining | ";
            else if(j == 1) file << "|" << setw(34) << right << "| Double Hashing    | ";
            else file << "|" << setw(34) << right << "| Custom Probing    | ";

            for(long long k = 0; k < 2; k++) {
                for(long long m = 0; m < 2; m++) {
                    if(m == 0) file << defaultfloat << setw(5) << setprecision(5) << right << report[i][j][k][m] << "  ";
                    if(m == 1) file << fixed << setw(2) << setprecision(3) << " " << report[i][j][k][m] << "  ";
                }

                file << "| ";
            }

            file << endl;
        }

        file << "|" << setw(69) << setfill('_') << "|" << endl;
        file << setfill(' ');
        i++;
    }

    file.close();

    return 0;
}
