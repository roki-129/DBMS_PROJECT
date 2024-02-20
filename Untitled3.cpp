#include <iostream>
#include <string>

enum COLOR { RED, BLACK };

class RBTree {
private:
    struct Node {
        int key;
        std::string value;
        COLOR color;
        Node *left, *right, *parent;

        Node(int key, const std::string& value)
            : key(key), value(value), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}

        bool isOnLeft() { return this == parent->left; }

        Node* sibling() {
            if (parent == nullptr)
                return nullptr;

            if (isOnLeft())
                return parent->right;
            else
                return parent->left;
        }

        Node* uncle() {
            if (parent == nullptr or parent->parent == nullptr)
                return nullptr;

            if (parent->isOnLeft())
                return parent->parent->right;
            else
                return parent->parent->left;
        }

        bool hasRedChild() {
            return (left != nullptr and left->color == RED) or
                   (right != nullptr and right->color == RED);
        }

        void moveDown(Node* nParent) {
            if (parent != nullptr) {
                if (isOnLeft()) {
                    parent->left = nParent;
                } else {
                    parent->right = nParent;
                }
            }
            nParent->parent = parent;
            parent = nParent;
        }
    };

    Node* root;

    void leftRotate(Node* x) {
        Node* nParent = x->right;

        if (x == root)
            root = nParent;

        x->moveDown(nParent);

        x->right = nParent->left;
        if (nParent->left != nullptr)
            nParent->left->parent = x;

        nParent->left = x;
    }

    void rightRotate(Node* x) {
        Node* nParent = x->left;

        if (x == root)
            root = nParent;

        x->moveDown(nParent);

        x->left = nParent->right;
        if (nParent->right != nullptr)
            nParent->right->parent = x;

        nParent->right = x;
    }

    void swapColors(Node* x1, Node* x2) {
        COLOR temp = x1->color;
        x1->color = x2->color;
        x2->color = temp;
    }

    void swapValues(Node* u, Node* v) {
        std::swap(u->key, v->key);
        std::swap(u->value, v->value);
    }

    void fixRedRed(Node* x) {
        if (x == root) {
            x->color = BLACK;
            return;
        }

        Node* parent = x->parent;
        Node* grandparent = parent->parent;
        Node* uncle = x->uncle();

        if (parent->color != BLACK) {
            if (uncle != nullptr && uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                fixRedRed(grandparent);
            } else {
                if (parent->isOnLeft()) {
                    if (x->isOnLeft()) {
                        swapColors(parent, grandparent);
                    } else {
                        leftRotate(parent);
                        swapColors(x, grandparent);
                    }
                    rightRotate(grandparent);
                } else {
                    if (x->isOnLeft()) {
                        rightRotate(parent);
                        swapColors(x, grandparent);
                    } else {
                        swapColors(parent, grandparent);
                    }
                    leftRotate(grandparent);
                }
            }
        }
    }

    Node* searchHelper(Node* current, int key) {
        while (current != nullptr) {
            if (key < current->key) {
                if (current->left == nullptr)
                    break;
                else
                    current = current->left;
            } else if (key == current->key) {
                break;
            } else {
                if (current->right == nullptr)
                    break;
                else
                    current = current->right;
            }
        }

        return current;
    }

public:
    // Make the search function public
    Node* search(int key) {
        return searchHelper(root, key);
    }

    // Public wrapper for printing the result
    void printSearchResult(int key) {
        Node* result = search(key);
        if (result != nullptr && result->key == key)
            std::cout << key << " found" << std::endl;
        else
            std::cout << key << " not found" << std::endl;
    }


    void deleteNode(Node* v) {
        Node* u = BSTreplace(v);
        bool uvBlack = ((u == nullptr or u->color == BLACK) and (v->color == BLACK));
        Node* parent = v->parent;

        if (u == nullptr) {
            if (v == root) {
                v->key = 0; // Dummy key value
                v->value = "";
                v->left = v->right = nullptr;
            } else {
                if (uvBlack) {
                    fixDoubleBlack(v);
                } else {
                    if (v->sibling() != nullptr)
                        v->sibling()->color = RED;
                }

                if (v->isOnLeft()) {
                    parent->left = nullptr;
                } else {
                    parent->right = nullptr;
                }
            }
            delete v;
            return;
        }

        if (v->left == nullptr or v->right == nullptr) {
            if (v == root) {
                v->key = u->key;
                v->value = u->value;
                v->left = v->right = nullptr;
                delete u;
            } else {
                if (v->isOnLeft()) {
                    parent->left = u;
                } else {
                    parent->right = u;
                }
                delete v;
                u->parent = parent;
                if (uvBlack) {
                    fixDoubleBlack(u);
                } else {
                    u->color = BLACK;
                }
            }
            return;
        }

        swapValues(u, v);
        deleteNode(u);
    }

    void fixDoubleBlack(Node* x) {
        if (x == root)
            return;

        Node* sibling = x->sibling();
        Node* parent = x->parent;

        if (sibling == nullptr) {
            fixDoubleBlack(parent);
        } else {
            if (sibling->color == RED) {
                parent->color = RED;
                sibling->color = BLACK;
                if (sibling->isOnLeft()) {
                    rightRotate(parent);
                } else {
                    leftRotate(parent);
                }
                fixDoubleBlack(x);
            } else {
                if (sibling->hasRedChild()) {
                    if (sibling->left != nullptr and sibling->left->color == RED) {
                        if (sibling->isOnLeft()) {
                            sibling->left->color = sibling->color;
                            sibling->color = parent->color;
                            rightRotate(parent);
                        } else {
                            sibling->left->color = parent->color;
                            rightRotate(sibling);
                            leftRotate(parent);
                        }
                    } else {
                        if (sibling->isOnLeft()) {
                            sibling->right->color = parent->color;
                            leftRotate(sibling);
                            rightRotate(parent);
                        } else {
                            sibling->right->color = sibling->color;
                            sibling->color = parent->color;
                            leftRotate(parent);
                        }
                    }
                    parent->color = BLACK;
                } else {
                    sibling->color = RED;
                    if (parent->color == BLACK)
                        fixDoubleBlack(parent);
                    else
                        parent->color = BLACK;
                }
            }
        }
    }

    Node* BSTreplace(Node* x) {
        if (x->left != nullptr and x->right != nullptr)
            return successor(x->right);

        if (x->left == nullptr and x->right == nullptr)
            return nullptr;

        if (x->left != nullptr)
            return x->left;
        else
            return x->right;
    }

    Node* successor(Node* x) {
        while (x->left != nullptr)
            x = x->left;
        return x;
    }

public:
    RBTree() : root(nullptr) {}

    Node* getRoot() const {
        return root;
    }

    void insert(int key, const std::string& value) {
        Node* newNode = new Node(key, value);
        if (root == nullptr) {
            newNode->color = BLACK;
            root = newNode;
        } else {
            Node* temp = search(key);

            if (temp->key == key) {
                // Replace the value if the key already exists
                temp->value = value;
                delete newNode;
                return;
            }

            newNode->parent = temp;

            if (key < temp->key)
                temp->left = newNode;
            else
                temp->right = newNode;

            fixRedRed(newNode);
        }
    }

    void erase(int key) {
        Node* v = search(key);
        if (v->key != key) {
            std::cout << key << " not found for deletion" << std::endl;
            return;
        }

        deleteNode(v);
    }

    void clear() {
        deleteTree(root);
        root = nullptr;
        std::cout << "unsuccessful" << std::endl;
    }

    void find(int key) {
        Node* result = search(key);
        if (result->key == key)
            std::cout << key << " found" << std::endl;
        else
            std::cout << key << " not found" << std::endl;
    }

    bool isEmpty() const {
        return root == nullptr;
    }

    void size() {
        std::cout << countNodes(root) << std::endl;
    }

    void iteration() {
        inorderTraversal(root);
        std::cout << std::endl;
    }

    void inorderTraversal(Node* x) {
        if (x != nullptr) {
            inorderTraversal(x->left);
            std::cout << x->key << " ⇒ " << x->value << " ";
            inorderTraversal(x->right);
        }
    }

    int countNodes(Node* x) {
        if (x == nullptr)
            return 0;
        return countNodes(x->left) + countNodes(x->right) + 1;
    }

    void deleteTree(Node* x) {
        if (x != nullptr) {
            deleteTree(x->left);
            deleteTree(x->right);
            delete x;
        }
    }

    ~RBTree() {
        deleteTree(root);
    }
};

int main() {
    RBTree map;
    std::string operation;
    int commandCount = 0;

    while (std::cin >> operation) {
        commandCount++;

        std::cout << "Cmd " << commandCount << ": " << operation;

        if (operation == "Clr") {
            map.clear();
        } else if (operation == "Em") {
            std::cout << std::endl;
            if (map.isEmpty()) {
                std::cout << "yes" << std::endl;
            } else {
                std::cout << "no" << std::endl;
            }
        } else if (operation == "I") {
            int key;
            std::string value;
            std::cin >> key >> value;
            map.insert(key, value);
            std::cout << "(" << key << "_" << value << ")" << std::endl;
        } else if (operation == "F") {
            int key;
            std::cin >> key;
            map.printSearchResult(key);
        } else if (operation == "E") {
            int key;
            std::cin >> key;
            map.erase(key);
        } else if (operation == "S") {
            std::cout << std::endl << map.countNodes(map.getRoot()) << std::endl;
        } else if (operation == "Itr") {
            map.inorderTraversal(map.getRoot());
            std::cout << std::endl;
        } else {
            // Handle other commands if needed
            std::cout << std::endl;
        }
    }

    return 0;
}
