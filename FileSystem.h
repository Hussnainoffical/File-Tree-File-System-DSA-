#pragma once
#include "Node.h"

// FileSystem: Simulates a Unix-like file system using a tree
class FileSystem {
private:
    Node* root;
    Node* cwd;
    void deleteSubtree(Node*);

public:
    FileSystem();
    ~FileSystem();

    void mkdir(const std::string& name);
    void touch(const std::string& name);
    void cd(const std::string& path);
    void ls() const;
    void rm(const std::string& name);
    void pwd() const;
    void runREPL();

    // File content simulation
    void writeFile(const std::string& name);
    void readFile(const std::string& name) const;
    void editFile(const std::string& name);
    void deleteFile(const std::string& name);
}; 
