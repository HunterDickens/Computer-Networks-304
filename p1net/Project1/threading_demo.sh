#!/bin/bash

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
    echo "Starting client request for $1 in the background..."
    ./client $1 & # Run the client in the background
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
