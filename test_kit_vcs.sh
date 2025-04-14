#!/bin/bash

# Test script for kit-vcs
echo "Running tests for kit-vcs..."

# Test directory
TEST_DIR="kit-vcs-test"

# Path to the kit-vcs executable
KIT_VCS_EXEC="../build/kit-vcs"

# Colors for output
GREEN="\033[1;32m"
RED="\033[1;31m"
YELLOW="\033[1;33m"
RESET="\033[0m"

# Helper function to print success messages
function print_success() {
    echo -e "${GREEN}[PASS]${RESET} $1"
}

# Helper function to print failure messages
function print_failure() {
    echo -e "${RED}[FAIL]${RESET} $1"
    exit 1
}

# Helper function to print warnings
function print_warning() {
    echo -e "${YELLOW}[WARN]${RESET} $1"
}

# Cleanup any previous test runs
if [ -d "$TEST_DIR" ]; then
    rm -rf "$TEST_DIR"
fi

# Create a fresh test directory
mkdir "$TEST_DIR"
cd "$TEST_DIR" || exit 1

# Test 1: Initialize repository
echo "Test 1: Initializing repository..."
$KIT_VCS_EXEC --init
if [ ! -d ".kit" ]; then
    print_failure "Repository initialization failed."
else
    print_success "Repository initialized successfully."
fi

# Test 2: Add a file to the staging area
echo "Test 2: Adding a file to the staging area..."
echo "Hello, kit-vcs!" >test.txt
$KIT_VCS_EXEC --add test.txt
if ! grep -q "test.txt" .kit/index; then
    print_failure "File was not added to the staging area."
else
    print_success "File added to the staging area successfully."
fi

# Test 3: Commit the staged file
echo "Test 3: Committing the staged file..."
$KIT_VCS_EXEC --commit "Initial commit"
if [ $? -ne 0 ]; then
    print_failure "Commit failed."
else
    print_success "Commit created successfully."
fi

# Test 4: Check repository status
echo "Test 4: Checking repository status..."
STATUS_OUTPUT=$($KIT_VCS_EXEC --status)
if [[ "$STATUS_OUTPUT" == *"Working directory clean. Nothing to commit."* ]]; then
    print_success "Repository status is clean."
else
    echo "Actual status output: $STATUS_OUTPUT"
    print_failure "Repository status is incorrect."
fi

# Test 5: Add a non-existent file
echo "Test 5: Adding a non-existent file..."
NON_EXISTENT_OUTPUT=$($KIT_VCS_EXEC --add non_existent.txt 2>&1)
if [[ "$NON_EXISTENT_OUTPUT" == *"File does not exist: non_existent.txt"* ]]; then
    print_success "Correctly handled non-existent file."
else
    print_failure "Did not handle non-existent file correctly."
fi

# Test 6: Commit without a message
echo "Test 6: Committing without a message..."
EMPTY_COMMIT_OUTPUT=$($KIT_VCS_EXEC --commit "" 2>&1)
if [[ "$EMPTY_COMMIT_OUTPUT" == *"Commit message cannot be empty."* ]]; then
    print_success "Correctly handled empty commit message."
else
    print_failure "Did not handle empty commit message correctly."
fi

# Test 7: Check version
echo "Test 7: Checking version..."
VERSION_OUTPUT=$($KIT_VCS_EXEC --version)
if [[ "$VERSION_OUTPUT" == *"kit-vcs version"* ]]; then
    print_success "Version displayed correctly."
else
    print_failure "Version not displayed correctly."
fi

# Test 8: Stash changes
echo "Test 8: Stashing changes..."
echo "Temporary changes" >temp.txt
$KIT_VCS_EXEC --add temp.txt
$KIT_VCS_EXEC --stash
if [ $? -ne 0 ]; then
    print_failure "Failed to stash changes."
else
    print_success "Changes stashed successfully."
fi

# Test 9: Create and switch branches
echo "Test 9: Creating and switching branches..."
$KIT_VCS_EXEC --branch another-branch
if [ $? -ne 0 ]; then
    print_failure "Failed to create branch: another-branch."
fi
$KIT_VCS_EXEC --checkout another-branch
if [ $? -ne 0 ]; then
    print_failure "Failed to switch to branch: another-branch."
fi
CURRENT_BRANCH=$($KIT_VCS_EXEC --status | grep "On branch another-branch")
if [[ "$CURRENT_BRANCH" == *"another-branch"* ]]; then
    print_success "Branch created and switched successfully."
else
    print_failure "Failed to create or switch branches."
fi

# Test 10: Merge branches
echo "Test 10: Merging branches..."
$KIT_VCS_EXEC --checkout main
$KIT_VCS_EXEC --merge new-branch
if [ $? -ne 0 ]; then
    print_failure "Failed to merge branches."
else
    print_success "Branches merged successfully."
fi

# Test 11: Reset to a specific commit
echo "Test 11: Resetting to a specific commit..."
COMMIT_HASH=$($KIT_VCS_EXEC --log | head -n 1 | awk '{print $1}')
$KIT_VCS_EXEC --reset "$COMMIT_HASH"
if [ $? -ne 0 ]; then
    print_failure "Failed to reset to commit: $COMMIT_HASH."
else
    print_success "Reset to commit successfully: $COMMIT_HASH."
fi

# Test 12: Show differences
echo "Test 12: Showing differences..."
DIFF_OUTPUT=$($KIT_VCS_EXEC --diff)
if [[ "$DIFF_OUTPUT" == *"Differences between commits or working directory."* ]]; then
    print_success "Differences displayed successfully."
else
    print_failure "Failed to display differences."
fi

# Cleanup
cd ..
rm -rf "$TEST_DIR"

echo -e "${GREEN}All tests passed successfully!${RESET}"
