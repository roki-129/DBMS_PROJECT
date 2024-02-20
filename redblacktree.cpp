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

    Node* search(int key) {
        Node* temp = root;
        while (temp != nullptr) {
            if (key < temp->key) {
                if (temp->left == nullptr)
                    break;
                else
                    temp = temp->left;
            } else if (key == temp->key) {
                break;
            } else {
                if (temp->right == nullptr)
                    break;
                else
                    temp = temp->right;
            }
        }

        return temp;
    }

    void deleteNode(Node* v) {
        Node* u = BSTreplace(v);
        bool uvBlack = ((u == nullptr or u->color == BLACK) and (v->color == BLACK));
        Node* parent = v->parent;

        if (u == nullptr) {
            if (v == root) {
                root = nullptr;
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

    // Print the inserted node
    std::cout << key << "_" << value << std::endl;
}


    void erase(int key) {
        Node* v = search(key);
        if (v->key != key && root==nullptr ) {
            std::cout << "Key not found for deletion: " << key << std::endl;
            return;
        }

        deleteNode(v);
    }

   void clear() {
    if (root == nullptr) {
        std::cout << "unsuccessful" << std::endl;
    } else {
        deleteTree(root);
        root = nullptr;
        std::cout << "successful" << std::endl;
    }
}


    void find(int key) {
        Node* result = search(key);
        if (result->key == key)
            std::cout << "found" << std::endl;
        else
            std::cout << "not found" << std::endl;
    }

    void empty() {
        if (root == nullptr)
            std::cout << "yes" << std::endl;
        else
            std::cout << "no" << std::endl;
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
            std::cout << x->key << " => " << x->value << " ";
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
    while (std::cin >> operation) {
        if (operation == "I") {
            int key;
            std::string value;
            std::cin >> key >> value;
            map.insert(key, value);
            map.iteration();
        } else if (operation == "E") {
            int key;
            std::cin >> key;
            map.erase(key);
            map.iteration();
        } else if (operation == "Clr") {
            map.clear();
        } else if (operation == "F") {
            int key;
            std::cin >> key;
            map.find(key);
        } else if (operation == "Em") {
            map.empty();
        } else if (operation == "S") {
            map.size();
        } else if (operation == "Itr") {
            map.iteration();
        }
    }

    return 0;
}
