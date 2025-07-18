#include "Node.h"
#include <iostream>
using namespace std;

Node::Node(const std::string& name, NodeType type, Node* parent)
    : name(name), type(type), parent(parent), firstChild(nullptr), nextSibling(nullptr) {}

Node::~Node() {
    Node* child = firstChild;
    while (child) {
        Node* next = child->nextSibling;
        delete child;
        child = next;
    }
}

Node* Node::findChild(const std::string& name) const {
    Node* curr = firstChild;
    while (curr) {
        if (curr->name == name) return curr;
        curr = curr->nextSibling;
    }
    return nullptr;
}

void Node::removeChild(const std::string& name) {
    Node* prev = nullptr;
    Node* curr = firstChild;
    while (curr) {
        if (curr->name == name) {
            if (prev) {
                prev->nextSibling = curr->nextSibling;
            } else {
                firstChild = curr->nextSibling;
            }
            curr->nextSibling = nullptr;
            delete curr;
            return;
        }
        prev = curr;
        curr = curr->nextSibling;
    }
} 