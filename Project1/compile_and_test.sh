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

