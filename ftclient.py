#
# Author: Eric Ferrer
# Email: ferrere@oregonstate.edu
# CS361 / Project02               	Due Date: 5/29/2016
# Description: FTP client
#
#			  References include: Python Documentation: https://docs.python.org/2/
#								     					https://docs.python.org/2/tutorial/inputoutput.html
#								  						https://docs.python.org/2/howto/sockets.html
#								  Computer Networking: A Top-Down Approach - Kurose | Ross. (p.164)
#
#			  
#
#********************************************************/


#Taken from Networking Book.p.164
import sys
import os
from socket import *

##################################################
#
#Function:  main():
#
#Param: N/A
#
#Description: Runs the program
#             
#
##################################################

def main():

    if (len(sys.argv) < 4):
        print "Proper Usage: python ftclient.py <hostname> <port number> <data port number> <command>  <filename>"  #display if incorrect usage
        sys.exit("error")	
    else:

		serverName = sys.argv[1]   # name of the server
		serverPort = int(sys.argv[2])# port that server is on
		connect(serverName, serverPort) #establish connection between client and server

##################################################
#
#Function:  connect(serverName, serverPort):
#
#Param: Takes name of server and port number
#
#Description: Connects the clientserver to the chat server
#             via the proper port. 
#             
#
#
##################################################
def connect(serverName, serverPort):

    clientSocket = socket(AF_INET, SOCK_STREAM)# creates the client socket
    clientSocket.connect((serverName, serverPort))# intiates TCP onnection between client and server


    sendInfo(serverName, clientSocket)


#######################################################################
#
#Function:  sendInfo(serverName, clientSocket)
#
#Param: Takes name of server and the client's socket
#
#Description: Take the command line arguments and sends them to the server
# 			  
#             
########################################################################
def sendInfo(serverName, clientSocket):

	commands = sys.argv[4]

	dataPort = str(sys.argv[3])
	

	if (len(sys.argv) == 6):
		file = sys.argv[5]
		arguments = commands + " " + dataPort + " " + file + "\n"

	else:
		file = None 
		arguments = commands + " " + dataPort + "\n"
	
	print arguments

	clientSocket.send(arguments)
	
	dataConnection(serverName, clientSocket, commands, dataPort, file)
	
#######################################################################
#
#Function:  dataConnection(serverName, clientSocket, commands, dataPort, file)
#
#Param: Takes the name of the server and the client socket. Also takes the
#		command, port number for the data connection, and the filename.
#
#Description: Creates a new connection(connection Q). Receives the directory
#			  and file from the server. Closes connections.
# 			  
#             
########################################################################		
def dataConnection(serverName, clientSocket, commands, dataPort, file):
	
	dataPort1 = int(dataPort)
	dataSocket = socket(AF_INET, SOCK_STREAM)
	dataSocket.connect((serverName, dataPort1))# intiates TCP onnection between client and server
	
	if(commands == "-l"):
	
		modifiedSentence = dataSocket.recv(2048)# receives bytes from the server# server calls\ quit
		print modifiedSentence

	elif(commands == "-g"):
		
		f = open(file, "a")		#https://docs.python.org/2/tutorial/inputoutput.html
		modifiedSentence = dataSocket.recv(2048)# receives bytes from the server# server calls\ quit
		
		
		if not modifiedSentence:	#http://stackoverflow.com/questions/16739555/python-if-not-syntax
			print "No such file or directory"
			
		else:
			f.write(modifiedSentence)
			print "Transfer Complete \n"
			print file + " has been received"
		
		f.close()
	
	dataSocket.close()
	clientSocket.close()# closes the socket
	
	

if __name__ == '__main__':
    main()