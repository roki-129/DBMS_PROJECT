#include <bits/stdc++.h>
using namespace std;

class FibonacciNode {
public:
    int key;
    int value;
    int degree;
    bool marked;
    FibonacciNode* child;
    FibonacciNode* next;
    FibonacciNode* prev;

    FibonacciNode(int k, int v) : key(k), value(v), degree(0), marked(false), child(nullptr), next(this), prev(this) {}
};

class FibonacciHeap {
private:
    FibonacciNode* maxNode;
    int numOfNodes;

    void link(FibonacciNode* parent, FibonacciNode* child) {
        child->prev->next = child->next;
        child->next->prev = child->prev;

        child->prev = parent;
        child->next = parent->child;
        parent->child->prev = child;
        parent->child = child;

        parent->degree++;
        child->marked = false;
    }

    void consolidate() {
        int maxDegree = static_cast<int>(floor(log2(numOfNodes))) + 1;
        vector<FibonacciNode*> degreeTable(maxDegree, nullptr);

        FibonacciNode* current = maxNode;
        vector<FibonacciNode*> nodes;

        do {
            nodes.push_back(current);
            current = current->next;
        } while (current != maxNode);

        for (FibonacciNode* node : nodes) {
            int degree = node->degree;
            while (degreeTable[degree] != nullptr) {
                FibonacciNode* other = degreeTable[degree];
                if (node->key < other->key) {
                    swap(node, other);
                }
                link(node, other);
                degreeTable[degree] = nullptr;
                degree++;
            }
            degreeTable[degree] = node;
        }

        maxNode = nullptr;
        for (FibonacciNode* node : degreeTable) {
            if (node != nullptr) {
                if (maxNode == nullptr || node->key > maxNode->key) {
                    maxNode = node;
                }
            }
        }
    }

    void cut(FibonacciNode* child, FibonacciNode* parent) {
        if (child->next == child) {
            parent->child = nullptr;
        } else {
            child->prev->next = child->next;
            child->next->prev = child->prev;
            if (parent->child == child) {
                parent->child = child->next;
            }
        }

        parent->degree--;
        child->next = child->prev = child;
        maxNode->prev->next = child;
        child->prev = maxNode->prev;
        maxNode->prev = child;
        child->marked = false;
    }

    void cascadingCut(FibonacciNode* node) {
        FibonacciNode* parent = node->prev;
        if (parent != nullptr) {
            if (!node->marked) {
                node->marked = true;
            } else {
                cut(node, parent);
                cascadingCut(parent);
            }
        }
    }

public:
    FibonacciHeap() : maxNode(nullptr), numOfNodes(0) {}

    FibonacciNode* make_heap() {
        return nullptr;
    }

    bool is_empty(FibonacciNode* heap) {
        return heap == nullptr;
    }

    FibonacciNode* insert(FibonacciNode* heap, int key, int value) {
        FibonacciNode* node = new FibonacciNode(key, value);
        if (heap == nullptr) {
            heap = node;
        } else {
            node->prev = heap->prev;
            node->next = heap;
            heap->prev->next = node;
            heap->prev = node;
            if (key > heap->key) {
                heap = node;
            }
        }
        numOfNodes++;
        return heap;
    }

    void extract_max() {
        if (maxNode == nullptr) {
            cout << "Heap is empty" << endl;
            return;
        }

        FibonacciNode* maxNode = this->maxNode;

        if (maxNode->next == maxNode) {
            this->maxNode = nullptr;
        } else {
            maxNode->prev->next = maxNode->next;
            maxNode->next->prev = maxNode->prev;
            this->maxNode = maxNode->next;
            consolidate();
        }

        if (maxNode->child != nullptr) {
            FibonacciNode* child = maxNode->child;
            do {
                FibonacciNode* nextChild = child->next;
                child->prev = nullptr;
                child->next = this->maxNode;
                if (this->maxNode != nullptr) {
                    this->maxNode->prev = child;
                }
                this->maxNode = child;

                child->marked = false;

                child = nextChild;
            } while (child != maxNode->child);
        }

        numOfNodes--;
        delete maxNode;
    }

    void increase_key(FibonacciNode* node, int newKey) {
        if (newKey < node->key) {
            cout << "New key is smaller than the current key" << endl;
            return;
        }

        node->key = newKey;
        FibonacciNode* parent = node->prev;

        if (parent != nullptr && node->key > parent->key) {
            cut(node, parent);
            cascadingCut(parent);
        }

        if (node->key > maxNode->key) {
            maxNode = node;
        }
    }

    void delete_node(FibonacciNode* node) {
        increase_key(node, INT_MAX);
        extract_max();
        cout << "Node Deleted" << endl;
    }

    FibonacciNode* meld(FibonacciNode* heap1, FibonacciNode* heap2) {
        if (heap1 == nullptr) {
            return heap2;
        }
        if (heap2 == nullptr) {
            return heap1;
        }

        heap1->prev->next = heap2;
        heap2->prev->next = heap1;
        swap(heap1->prev, heap2->prev);

        return (heap1->key > heap2->key) ? heap1 : heap2;
    }

    int find_max(FibonacciNode* heap) {
        return (heap != nullptr) ? heap->key : INT_MIN;
    }

    void print(FibonacciNode* heap) {
        if (heap == nullptr) {
            cout << "Empty Heap" << endl;
            return;
        }

        FibonacciNode* current = heap;
        do {
            cout << "Tree: ";
            FibonacciNode* node = current;
            do {
                cout << "(" << node->key << ", " << node->value << ")";
                if (node->child != nullptr) {
                    cout << " -> ";
                    FibonacciNode* child = node->child;
                    do {
                        cout << "(" << child->key << ", " << child->value << ")";
                        cout << (child->next != node->child ? ", " : "");
                        child = child->next;
                    } while (child != node->child);
                }
                cout << (node->next != current ? " -> " : "");
                node = node->next;
            } while (node != current);
            cout << endl;

            current = current->next;
        } while (current != heap);
    }
};

class PriorityQueue {
private:
    FibonacciHeap fibHeap;

public:
    PriorityQueue() {}

    FibonacciNode* make_heap() {
        return fibHeap.make_heap();
    }

    bool is_empty(FibonacciNode* heap) {
        return fibHeap.is_empty(heap);
    }

    FibonacciNode* insert(FibonacciNode* heap, int key, int value) {
        return fibHeap.insert(heap, key, value);
    }

    void extract_max(FibonacciNode*& heap) {
        fibHeap.extract_max();
    }

    void increase_key(FibonacciNode*& heap, FibonacciNode* node, int newKey) {
        fibHeap.increase_key(node, newKey);
    }

    void delete_node(FibonacciNode*& heap, FibonacciNode* node) {
        fibHeap.delete_node(node);
    }

    FibonacciNode* meld(FibonacciNode* heap1, FibonacciNode* heap2) {
        return fibHeap.meld(heap1, heap2);
    }

    int find_max(FibonacciNode* heap) {
        return fibHeap.find_max(heap);
    }

    void print(FibonacciNode* heap) {
        fibHeap.print(heap);
    }

    void test() {
    // Scenario 1: Basic operations on a single heap
    FibonacciNode* heap1 = make_heap();
    cout << "Inserting into Heap 1" << endl;
    heap1 = insert(heap1, 10, 1);
    heap1 = insert(heap1, 20, 2);
    heap1 = insert(heap1, 30, 3);
    cout << "Heap 1: ";
    print(heap1);

    cout << "Extracting Max from Heap 1" << endl;
    extract_max(heap1);
    cout << "Heap 1 after extraction: ";
    print(heap1);

    cout << "Increasing Key of a node in Heap 1" << endl;
    increase_key(heap1, heap1->child, 50);
    cout << "Heap 1 after increasing key: ";
    print(heap1);

    cout << "Deleting a node from Heap 1" << endl;
    delete_node(heap1, heap1->child);
    cout << "Heap 1 after deletion: ";
    print(heap1);

    cout << "Finding Max in Heap 1" << endl;
    cout << "Max: " << find_max(heap1) << endl;

    // Scenario 2: Melding two heaps
    FibonacciNode* heap2 = make_heap();
    cout << "Inserting into Heap 2" << endl;
    heap2 = insert(heap2, 15, 5);
    heap2 = insert(heap2, 25, 4);
    cout << "Heap 2: ";
    print(heap2);

    cout << "Melding Heaps 1 and 2" << endl;
    FibonacciNode* meldedHeap = meld(heap1, heap2);
    cout << "Melded Heap: ";
    print(meldedHeap);

    // Scenario 3: Extracting Max from the melded heap
    cout << "Extracting Max from Melded Heap" << endl;
    extract_max(meldedHeap);
    cout << "Melded Heap after extraction: ";
    print(meldedHeap);

    // Scenario 4: Increasing key in the melded heap
    cout << "Increasing Key in Melded Heap" << endl;
    increase_key(meldedHeap, meldedHeap->child, 50);
    cout << "Melded Heap after increasing key: ";
    print(meldedHeap);

    // Scenario 5: Deleting a node from the melded heap
    cout << "Deleting a node from Melded Heap" << endl;
    delete_node(meldedHeap, meldedHeap->child);
    cout << "Melded Heap after deletion: ";
    print(meldedHeap);

    cout << "Finding Max in Melded Heap" << endl;
    cout << "Max: " << find_max(meldedHeap) << endl;

    // Scenario 6: Testing with an empty heap
    FibonacciNode* emptyHeap = make_heap();
    cout << "Is Empty Heap empty? " << (is_empty(emptyHeap) ? "Yes" : "No") << endl;

    // Additional Scenarios: Add more scenarios as needed

    cout << "All test cases passed!" << endl;
}

};

int main() {
    // Creating an instance of the PriorityQueue class
    PriorityQueue pq;

    // Test Case for make_heap
    FibonacciNode* heap1 = pq.make_heap();
    cout << "Heap 1: ";
    pq.print(heap1);
    cout << endl;

    // Test Case for insert
    cout << "Inserting into Heap 1" << endl;
    heap1 = pq.insert(heap1, 10, 1);
    heap1 = pq.insert(heap1, 20, 2);
    heap1 = pq.insert(heap1, 30, 3);
    cout << "Heap 1: ";
    pq.print(heap1);
    cout << endl;

    // Test Case for is_empty
    cout << "Is Heap 1 empty? " << (pq.is_empty(heap1) ? "Yes" : "No") << endl;
    cout << endl;

    // Test Case for extract_max
    cout << "Extracting Max from Heap 1" << endl;
    pq.extract_max(heap1);
    cout << "Heap 1 after extraction: ";
    pq.print(heap1);
    cout << endl;

    // Test Case for increase_key
    cout << "Increasing Key of a node in Heap 1" << endl;
    pq.increase_key(heap1, heap1->child, 50);
    cout << "Heap 1 after increasing key: ";
    pq.print(heap1);
    cout << endl;

    // Test Case for delete_node
    cout << "Deleting a node from Heap 1" << endl;
    pq.delete_node(heap1, heap1->child);
    cout << "Heap 1 after deletion: ";
    pq.print(heap1);
    cout << endl;

    // Test Case for find_max
    cout << "Finding Max in Heap 1" << endl;
    cout << "Max: " << pq.find_max(heap1) << endl;

    pq.test();

    return 0;
}
