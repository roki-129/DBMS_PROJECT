#include <bits/stdc++.h>

using namespace std;

class Word_generator {
public:
    vector<string> words;
    Word_generator(int n) {
        for (int i = 0; i < n; i++) {
            words.push_back(genRandWord());
        }
    }
    string genRandWord() {
        int wordLength = 5 + rand() % 6;
        string word;

        for (int i = 0; i < wordLength; i++) {
            char randomChar = 'a' + rand() % 26;
            word += randomChar;
        }

        return word;
    }
};

class PrimeListing {
    int n;
    vector<int> prime;

public:
    PrimeListing(int n) {
        this->n = n;
        primeList();
    }
    void primeList() {
        vector<bool> isPrime(n + 1, true);
        isPrime[0] = isPrime[1] = false;

        for (int i = 2; i * i <= n; i++) {
            if (isPrime[i]) {
                for (int j = 2; j <= n / i; j++) {
                    isPrime[i * j] = false;
                }
            }
        }
        for (int i = 2; i <= n; i++) {
            if (isPrime[i]) {
                prime.push_back(i);
            }
        }
    }

    int nextPrime(int x) {
        for (int i = 0; i <= prime.size(); i++) {
            if (prime[i] >= x) {
                x = prime[i];
                break;
            }
        }
        return x;
    }
};

enum status
{
    ACTIVE,
    DELETED,
    EMPTY
};

class HashNode
{
public:
    string key;
    int value;
    status stat;
    HashNode()
    {
        this->key = "";
        this->value = 0;
        this->stat = EMPTY;
    }
    HashNode(string key, int value, status stat)
    {
        this->key = key;
        this->value = value;
        this->stat = stat;
    }
};
PrimeListing primes(1e7); //nextPrime

enum resolutionType
{
    SEPARATE_CHAIN,
    DOUBLE_HASHING,
    CUSTOM_PROBING
};

class Hash
{
    resolutionType rtype;
    vector<vector<HashNode>> hashTable;
    HashNode defaultObj;
    vector<HashNode> hashes;
    int C1 = 37, C2 = 47;
    int base = 37;
    bool functionType;
    int maxChainLength;
    int insertionCount;
    int deletionCount;

    int hash1(string key)
    { //polynomial rolling
        int hashValue = 0;
        int p = 1;
        for (char ch : key)
        {
            hashValue = ((hashValue + (ch - 'a' + 1)) * p) % hashTable.size();
            p = (p * base) % hashTable.size();
        }
        return hashValue;
    }

    int hash2(string key)
    { //djb2
        int hashValue = 5381;
        for (char c : key)
            hashValue = (((hashValue << 5) + hashValue) + c) % hashTable.size();

        return hashValue;
    }

    int hash1_2(string key)
    {
        if (!functionType)
            return hash1(key);
        else
            return hash2(key);
    }
    int aux_Hash(string key)
    {
        int auxHash = 0;
        for (char c : key)
            auxHash += c - 'a';
        return auxHash % hashTable.size();
    }

    void rehash(bool useFirstHash)
    {
        int oldCapacity = capacity;
        capacity = useFirstHash ? primes.nextPrime(capacity * 2) : primes.nextPrime(capacity / 2);
        vector<vector<HashNode>> newTable(capacity, hashes);

        for (auto &bucket : hashTable)
        {
            for (auto &node : bucket)
            {
                if (node.stat == ACTIVE)
                {
                    int hash = useFirstHash ? hash1_2(node.key) : aux_Hash(node.key);
                    int curHash = hash;
                    int i = 1;

                    while (newTable[curHash][0].stat != EMPTY)
                    {
                        if (rtype == DOUBLE_HASHING)
                            curHash = (hash + i * aux_Hash(node.key)) % capacity;
                        else
                            curHash = (hash + C1 * i * aux_Hash(node.key) + C2 * i * i) % capacity;
                        i++;
                    }

                    newTable[curHash][0] = node;
                }
            }
        }

        hashTable = move(newTable);
        maxChainLength = 0;
        insertionCount = 0;
        deletionCount = 0;
    }

public:
    int curSize, capacity, countProbes, countCollision;
    Hash(int size, resolutionType type, bool functionType, int maxChainLength) : maxChainLength(maxChainLength), insertionCount(0), deletionCount(0)
    {
        this->rtype = type;
        hashes.push_back(defaultObj);
        capacity = primes.nextPrime(size);
        this->hashTable.resize(capacity);
        for (int i = 0; i < hashTable.size(); i++)
        {
            hashTable[i] = hashes;
        }
        curSize = countProbes = countCollision = 0;
        this->functionType = functionType;
    }

    bool Insert(string key, int value)
    {
        if (findKey(key) != -1)
        {
            return false;
        }
        int hash = hash1_2(key);
        int curhash = hash;
        int auxHash = aux_Hash(key);
        HashNode obj(key, value, ACTIVE);
        if (rtype == SEPARATE_CHAIN) //separate chaining
        {
            if (hashTable[curhash].front().stat == EMPTY)
            {
                hashTable[curhash].front() = obj;
            }
            else
            {
                countCollision++;
                hashTable[curhash].push_back(obj);
            }
            curSize++;
            insertionCount++;

            if (insertionCount % 100 == 0)
            {
                // Check max chain length and rehash if needed
                int maxChain = 0;
                for (const auto &bucket : hashTable)
                {
                    maxChain = max(maxChain, static_cast<int>(bucket.size()));
                }

                if (maxChain > maxChainLength)
                {
                    double loadFactor = static_cast<double>(curSize) / capacity;
                    double avgProbes = calculateAverageProbes(0.1 * curSize);
                    cout << "Before rehash (Insert): "
                         << "Average Probes: " << avgProbes << ", Load Factor: " << loadFactor << ", Max Chain Length: " << maxChain << endl;

                    rehash(true);

                    loadFactor = static_cast<double>(curSize) / capacity;
                    avgProbes = calculateAverageProbes(0.1 * curSize);
                    cout << "After rehash (Insert): "
                         << "Average Probes: " << avgProbes << ", Load Factor: " << loadFactor << ", Max Chain Length: " << maxChain << endl;
                }
            }

            return true;
        }
        else //double hashing or custom hashing
        {
            int i = 1;
            while (true)
            {
                if (hashTable[curhash].front().stat == EMPTY)
                {
                    hashTable[curhash].front() = obj;
                    curSize++;
                    insertionCount++;

                    if (insertionCount % 100 == 0)
                    {
                        // Check max chain length and rehash if needed
                        int maxChain = 0;
                        for (const auto &bucket : hashTable)
                        {
                            maxChain = max(maxChain, static_cast<int>(bucket.size()));
                        }

                        if (maxChain > maxChainLength)
                        {
                            double loadFactor = static_cast<double>(curSize) / capacity;
                            double avgProbes = calculateAverageProbes(0.1 * curSize);
                            cout << "Before rehash (Insert): "
                                 << "Average Probes: " << avgProbes << ", Load Factor: " << loadFactor << ", Max Chain Length: " << maxChain << endl;

                            rehash(true);

                            loadFactor = static_cast<double>(curSize) / capacity;
                            avgProbes = calculateAverageProbes(0.1 * curSize);
                            cout << "After rehash (Insert): "
                                 << "Average Probes: " << avgProbes << ", Load Factor: " << loadFactor << ", Max Chain Length: " << maxChain << endl;
                        }
                    }

                    return true;
                }
                else
                {
                    countCollision++;
                    if (rtype == DOUBLE_HASHING)
                        curhash = (hash + i * auxHash) % hashTable.size();
                    else
                        curhash = (hash + C1 * i * auxHash + C2 * i * i) % hashTable.size();
                    i++;
                    if (i > curSize)
                    {
                        return false;
                    }
                }
            }
        }
    }

    int findKey(string key)
    {
        int hash = hash1_2(key), auxHash = aux_Hash(key);
        int currentHash = hash;
        int i = 1, j = 0;
        if (rtype == SEPARATE_CHAIN)
        {
            if (hashTable[currentHash].front().stat == EMPTY)
            {
                countProbes++;
                return -1;
            }
            else
            {
                for (int j = 0; j < hashTable[currentHash].size(); j++)
                {
                    countProbes++;
                    if (hashTable[currentHash][j].key == key)
                    {
                        return currentHash;
                    }
                }
                return -1;
            }
        }

        else
        {
            while (true)
            {
                if (hashTable[currentHash][0].stat == EMPTY)
                {
                    countProbes++;
                    return -1;
                }
                else if (hashTable[currentHash][0].stat == ACTIVE)
                {
                    if (hashTable[currentHash].front().key == key)
                    {
                        countProbes++;
                        return currentHash;
                    }
                    else
                    {
                        countProbes++;
                        if (rtype == DOUBLE_HASHING)
                        {
                            currentHash = (hash + i * auxHash) % hashTable.size();
                        }
                        else
                        {
                            currentHash = (hash + C1 * i * auxHash + C2 * i * i) % hashTable.size();
                        }
                        i++;
                        if (i > capacity)
                        {
                            return -1;
                        }
                    }
                }
            }
        }
    }

    int deleteKey(string key)
    {
        int hash = findKey(key);
        if (hash == -1)
        {
            return -1;
        }
        if (rtype == SEPARATE_CHAIN)
        {
            for (auto it = hashTable[hash].begin(); it != hashTable[hash].end(); it++)
            {
                if (it->key == key)
                {
                    int value = it->value;
                    it = hashTable[hash].erase(it);
                    curSize--;
                    deletionCount++;

                    if (deletionCount % 100 == 0)
                    {
                        // Check max chain length and rehash if needed
                        int maxChain = 0;
                        for (const auto &bucket : hashTable)
                        {
                            maxChain = max(maxChain, static_cast<int>(bucket.size()));
                        }

                        double loadFactor = static_cast<double>(curSize) / capacity;
                        double avgProbes = calculateAverageProbes(0.1 * curSize);
                        cout << "Before rehash (Delete): "
                             << "Average Probes: " << avgProbes << ", Load Factor: " << loadFactor << ", Max Chain Length: " << maxChain << endl;

                        rehash(false);

                        loadFactor = static_cast<double>(curSize) / capacity;
                        avgProbes = calculateAverageProbes(0.1 * curSize);
                        cout << "After rehash (Delete): "
                             << "Average Probes: " << avgProbes << ", Load Factor: " << loadFactor << ", Max Chain Length: " << maxChain << endl;
                    }

                    return value;
                }
            }
            return -1;
        }
        else
        {
            hashTable[hash].front().stat = DELETED;
            curSize--;
            deletionCount++;

            if (deletionCount % 100 == 0)
            {
                // Check max chain length and rehash if needed
                int maxChain = 0;
                for (const auto &bucket : hashTable)
                {
                    maxChain = max(maxChain, static_cast<int>(bucket.size()));
                }

                double loadFactor = static_cast<double>(curSize) / capacity;
                double avgProbes = calculateAverageProbes(0.1 * curSize);
                cout << "Before rehash (Delete): "
                     << "Average Probes: " << avgProbes << ", Load Factor: " << loadFactor << ", Max Chain Length: " << maxChain << endl;

                rehash(false);

                loadFactor = static_cast<double>(curSize) / capacity;
                avgProbes = calculateAverageProbes(0.1 * curSize);
                cout << "After rehash (Delete): "
                     << "Average Probes: " << avgProbes << ", Load Factor: " << loadFactor << ", Max Chain Length: " << maxChain << endl;
            }

            return hashTable[hash].front().value;
        }

        if (rtype == SEPARATE_CHAIN)
        {
            int at = 0;
            vector<HashNode> temp;
            while (at < hashTable[hash].size())
            {
                if (hashTable[hash][at].key != key)
                    temp.push_back(hashTable[hash][at]);
                else
                    curSize--;
            }
            hashTable[hash].clear();
            hashTable[hash] = temp;
        }
    }

    double calculateAverageProbes(int numProbes)
    {
        double totalProbes = 0;
        for (int i = 0; i < numProbes; ++i)
        {
            string key = hashes[rand() % curSize].key;
            findKey(key); // Increment countProbes during search
            totalProbes += countProbes;
        }
        countProbes = 0; // Reset countProbes after calculation
        return totalProbes / numProbes;
    }
};

int main()
{
    int M = 10000, sz[] = {5000, 10000, 20000};
    int search = 1000;
    int maxChainLength = 50; // Set your desired max chain length
    vector<string> searchWords;
    vector<string> insertWords;
    srand(29);
    Word_generator randWord(M);

    for (int N : sz)
    {
        cout << "Hash Table Size: " << primes.nextPrime(N) << endl;

        Hash Table1_S(N, SEPARATE_CHAIN, false, maxChainLength);
        Hash Table1_D(N, DOUBLE_HASHING, false, maxChainLength);
        Hash Table1_C(N, CUSTOM_PROBING, false, maxChainLength);

        Hash Table2_S(N, SEPARATE_CHAIN, true, maxChainLength);
        Hash Table2_D(N, DOUBLE_HASHING, true, maxChainLength);
        Hash Table2_C(N, CUSTOM_PROBING, true, maxChainLength);

        if (N > M)
        {
            N = M;
        }
        for (int i = 0; i < N; i++)
        {
            Table1_S.Insert(randWord.words[i], i);
            Table1_D.Insert(randWord.words[i], i);
            Table1_C.Insert(randWord.words[i], i);
            Table2_S.Insert(randWord.words[i], i);
            Table2_D.Insert(randWord.words[i], i);
            Table2_C.Insert(randWord.words[i], i);
        }

        Table1_S.countProbes = 0;
        Table1_D.countProbes = 0;
        Table1_C.countProbes = 0;
        Table2_S.countProbes = 0;
        Table2_D.countProbes = 0;
        Table2_C.countProbes = 0;

        int j = 0;
        while (j < search)
        {
            searchWords.push_back(randWord.words[rand() % N]);
            Table1_S.findKey(searchWords[j]);
            Table1_D.findKey(searchWords[j]);
            Table1_C.findKey(searchWords[j]);
            Table2_S.findKey(searchWords[j]);
            Table2_D.findKey(searchWords[j]);
            Table2_C.findKey(searchWords[j]);
            j++;
        }
        searchWords.clear();
        cout << "hash1: " << endl
             << endl;

        cout << "# of Collisions for Chaining " << Table1_S.countCollision << endl;

        cout << "# of Collisions for Double Hashing " << Table1_D.countCollision << endl;

        cout << "# of Collisions for Custom Probing " << Table1_C.countCollision << endl;

        cout << "Average Probes for Chaining " << Table1_S.countProbes * 1.0 / search << endl;

        cout << "Average Probes for Double Hashing " << Table1_D.countProbes * 1.0 / search << endl;

        cout << "Average Probes for Custom Probing " << Table1_C.countProbes * 1.0 / search << endl
             << endl;

        cout << "hash2: " << endl
             << endl;

        cout << "# of Collisions for Chaining " << Table2_S.countCollision << endl;

        cout << "# of Collisions for Double Hashing " << Table2_D.countCollision << endl;

        cout << "# of Collisions for Custom Probing " << Table2_C.countCollision << endl;

        cout << "Average Probes for Chaining " << Table2_S.countProbes * 1.0 / search << endl;

        cout << "Average Probes for Double Hashing " << Table2_D.countProbes * 1.0 / search << endl;

        cout << "Average Probes for Custom Probing " << Table2_C.countProbes * 1.0 / search << endl;

        cout << endl
             << endl;
    }
    return 0;
}
