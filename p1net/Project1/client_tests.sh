#!/bin/bash
rm client
rm server
gcc -o client client.c
gcc -o server server.c
# Restart the server for client test cases
echo "Restarting the server for client test cases..."
./server &
SERVER_PID=$!

# Wait for the server to start again
sleep 2

# Client test cases
echo "Running client test cases..."

# Test case 1: Requesting HelloWorld.html using the client
echo "Test case 1: Requesting HelloWorld.html using the client"
./client /HelloWorld.html
echo ""  # New line for readability

# Test case 2: Requesting a non-existent file using the client
echo "Test case 2: Requesting NonExistentPage.html using the client"
./client /NonExistentPage.html
echo ""  # New line for readability

# Stop the server
echo "Stopping the server after client test cases..."
kill $SERVER_PID
wait $SERVER_PID  # Wait for the server to stop

