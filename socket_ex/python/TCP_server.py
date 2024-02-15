
from socket import * # Import socket module
serverPort = 12000 # Set the server port
serverSocket = socket(AF_INET,SOCK_STREAM) # Create a socket object
serverSocket.bind(('',serverPort)) # Bind the server port to the socket
serverSocket.listen(1) # Listen for incoming connections
print ("The server is ready to receive") # Print a message to the console
while True: # Create an infinite loop
     connectionSocket, addr = serverSocket.accept() # Accept the incoming connection
     
     sentence = connectionSocket.recv(1024).decode() # Receive the user input
     capitalizedSentence = sentence.upper() # Capitalize the user input
     connectionSocket.send(capitalizedSentence.encode()) # Send the modified user input
 
     connectionSocket.close() # Close the connection

