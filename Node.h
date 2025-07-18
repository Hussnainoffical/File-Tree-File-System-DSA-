#pragma once
#include <string>

enum class NodeType { File, Directory };


class Node {
public:
    std::string name;
    NodeType type;
    Node* parent;
    Node* firstChild;
    Node* nextSibling;
    std::string content; 

    Node(const std::string& name, NodeType type, Node* parent = nullptr);
    ~Node();

    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;
    Node(Node&&) = default;
    Node& operator=(Node&&) = default;

    Node* findChild(const std::string& name) const;
    void removeChild(const std::string& name);
}; 