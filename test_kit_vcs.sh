#!/bin/bash

# Test script for kit-vcs
echo "Running tests for kit-vcs..."

# Test directory
TEST_DIR="kit-vcs-test"

# Path to the kit-vcs executable (set explicitly)
KIT_VCS_EXEC="$(pwd)/build/kit-vcs"

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

# Helper function to check command success
function run_command() {
    local description="$1"
    shift
    echo "$description..."
    "$@" >/dev/null 2>&1
    if [ $? -ne 0 ]; then
        print_failure "$description failed."
    else
        print_success "$description succeeded."
    fi
}

# Check if required tools are available
function check_dependencies() {
    for cmd in grep awk; do
        if ! command -v "$cmd" &>/dev/null; then
            print_failure "Required command '$cmd' is not installed."
        fi
    done
}

# Check if the kit-vcs executable exists
function check_executable() {
    if [ ! -f "$KIT_VCS_EXEC" ]; then
        print_failure "kit-vcs executable not found at $KIT_VCS_EXEC. Build the project first."
    fi
}

# Cleanup any previous test runs
function cleanup() {
    if [ -d "$TEST_DIR" ]; then
        rm -rf "$TEST_DIR"
    fi
}

# Run all tests
function run_tests() {
    # Create a fresh test directory
    mkdir "$TEST_DIR"
    cd "$TEST_DIR" || exit 1

    # Test 1: Initialize repository
    run_command "Test 1: Initializing repository" "$KIT_VCS_EXEC" --init
    [ -d ".kit" ] || print_failure "Repository initialization failed."

    # Test 2: Add a file to the staging area
    echo "Hello, kit-vcs!" >test.txt
    run_command "Test 2: Adding a file to the staging area" "$KIT_VCS_EXEC" --add test.txt
    grep -q "test.txt" .kit/index || print_failure "File was not added to the staging area."

    # Test 3: Commit the staged file
    run_command "Test 3: Committing the staged file" "$KIT_VCS_EXEC" --commit "Initial commit"

    # Test 4: Check repository status
    STATUS_OUTPUT=$("$KIT_VCS_EXEC" --status)
    [[ "$STATUS_OUTPUT" == *"Working directory clean. Nothing to commit."* ]] || print_failure "Repository status is incorrect."

    # Test 5: Add a non-existent file
    NON_EXISTENT_OUTPUT=$("$KIT_VCS_EXEC" --add non_existent.txt 2>&1)
    [[ "$NON_EXISTENT_OUTPUT" == *"File does not exist: non_existent.txt"* ]] || print_failure "Did not handle non-existent file correctly."

    # Test 6: Commit without a message
    EMPTY_COMMIT_OUTPUT=$("$KIT_VCS_EXEC" --commit "" 2>&1)
    [[ "$EMPTY_COMMIT_OUTPUT" == *"Commit message cannot be empty."* ]] || print_failure "Did not handle empty commit message correctly."

    # Test 7: Check version
    VERSION_OUTPUT=$("$KIT_VCS_EXEC" --version)
    [[ "$VERSION_OUTPUT" == *"kit-vcs version"* ]] || print_failure "Version not displayed correctly."

    # Test 8: Stash changes
    echo "Temporary changes" >temp.txt
    run_command "Test 8: Adding temp.txt to the staging area" "$KIT_VCS_EXEC" --add temp.txt
    run_command "Test 8: Stashing changes" "$KIT_VCS_EXEC" --stash

    # Test 9: Create and switch branches
    run_command "Test 9: Creating branch another-branch" "$KIT_VCS_EXEC" --branch another-branch
    run_command "Test 9: Switching to branch another-branch" "$KIT_VCS_EXEC" --checkout another-branch
    CURRENT_BRANCH=$("$KIT_VCS_EXEC" --status | grep "On branch another-branch")
    [[ "$CURRENT_BRANCH" == *"another-branch"* ]] || print_failure "Failed to create or switch branches."

    # Test 10: Merge branches
    run_command "Test 10: Switching to main branch" "$KIT_VCS_EXEC" --checkout main
    run_command "Test 10: Merging branch another-branch" "$KIT_VCS_EXEC" --merge another-branch

    # Test 11: Reset to a specific commit
    COMMIT_HASH=$("$KIT_VCS_EXEC" --log | head -n 1 | awk '{print $1}')
    run_command "Test 11: Resetting to commit $COMMIT_HASH" "$KIT_VCS_EXEC" --reset "$COMMIT_HASH"

    # Test 12: Show differences
    DIFF_OUTPUT=$("$KIT_VCS_EXEC" --diff)
    [[ "$DIFF_OUTPUT" == *"Differences between commits or working directory."* ]] || print_failure "Failed to display differences."

    # Cleanup
    cd ..
    rm -rf "$TEST_DIR"

    echo -e "${GREEN}All tests passed successfully!${RESET}"
}

# Main script execution
check_dependencies
check_executable
cleanup
run_tests
