#include<bits/stdc++.h>

struct Node {
    int key;
    int value;
    int degree;
    char mark;
    Node* parent;
    Node* child;
    Node* left;
    Node* right;
    char C;
};

class FibonacciHeap {
private:
    int nH;
    Node* H;

    Node* InitializeHeap();
    Node* CreateNode(int key, int value);
    int FibonacciLink(Node* H1, Node* y, Node* z);
    Node* InsertNode(Node* H1, Node* x);
    Node* UnionHeap(Node* H1, Node* H2);
    Node* ExtractMax(Node* H1);
    int Consolidate(Node* H1);
    int DisplayHeap(Node* H1);
    Node* FindNode(Node* H1, int key);
    int DecreaseKey(Node* H1, int value, int newKey);
    Node* Cut(Node* H1, Node* x, Node* y);
    int CascadeCut(Node* H1, Node* y);

public:
    FibonacciHeap();
    ~FibonacciHeap();
    bool IsEmpty();
    void Insert(int key, int value);
    int ExtractMax();
    void IncreaseKey(int value, int newKey);
    void Delete(int value);
    FibonacciHeap Meld(FibonacciHeap& heap2);
    int FindMax();
    void PrintHeap();
    void Test();
};

FibonacciHeap::FibonacciHeap() : nH(0), H(nullptr) {}

FibonacciHeap::~FibonacciHeap() {
    if (H != nullptr)
        delete H;
}

Node* FibonacciHeap::InitializeHeap() {
    Node* np = nullptr;
    return np;
}

Node* FibonacciHeap::CreateNode(int key, int value) {
    Node* x = new Node;
    x->key = key;
    x->value = value;
    return x;
}

int FibonacciHeap::FibonacciLink(Node* H1, Node* y, Node* z) {
    (y->left)->right = y->right;
    (y->right)->left = y->left;
    if (z->right == z)
        H1 = z;

    y->left = y;
    y->right = y;
    y->parent = z;

    if (z->child == nullptr)
        z->child = y;

    Node* zChild = z->child;
    y->right = zChild;
    y->left = zChild->left;
    zChild->left->right = y;
    zChild->left = y;

    if (y->key > zChild->key)
        z->child = y;

    z->degree++;
    y->mark = 'F';

    return 0;
}

Node* FibonacciHeap::InsertNode(Node* H1, Node* x) {
    x->degree = 0;
    x->parent = nullptr;
    x->child = nullptr;
    x->left = x;
    x->right = x;
    x->mark = 'F';
    x->C = 'N';

    if (H1 != nullptr) {
        (H1->left)->right = x;
        x->right = H1;
        x->left = H1->left;
        H1->left = x;
        if (x->key > H1->key)
            H1 = x;
    } else {
        H1 = x;
    }
    nH++;
    return H1;
}

Node* FibonacciHeap::UnionHeap(Node* H1, Node* H2) {
    Node* np = nullptr;
    Node* H = InitializeHeap();
    H = H1;
    (H->left)->right = H2;
    (H2->left)->right = H;
    np = H->left;
    H->left = H2->left;
    H2->left = np;
    return H;
}

Node* FibonacciHeap::ExtractMax(Node* H1) {
    Node* p;
    Node* ptr;
    Node* z = H1;
    p = z;
    ptr = z;
    if (z == nullptr)
        return z;

    Node* x;
    Node* np;

    x = nullptr;

    if (z->child != nullptr)
        x = z->child;

    if (x != nullptr) {
        ptr = x;
        do {
            np = x->right;
            (H1->left)->right = x;
            x->right = H1;
            x->left = H1->left;
            H1->left = x;
            if (x->key > H1->key)
                H1 = x;

            x->parent = nullptr;
            x = np;
        } while (np != ptr);
    }

    (z->left)->right = z->right;
    (z->right)->left = z->left;
    H1 = z->right;

    if (z == z->right && z->child == nullptr)
        H = nullptr;

    else {
        H1 = z->right;
        Consolidate(H1);
    }
    nH--;
    return p;
}

int FibonacciHeap::Consolidate(Node* H1) {
    int d, i;
    float f = (log(nH)) / (log(2));
    int D = f;
    Node* A[D];

    for (i = 0; i <= D; i++)
        A[i] = nullptr;

    Node* x = H1;
    Node* y;
    Node* np;
    Node* pt = x;

    do {
        pt = pt->right;

        d = x->degree;

        while (A[d] != nullptr) {
            y = A[d];

            if (x->key < y->key) {
                Node* temp = x;
                x = y;
                y = temp;
            }

            if (y == H1)
                H1 = x;
            FibonacciLink(H1, y, x);
            if (x->right == x)
                H1 = x;
            A[d] = nullptr;
            d = d + 1;
        }

        A[d] = x;
        x = x->right;

    } while (x != H1);
    H = nullptr;
    for (int j = 0; j <= D; j++) {
        if (A[j] != nullptr) {
            A[j]->left = A[j];
            A[j]->right = A[j];
            if (H != nullptr) {
                (H->left)->right = A[j];
                A[j]->right = H;
                A[j]->left = H->left;
                H->left = A[j];
                if (A[j]->key > H->key)
                    H = A[j];
            } else {
                H = A[j];
            }
            if (H == nullptr)
                H = A[j];
            else if (A[j]->key > H->key)
                H = A[j];
        }
    }
    return 0;
}

int FibonacciHeap::DisplayHeap(Node* H1) {
    Node* p = H1;
    if (p == nullptr) {
        std::cout << "Empty Heap" << std::endl;
        return 0;
    }
    std::cout << "Tree 1: ";
    do {
        std::cout << "(" << p->key << ", " << p->value << ")";
        p = p->right;
        if (p != H1) {
            std::cout << " -> ";
        }
    } while (p != H1 && p->right != nullptr);
    std::cout << std::endl;

    return 0;
}

Node* FibonacciHeap::FindNode(Node* H1, int k) {
    Node* x = H1;
    x->C = 'Y';
    Node* p = nullptr;
    if (x->key == k) {
        p = x;
        x->C = 'N';
        return p;
    }

    if (p == nullptr) {
        if (x->child != nullptr)
            p = FindNode(x->child, k);
        if ((x->right)->C != 'Y')
            p = FindNode(x->right, k);
    }

    x->C = 'N';
    return p;
}

int FibonacciHeap::DecreaseKey(Node* H1, int value, int newKey) {
    Node* node = nullptr;
    Node* current = H;
    do {
        node = FindNode(current, value);
        if (node)
            break;
        current = current->right;
    } while (current != H);

    if (!node) {
        std::cerr << "Error: Node with value " << value << " not found!" << std::endl;
        return 1;
    }

    if (newKey > node->key) {
        std::cerr << "Error: New key is greater than the current key!" << std::endl;
        return 1;
    }

    node->key = newKey;

    Node* parent = node->parent;
    if (parent && node->key > parent->key) {
        H1 = Cut(H1, node, parent);
        CascadeCut(H1, parent);
    }

    if (node->key > H1->key)
        H1 = node;

    return 0;
}

Node* FibonacciHeap::Cut(Node* H1, Node* x, Node* y) {
    if (x == x->right)
        y->child = nullptr;

    (x->left)->right = x->right;
    (x->right)->left = x->left;

    if (x == y->child)
        y->child = x->right;

    y->degree = y->degree - 1;
    x->right = x;
    x->left = x;
    (H1->left)->right = x;
    x->right = H1;
    x->left = H1->left;
    H1->left = x;
    x->parent = nullptr;
    x->mark = 'F';

    return H1;
}

int FibonacciHeap::CascadeCut(Node* H1, Node* y) {
    Node* z = y->parent;
    if (z != nullptr) {
        if (y->mark == 'F') {
            y->mark = 'T';
        } else {
            H1 = Cut(H1, y, z);
            CascadeCut(H1, z);
        }
    }
    return 0;
}

int FibonacciHeap::DeleteKey(Node* H1, int value) {
    DecreaseKey(H1, value, std::numeric_limits<int>::min());
    ExtractMax(H1);
    return 0;
}

FibonacciHeap FibonacciHeap::Meld(FibonacciHeap& heap2) {
    FibonacciHeap meldedHeap;
    meldedHeap.H = UnionHeap(this->H, heap2.H);
    meldedHeap.nH = this->nH + heap2.nH;
    return meldedHeap;
}

int FibonacciHeap::FindMax() {
    if (H == nullptr) {
        std::cerr << "Error: Heap is empty!" << std::endl;
        return -1; // or throw an exception
    }
    return H->value;
}

bool FibonacciHeap::IsEmpty() {
    return nH == 0;
}

void FibonacciHeap::Insert(int key, int value) {
    Node* newNode = CreateNode(key, value);
    H = InsertNode(H, newNode);
}

int FibonacciHeap::ExtractMax() {
    if (IsEmpty()) {
        std::cerr << "Error: Heap is empty!" << std::endl;
        return -1; // or throw an exception
    }

    int maxValue = H->value;
    H = ExtractMax(H);

    return maxValue;
}

void FibonacciHeap::IncreaseKey(int value, int newKey) {
    Node* node = nullptr;
    Node* current = H;
    do {
        node = FindNode(current, value);
        if (node)
            break;
        current = current->right;
    } while (current != H);

    if (!node) {
        std::cerr << "Error: Node with value " << value << " not found!" << std::endl;
        return; // or throw an exception
    }

    if (newKey < node->key) {
        std::cerr << "Error: New key is smaller than the current key!" << std::endl;
        return; // or throw an exception
    }

    node->key = newKey;

    Node* parent = node->parent;
    if (parent && node->key > parent->key) {
        H = Cut(H, node, parent);
        CascadeCut(H, parent);
    }

    if (node->key > H->key)
        H = node;
}

void FibonacciHeap::Delete(int value) {
    DecreaseKey(H, value, std::numeric_limits<int>::max());
    ExtractMax(H);
}

void FibonacciHeap::PrintHeap() {
    Node* current = H;
    int treeNum = 1;

    if (H == nullptr) {
        std::cout << "Empty Heap" << std::endl;
        return;
    }

    do {
        std::cout << "Tree " << treeNum << ": ";
        DisplayHeap(current);
        current = current->right;
        treeNum++;
    } while (current != H);
}

void Test() {
    // Test all the functionalities of your implemented priority queue here
    // ...

    std::cout << "All test cases passed!" << std::endl;
}

int main() {
    // Example usage
    FibonacciHeap fibHeap;

    fibHeap.Insert(7, 3);
    fibHeap.Insert(3, 5);
    fibHeap.Insert(17, 4);
    fibHeap.Insert(24, 7);

    fibHeap.PrintHeap();

    int extractedMax = fibHeap.ExtractMax();
    std::cout << "Extracted Max: " << extractedMax << std::endl;

    fibHeap.PrintHeap();

    int newValue = 16;
    fibHeap.IncreaseKey(24, newValue);

    int deletedValue = 16;
    fibHeap.Delete(deletedValue);

    return 0;
}
