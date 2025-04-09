#include <iostream>
#include <cxxopts.hpp>

int main(int argc, char* argv[]) {
    cxxopts::Options options("kit", "Kit - A cute minimal git clone");

    options.add_options()
        ("init", "Initialize a new kit repo")
        ("add", "Add file to staging area", cxxopts::value<std::vector<std::string>>())
        ("commit", "Commit staged files")
        ("h,help", "Print help");

    auto result = options.parse(argc, argv);

    if (result.count("help") || argc == 1) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (result.count("init")) {
        std::cout << "Initializing kit repository..." << std::endl;
        // TODO: Create .kit directory etc.
    }

    if (result.count("add")) {
        for (auto& file : result["add"].as<std::vector<std::string>>()) {
            std::cout << "Adding file: " << file << std::endl;
            // TODO: Implement blob creation
        }
    }

    if (result.count("commit")) {
        std::cout << "Committing..." << std::endl;
        // TODO: Implement commit logic
    }

    return 0;
}
