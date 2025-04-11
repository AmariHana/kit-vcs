#!/bin/bash

# Test script for kit-vcs
echo "Running tests for kit-vcs..."

# Test directory
TEST_DIR="kit-vcs-test"

# Cleanup any previous test runs
if [ -d "$TEST_DIR" ]; then
    rm -rf "$TEST_DIR"
fi

# Create a fresh test directory
mkdir "$TEST_DIR"
cd "$TEST_DIR" || exit 1

# Path to the kit-vcs executable
KIT_VCS_EXEC="../build/kit-vcs"

# Test 1: Initialize repository
echo "Test 1: Initializing repository..."
$KIT_VCS_EXEC --init
if [ ! -d ".kit" ]; then
    echo "[FAIL] Repository initialization failed."
    exit 1
else
    echo "[PASS] Repository initialized successfully."
fi

# Test 2: Add a file to the staging area
echo "Test 2: Adding a file to the staging area..."
echo "Hello, kit-vcs!" >test.txt
$KIT_VCS_EXEC --add test.txt
if ! grep -q "test.txt" .kit/index; then
    echo "[FAIL] File was not added to the staging area."
    exit 1
else
    echo "[PASS] File added to the staging area successfully."
fi

# Test 3: Commit the staged file
echo "Test 3: Committing the staged file..."
$KIT_VCS_EXEC --commit "Initial commit"
if [ $? -ne 0 ]; then
    echo "[FAIL] Commit failed."
    exit 1
else
    echo "[PASS] Commit created successfully."
fi

# Test 4: Check repository status
echo "Test 4: Checking repository status..."
STATUS_OUTPUT=$($KIT_VCS_EXEC --status)
if [[ "$STATUS_OUTPUT" == *"Working directory clean. Nothing to commit."* ]]; then
    echo "[PASS] Repository status is clean."
else
    echo "[FAIL] Repository status is incorrect."
    exit 1
fi

# Test 5: Add a non-existent file
echo "Test 5: Adding a non-existent file..."
NON_EXISTENT_OUTPUT=$($KIT_VCS_EXEC --add non_existent.txt 2>&1)
if [[ "$NON_EXISTENT_OUTPUT" == *"File does not exist: non_existent.txt"* ]]; then
    echo "[PASS] Correctly handled non-existent file."
else
    echo "[FAIL] Did not handle non-existent file correctly."
    exit 1
fi

# Test 6: Commit without a message
echo "Test 6: Committing without a message..."
EMPTY_COMMIT_OUTPUT=$($KIT_VCS_EXEC --commit "" 2>&1)
if [[ "$EMPTY_COMMIT_OUTPUT" == *"Commit message cannot be empty."* ]]; then
    echo "[PASS] Correctly handled empty commit message."
else
    echo "[FAIL] Did not handle empty commit message correctly."
    exit 1
fi

# Test 7: Check version
echo "Test 7: Checking version..."
VERSION_OUTPUT=$($KIT_VCS_EXEC --version)
if [[ "$VERSION_OUTPUT" == *"kit-vcs version"* ]]; then
    echo "[PASS] Version displayed correctly."
else
    echo "[FAIL] Version not displayed correctly."
    exit 1
fi

# Cleanup
cd ..
rm -rf "$TEST_DIR"

echo "All tests passed successfully!"
