#include "FileSystem.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

FileSystem::FileSystem() {
    root = new Node("/", NodeType::Directory, nullptr);
    cwd = root;
}

FileSystem::~FileSystem() {
    deleteSubtree(root);
}

void FileSystem::deleteSubtree(Node* node) {
    if (!node) return;
    Node* child = node->firstChild;
    while (child) {
        Node* next = child->nextSibling;
        deleteSubtree(child);
        child = next;
    }
    delete node;
}

void FileSystem::mkdir(const std::string& name) {
    if (cwd->findChild(name)) {
        cout << "mkdir: cannot create directory '" << name << "': File exists\n";
        return;
    }
    Node* dir = new Node(name, NodeType::Directory, cwd);
    if (!cwd->firstChild) {
        cwd->firstChild = dir;
    } else {
        Node* curr = cwd->firstChild;
        while (curr->nextSibling) curr = curr->nextSibling;
        curr->nextSibling = dir;
    }
}

void FileSystem::touch(const std::string& name) {
    if (cwd->findChild(name)) {
        cout << "touch: cannot create file '" << name << "': File exists\n";
        return;
    }
    Node* file = new Node(name, NodeType::File, cwd);
    if (!cwd->firstChild) {
        cwd->firstChild = file;
    } else {
        Node* curr = cwd->firstChild;
        while (curr->nextSibling) curr = curr->nextSibling;
        curr->nextSibling = file;
    }
}

void FileSystem::cd(const std::string& path) {
    if (path.empty()) return;
    Node* target = nullptr;
    if (path[0] == '/') {
        target = root;
    } else {
        target = cwd;
    }
    istringstream ss(path);
    string token;
    while (getline(ss, token, '/')) {
        if (token.empty() || token == ".") continue;
        if (token == "..") {
            if (target->parent) target = target->parent;
        } else {
            Node* next = target->findChild(token);
            if (!next || next->type != NodeType::Directory) {
                cout << "cd: no such directory: " << path << "\n";
                return;
            }
            target = next;
        }
    }
    cwd = target;
}

void FileSystem::ls() const {
    Node* curr = cwd->firstChild;
    while (curr) {
        cout << (curr->type == NodeType::Directory ? "[D] " : "[F] ") << curr->name << "\n";
        curr = curr->nextSibling;
    }
}

void FileSystem::rm(const std::string& name) {
    Node* victim = cwd->findChild(name);
    if (!victim) {
        cout << "rm: cannot remove '" << name << "': No such file or directory\n";
        return;
    }
    cwd->removeChild(name);
}

void FileSystem::pwd() const {
    if (cwd == root) {
        cout << "/\n";
        return;
    }
    vector<string> names;
    Node* curr = cwd;
    while (curr && curr != root) {
        names.push_back(curr->name);
        curr = curr->parent;
    }
    for (auto it = names.rbegin(); it != names.rend(); ++it) {
        cout << "/" << *it;
    }
    cout << "\n";
}

void FileSystem::writeFile(const std::string& name) {
    Node* file = cwd->findChild(name);
    if (!file) {
        cout << "write: file not found: " << name << "\n";
        return;
    }
    if (file->type != NodeType::File) {
        cout << "write: '" << name << "' is not a file\n";
        return;
    }
    cout << "Enter content (type ':wq' on a new line to save):\n";
    string content, line;
    while (true) {
        getline(cin, line);
        if (line == ":wq") break;
        content += line + "\n";
    }
    file->content = content;
    cout << "Content written to '" << name << "'.\n";
}

void FileSystem::readFile(const std::string& name) const {
    Node* file = cwd->findChild(name);
    if (!file) {
        cout << "read: file not found: " << name << "\n";
        return;
    }
    if (file->type != NodeType::File) {
        cout << "read: '" << name << "' is not a file\n";
        return;
    }
    if (file->content.empty()) {
        cout << "(file is empty)\n";
        return;
    }
    cout << file->content;
}

void FileSystem::editFile(const std::string& name) {
    writeFile(name);
}

void FileSystem::deleteFile(const std::string& name) {
    Node* file = cwd->findChild(name);
    if (!file) {
        cout << "delete: file not found: " << name << "\n";
        return;
    }
    if (file->type != NodeType::File) {
        cout << "delete: '" << name << "' is not a file\n";
        return;
    }
    cwd->removeChild(name);
    cout << "File '" << name << "' deleted.\n";
}

void FileSystem::runREPL() {
    map<string, string> suggestions = {
        {"wrtie", "write"},
        {"wriet", "write"},
        {"wirte", "write"},
        {"raed", "read"},
        {"reed", "read"},
        {"edti", "edit"},
        {"edti", "edit"},
        {"delte", "delete"},
        {"delet", "delete"},
        {"cat", "read"},
        {"lsit", "ls"},
        {"makdir", "mkdir"},
        {"mkdr", "mkdir"},
        {"tuch", "touch"}
    };
    string line;
    while (true) {
        cout << "filetreefs> ";
        if (!getline(cin, line)) break;
        istringstream iss(line);
        string cmd, arg;
        iss >> cmd;
        if (cmd == "exit" || cmd == "quit") break;
        else if (cmd == "help") {
            cout << "Available commands:\n";
            cout << "  mkdir <name>    - Create directory\n";
            cout << "  touch <name>    - Create file\n";
            cout << "  write <file>    - Write content to file\n";
            cout << "  read <file>     - Read file content\n";
            cout << "  edit <file>     - Edit file content\n";
            cout << "  delete <file>   - Delete file\n";
            cout << "  ls              - List directory contents\n";
            cout << "  cd <path>       - Change directory\n";
            cout << "  pwd             - Show current path\n";
            cout << "  rm <name>       - Remove file or directory\n";
            cout << "  exit            - Exit the program\n";
        } else if (cmd == "mkdir") {
            iss >> arg;
            if (arg.empty()) cout << "mkdir: missing operand. Usage: mkdir <name>\n";
            else mkdir(arg);
        } else if (cmd == "touch") {
            iss >> arg;
            if (arg.empty()) cout << "touch: missing operand. Usage: touch <name>\n";
            else touch(arg);
        } else if (cmd == "cd") {
            iss >> arg;
            if (arg.empty()) cout << "cd: missing operand. Usage: cd <path>\n";
            else cd(arg);
        } else if (cmd == "ls") {
            ls();
        } else if (cmd == "rm") {
            iss >> arg;
            if (arg.empty()) cout << "rm: missing operand. Usage: rm <name>\n";
            else rm(arg);
        } else if (cmd == "pwd") {
            pwd();
        } else if (cmd == "write") {
            iss >> arg;
            if (arg.empty()) cout << "write: missing filename. Usage: write <file>\n";
            else writeFile(arg);
        } else if (cmd == "read") {
            iss >> arg;
            if (arg.empty()) cout << "read: missing filename. Usage: read <file>\n";
            else readFile(arg);
        } else if (cmd == "edit") {
            iss >> arg;
            if (arg.empty()) cout << "edit: missing filename. Usage: edit <file>\n";
            else editFile(arg);
        } else if (cmd == "delete") {
            iss >> arg;
            if (arg.empty()) cout << "delete: missing filename. Usage: delete <file>\n";
            else deleteFile(arg);
        } else if (cmd.empty()) {
            continue;
        } else {
            cout << "Unknown command: " << cmd << "\n";
            if (suggestions.count(cmd)) {
                cout << "Did you mean: " << suggestions[cmd] << "?\n";
            }
            cout << "Type 'help' to see available commands.\n";
        }
    }
} 