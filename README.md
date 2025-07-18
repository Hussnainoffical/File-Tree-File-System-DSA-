#  FileTreeFS

**FileTreeFS** is a C++ command-line project that simulates a virtual file system completely in memory. Designed with manual pointer logic using the First-Child/Next-Sibling tree representation, it supports realistic file system operations like `mkdir`, `touch`, `cd`, `ls`, `pwd`, `rm`, and even in-memory `write`, `read`, `edit`, and `delete`.

---

##  Features

-  Create Directories (`mkdir`)
-  Create Files (`touch`)
-  Navigate (`cd`, `pwd`)
-  List Contents (`ls`)
-  Remove Files/Directories (`rm`)
-  Write, Read, Edit, Delete File Content — fully in-memory!
-  Helpful `help` command with usage descriptions
-  Suggests fixes for typos (`wrtie` → `write`)
-  Robust error messages with usage hints
-  Interactive REPL shell interface

---

##  Architecture

###  First-Child / Next-Sibling Tree Representation

Each node in the file system represents a file or directory and contains:
- `name`: Name of the file/directory
- `type`: Enum (File / Directory)
- `parent`: Pointer to parent node
- `firstChild`: Pointer to the first child (leftmost)
- `nextSibling`: Pointer to the next sibling
- `content`: (Files only) In-memory string storing simulated data

Efficient, minimal memory footprint — no STL containers for children!

---

## Project Structure

```bash
FileTreeFS
├── include/
│   ├── Node.h           # Node class and enum
│   └── FileSystem.h     # FileSystem class declarations
├── src/
│   ├── Node.cpp         # Node logic: creation, cleanup, tree ops
│   ├── FileSystem.cpp   # Full CLI logic, REPL loop, commands
│   └── main.cpp         # Program entry point
└── README.md            # This file
