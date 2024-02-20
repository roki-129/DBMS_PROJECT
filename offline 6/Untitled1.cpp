#include<bits/stdc++.h>
class Node {
public:
    int key;
    int value;
    int degree;
    bool marked;
    Node* parent;
    Node* child;
    Node* left;
    Node* right;

    Node(int k, int v) : key(k), value(v), degree(0), marked(false), parent(nullptr), child(nullptr), left(this), right(this) {}
};

class FibonacciHeap {
private:
    Node* maxNode;
    int size;

    void consolidate();
    void link(Node* y, Node* x);
    void cut(Node* child, Node* parent);
    void cascadingCut(Node* node);

public:
    FibonacciHeap();
    bool isEmpty();
    void insert(int key, int value);
    int extractMax();
    void increaseKey(int value, int newKey);
    void remove(int value);
};

FibonacciHeap::FibonacciHeap() : maxNode(nullptr), size(0) {}

bool FibonacciHeap::isEmpty() {
    return size == 0;
}

void FibonacciHeap::insert(int key, int value) {
    Node* newNode = new Node(key, value);
    if (!maxNode) {
        maxNode = newNode;
    } else {
        newNode->right = maxNode;
        newNode->left = maxNode->left;
        maxNode->left->right = newNode;
        maxNode->left = newNode;

        if (newNode->key > maxNode->key) {
            maxNode = newNode;
        }
    }
    size++;
}

int FibonacciHeap::extractMax() {
    if (isEmpty()) {
        std::cerr << "Error: Heap is empty!" << std::endl;
        return -1; // or throw an exception
    }

    int maxValue = maxNode->value;

    // Remove maxNode from the root list
    if (maxNode->child) {
        Node* child = maxNode->child;
        do {
            Node* nextChild = child->right;
            child->parent = nullptr;
            if (child->marked) {
                child->marked = false;
            }
            child = nextChild;
        } while (child != maxNode->child);
    }

    Node* maxNodeTemp = maxNode;
    if (maxNode == maxNode->right) {
        maxNode = nullptr;
    } else {
        maxNode->left->right = maxNode->right;
        maxNode->right->left = maxNode->left;
        maxNode = maxNode->right;
    }

    // Consolidate the heap
    if (maxNodeTemp->child) {
        consolidate();
    }

    delete maxNodeTemp;
    size--;

    return maxValue;
}

void FibonacciHeap::consolidate() {
    int maxDegree = static_cast<int>(log2(size)) + 1;
    std::vector<Node*> degreeTable(maxDegree, nullptr);

    Node* current = maxNode;
    std::vector<Node*> rootList;
    do {
        rootList.push_back(current);
        current = current->right;
    } while (current != maxNode);

    for (Node* node : rootList) {
        int degree = node->degree;
        while (degreeTable[degree] != nullptr) {
            Node* other = degreeTable[degree];
            if (node->key < other->key) {
                std::swap(node, other);
            }
            link(node, other);
            degreeTable[degree] = nullptr;
            degree++;
        }
        degreeTable[degree] = node;
    }

    maxNode = nullptr;
    for (Node* node : degreeTable) {
        if (node != nullptr) {
            if (!maxNode) {
                maxNode = node;
            } else {
                link(maxNode, node);
                if (node->key > maxNode->key) {
                    maxNode = node;
                }
            }
        }
    }
}

void FibonacciHeap::link(Node* y, Node* x) {
    y->left->right = y->right;
    y->right->left = y->left;

    y->parent = x;
    if (!x->child) {
        x->child = y;
        y->right = y;
        y->left = y;
    } else {
        y->left = x->child;
        y->right = x->child->right;
        x->child->right->left = y;
        x->child->right = y;
    }
    x->degree++;
    y->marked = false;
}

void FibonacciHeap::cut(Node* child, Node* parent) {
    // Remove child from the child list of parent
    if (child == child->right) {
        parent->child = nullptr;
    } else {
        child->left->right = child->right;
        child->right->left = child->left;
        if (parent->child == child) {
            parent->child = child->right;
        }
    }

    // Add child to the root list
    child->left = maxNode;
    child->right = maxNode->right;
    maxNode->right->left = child;
    maxNode->right = child;

    child->parent = nullptr;
    child->marked = false;
}

void FibonacciHeap::cascadingCut(Node* node) {
    Node* parent = node->parent;
    if (parent) {
        if (!node->marked) {
            node->marked = true;
        } else {
            cut(node, parent);
            cascadingCut(parent);
        }
    }
}

void FibonacciHeap::increaseKey(int value, int newKey) {
    Node* node = nullptr;
    std::unordered_map<int, Node*> nodeMap;

    // Find the node with the given value
    Node* current = maxNode;
    do {
        if (current->value == value) {
            node = current;
            break;
        }
        nodeMap[current->value] = current;
        current = current->right;
    } while (current != maxNode);

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
        cut(node, parent);
        cascadingCut(parent);
    }

    if (node->key > maxNode->key) {
        maxNode = node;
    }
}

void FibonacciHeap::remove(int value) {
    increaseKey(value, std::numeric_limits<int>::max());
    extractMax();
}

int main() {
    FibonacciHeap fibHeap;

    fibHeap.insert(5, 100);
    fibHeap.insert(3, 200);
    fibHeap.insert(7, 300);

    std::cout << "Max Value: " << fibHeap.extractMax() << std::endl;

    fibHeap.increaseKey(200, 400);
     fibHeap.remove(200);

    std::cout << "Is Empty: " << fibHeap.isEmpty() << std::endl;



    return 0;
}
