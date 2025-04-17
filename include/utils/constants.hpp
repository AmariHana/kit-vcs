#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>

// Base directory for the `.kit` repository
const std::string KIT_DIR = ".kit";

// Directory for branch references
const std::string HEADS_DIR = KIT_DIR + "/refs/heads";

// File for the current HEAD reference
const std::string HEAD_FILE = KIT_DIR + "/HEAD";

// File for the staging area index
const std::string INDEX_FILE = KIT_DIR + "/index";

// Directory for storing objects (commits, blobs, etc.)
const std::string OBJECTS_DIR = KIT_DIR + "/objects";

#endif // CONSTANTS_HPP
