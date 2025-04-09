# 🦊 kit-vcs

**`kit`** is a cute, minimal version control system written in modern C++. Inspired by Git, it is designed for learning and exploring the internals of version control systems.

![kit-vcs logo](assets/logo_pixel.png)
![kit banner](https://img.shields.io/badge/version-0.1.0-blue?style=flat-square)

---

## ✨ Features (WIP)

- [x] `kit init` – Initialize a repository.
- [x] `kit add <file>` – Stage files.
- [x] `kit commit` – Commit staged files.
- [ ] `kit log` – Show commit history.
- [ ] `kit status` – Show repository status.
- [ ] `.kit` structure and object storage.
- [ ] `kit stash` – Temporarily save changes.
- [ ] `kit branch` – Manage branches.
- [ ] `kit checkout` – Switch branches.
- [ ] `kit merge` – Merge branches.
- [ ] `kit reset` – Reset to a specific commit.
- [ ] `kit diff` – Show differences between commits or the working directory.

---

## 🛠 Getting Started

Follow these steps to set up and use `kit-vcs`:

### 🔧 Build

1. Clone the repository:
   ```bash
    git clone https://github.com/yourusername/kit-vcs.git
    cd kit-vcs
   ```

2. Build the project:
```bash
   mkdir build && cd build
   cmake ..
   make
```

3. Run the CLI:
   ```bash
   ./kit-vcs --help
   ```

---

🛠 CLI Commands
Here are the Git-like commands supported by kit-vcs:

- kit init – Initialize a new repository.
- kit add <file> – Add file(s) to the staging area.
- kit commit -m <message> – Commit staged files with a message.
- kit log – Show commit history.
- kit status – Show the current status of the repository.
- kit stash – Stash changes temporarily.
- kit branch – Manage branches.
- kit checkout <branch> – Switch to a specific branch.
- kit merge <branch> – Merge a branch into the current branch.
- kit reset <commit> – Reset to a specific commit.
- kit diff – Show differences between commits or the working directory.

---

🗂 CLI Usage
Here’s an example of how to use kit-vcs:

```bash
kit init
kit add main.cpp
kit commit -m "Initial commit"
kit log
```

---
🧪 Tech Stack

- C++17 – Modern C++ for clean and efficient code.
- CMake – Build system for cross-platform compatibility.
- cxxopts – Lightweight CLI parser.
- Filesystem API – For file and directory operations.
(Future) OpenSSL or Crypto++ – For hashing and encryption.

---

📦 Project Structure
The .kit directory structure:

```bash
.kit/
├── HEAD
├── objects/
└── refs/
    └── heads/
```
---

👀 Goals

   -  Learn Git internals

   -  Write a clean version control system

   -  Play with low-level file I/O, hashing, and DAGs
