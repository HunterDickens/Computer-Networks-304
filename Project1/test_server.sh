#!/bin/bash

# Compile the client program
gcc -o client client.c
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

# Start the server in the background
./server &
SERVER_PID=$!

# Wait for a moment to ensure the server starts
sleep 2

# Use curl to request HelloWorld.html
echo "Requesting HelloWorld.html via curl..."
curl http://localhost:6789/HelloWorld.html
echo ""  # New line for readability

# Stop the server
echo "Stopping the server..."
kill $SERVER_PID
wait $SERVER_PID  # Wait for the server to stop

# Pause for a moment before restarting the server
sleep 2

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

# Add the threading demonstration part here
# Pause before starting the threading demonstration
sleep 2

echo "------ Starting Threading Demonstration ------"
# Restart the server for threading demonstration
./server &
SERVER_PID=$!
echo "Server restarted for threading demonstration"

# Wait for the server to start again
sleep 2

# Function to run a client test in the background
run_client_test() {
    local request_path=$1
    local response_file="response_$request_path.txt"
    echo "Starting client request for $request_path in the background..."
    ./client $request_path > $response_file 2>&1 &
    echo "Client request for $request_path launched, output will be in $response_file"
}


# Run multiple client tests concurrently for threading demonstration
run_client_test /HelloWorld.html
run_client_test /AnotherPage.html  # Assuming this page exists
run_client_test /NonExistentPage.html

# Allow some time for the server to handle the requests
echo "Waiting for server to process requests..."
sleep 5

# Stop the server
echo "Stopping the server after threading demonstration..."
kill $SERVER_PID
wait $SERVER_PID 

# Display the responses
echo "Responses from server:"
cat response_*.txt

# Clean up
# rm response_*.txt
