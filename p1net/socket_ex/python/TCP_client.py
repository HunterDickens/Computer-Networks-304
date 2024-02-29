from socket import * # Import socket module
serverName = '127.0.0.1' # Set the server address
serverPort = 12000 # Set the server port
clientSocket = socket(AF_INET, SOCK_STREAM) # Create a socket object
clientSocket.connect((serverName,serverPort)) # Connect to the server
sentence = raw_input('Input lowercase sentence:') # Get user input
clientSocket.send(sentence.encode()) # Send the user input to the server
modifiedSentence = clientSocket.recv(1024) # Receive the modified user input
print ("From Server:", modifiedSentence.decode()) # Print the modified user input
clientSocket.close() # Close the socket 

