
Author: Eric Ferrer
Due Date: 5/29/16
CS 372 Project 2
Description: ReadMe File for ftClient/ftServer


Usage: 

Place a server in one directory and place the python file in a separate directory.

In the directory with the server file, run make and this will give the "ftserver" file. Run ./ftserver <portnumber> to start the server. 

Once the server is running it will wait for any incoming connections. This is when you run the
python file by typing in the following:

python ftclient.py <hostname> <port number> <data port number> <command>  <filename> 

example usage:

For directory contents:

python ftserver.py flip.engr.oregonstate.edu 3333 3334 -l 

For file transfer:

python ftserver.py flip.engr.oregonstate.edu 3333 3334 -g short.txt


To use the program correctly the server must be started first followed by the client. Once the server is launced
it will wait for the client to connect. Typing python ftserver.py flip.engr.oregonstate.edu 3333 3334 -l will display 
the directory contents. To get a file type python ftserver.py flip.engr.oregonstate.edu 3333 3334 -g short.txt and it will
transfer the file to the directory where the python file is



***
Resources

1) Beej's Guide to Network Programming
2) Socket Programming Howto https://docs.python.org/2/howto/sockets.html
3) Computer Networking: A Top-Down Approach - Kurose | Ross. (p.164)
3) python documentation: https://docs.python.org/2/