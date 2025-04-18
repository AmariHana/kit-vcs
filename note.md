# Future implements folder changes



kit-vcs/
├── src/
│   ├── core/                     # Core logic for Git objects and repository management
│   │   ├── objects/              # Git object models (blobs, commits, trees, etc.)
│   │   │   ├── blob.cpp
│   │   │   ├── commit.cpp
│   │   │   ├── tree.cpp
│   │   │   └── object.hpp
│   │   ├── repository/           # Repository management logic
│   │   │   ├── repository.cpp
│   │   │   ├── repository.hpp
│   │   │   └── refs.cpp          # Reference management (HEAD, branches, tags)
│   │   └── utils/                # Shared utilities
│   │       ├── file_utils.cpp
│   │       ├── hash_utils.cpp
│   │       ├── constants.hpp
│   │       └── error_handler.cpp
│   ├── server/                   # Git server logic
│   │   ├── http/                 # HTTP server implementation
│   │   │   ├── http_server.cpp
│   │   │   ├── http_server.hpp
│   │   │   └── auth.cpp
│   │   ├── protocol/             # Git protocol implementation
│   │   │   ├── git_protocol.cpp
│   │   │   ├── git_protocol.hpp
│   │   │   └── packfile.cpp      # Packfile handling
│   │   └── server_main.cpp       # Entry point for the Git server
│   ├── cli/                      # Command-line interface logic
│   │   ├── commands/             # Individual CLI commands
│   │   │   ├── init.cpp
│   │   │   ├── commit.cpp
│   │   │   ├── diff.cpp
│   │   │   ├── merge.cpp
│   │   │   └── reset.cpp
│   │   ├── cli.cpp               # CLI entry point
│   │   └── cli.hpp
│   └── main.cpp                  # Main entry point for the CLI
├── include/                      # Header files
│   ├── core/                     # Core headers
│   │   ├── objects/
│   │   │   ├── blob.hpp
│   │   │   ├── commit.hpp
│   │   │   ├── tree.hpp
│   │   │   └── object.hpp
│   │   ├── repository.hpp
│   │   ├── refs.hpp
│   │   └── utils.hpp
│   ├── server/                   # Server headers
│   │   ├── http/
│   │   │   ├── http_server.hpp
│   │   │   └── auth.hpp
│   │   ├── protocol/
│   │   │   ├── git_protocol.hpp
│   │   │   └── packfile.hpp
│   │   └── server_main.hpp
│   ├── cli/                      # CLI headers
│   │   ├── commands/
│   │   │   ├── init.hpp
│   │   │   ├── commit.hpp
│   │   │   ├── diff.hpp
│   │   │   ├── merge.hpp
│   │   │   └── reset.hpp
│   │   └── cli.hpp
│   └── kit_vcs.hpp               # Main header for the project
├── repos/                        # Hosted Git repositories
│   └── demo.git/                 # Example repository
├── tests/                        # Unit and integration tests
│   ├── core/                     # Tests for core logic
│   │   ├── test_objects.cpp
│   │   ├── test_repository.cpp
│   │   └── test_utils.cpp
│   ├── cli/                      # Tests for CLI commands
│   │   ├── test_commit.cpp
│   │   ├── test_diff.cpp
│   │   ├── test_merge.cpp
│   │   └── test_reset.cpp
│   ├── server/                   # Tests for server logic
│   │   ├── test_http_server.cpp
│   │   ├── test_git_protocol.cpp
│   │   └── test_auth.cpp
│   └── test_main.cpp             # Entry point for running all tests
├── CMakeLists.txt                # Build configuration
├── README.md                     # Project documentation
└── .gitignore                    # Ignored files and directories
