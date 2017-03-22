/*******************************************************
 Author: Eric Ferrer
 Email: ferrere@oregonstate.edu
 CS361 / Project02               	Due Date: 5/29/2016
 Description: The ftp server
			  References include: Beej's Guide to Network Programming: http://beej.us/guide/bgnet/
								  Socket's Tutorial: http://www.linuxhowtos.org/C_C++/socket.htm
								  Took directory code from http://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
				
********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h> 

#define MAXBUFFLENGTH 500
#define BACKLOG 10

#define HANDLE "Server>"

void error(const char *msg);
int create(int somePort);
int connect(int initialSocket);
void parseMsg(int servSocket);
void ftpServer(int initialSocket);
void dataConnection(char *commands, char *dataPort, char *file, int servSocket);
void sendFile(int dataConnect, char *filename);
void listDirectory(int dataConnect);


//globals

char buffer[MAXBUFFLENGTH];
int numbytes;


/*******************************************************
Function: error(contst char *msg)
Param: Takes a char literal
Description: This displays error messages

********************************************************/
void error(const char *msg)
{
    perror(msg);
    exit(1);
}


/**********************************************************************
Function: connect(portNum)
Param: Takes a portnumber
Description: This function sets up a socket, binds the socket
to a port, and then listens for an incoming connection.

Reference: Bulk of function's code taken from http://www.linuxhowtos.org/C_C++/socket.htm

**********************************************************************/
int create(int somePort)
{
    struct sockaddr_in serv_addr;
	int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    error("ERROR opening socket");
    int optval = 1;

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0)	//Taken from Beej's guide ch 9.20
    {
        error("setsockopt(SO_REUSEADDR) failed");

    }
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(somePort);


    if (bind(sockfd, (struct sockaddr *) &serv_addr,
    sizeof(serv_addr)) < 0)
    error("ERROR on binding");

    listen(sockfd,BACKLOG);                        //Waiting for incoming connections

	printf("SOCKET OPENED \n");
    return sockfd;
}
/**********************************************************************
Function: connect(intialSocket)
Param: Takes the socket that we opened
Description:  Accepts an incoming connection.

Reference: Bulk of function's code taken from http://www.linuxhowtos.org/C_C++/socket.htm

**********************************************************************/

int connect(int initialSocket)
{	
	socklen_t clilen;
	struct sockaddr_in cli_addr;
	int new_fd;

    clilen = sizeof(cli_addr);
    new_fd = accept(initialSocket,(struct sockaddr *) &cli_addr, &clilen);  //Accept pending connection and create new socket file descriptor
    if (new_fd < 0)                                 //Accept returns -1
	{
    error("ERROR on accept");
    }
    printf("Client has been connected! \n" ) ;

	return new_fd;

}
/**********************************************************************
Function: parseMsg(servSocket)
Param: Takes socket that was created earlier as parameter 
Description: Receives command line parameters from client and parses the
string into individul variables

Resources 
http://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
**********************************************************************/
void parseMsg(int servSocket)
{
	char *dataPort;
	char *filename;
	char *commands;

    memset(buffer, 0, MAXBUFFLENGTH);
    if ((numbytes = recv(servSocket, buffer, MAXBUFFLENGTH-1, 0)) == -1) 		//receive command arguments from client
	{		
        error("error on recieve");
        exit(1);
    }
    buffer[numbytes-1] = '\0';       											 //clear null terminator
    printf("%s \n", buffer);



	commands = strtok(buffer, " \n");											// http://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
	dataPort = strtok(NULL, " \n");

	if (strcmp(commands, "-g") == 0)
	{
		filename = strtok(NULL, " \n");
	}

	dataConnection(commands, dataPort, filename, servSocket);


}
/**********************************************************************
Function: ftpServer(intialSocket)
Param: Takes the intial socket
Description: This function runs the FTP server

Reference: Bulk of function's code taken from http://www.linuxhowtos.org/C_C++/socket.html 
& reused from project 1
**********************************************************************/

void ftpServer(int initialSocket)
{	

	int clientServConnect;


	printf("SERVER NOW UP AND RUNNING. SUCCESS! \n");

	while(1)
	{	

		printf("WAITING FOR CLIENT TO CONNECT! \n");

		if(( clientServConnect = connect(initialSocket)) == -1)
		{
			error("ERROR HAPPENED SOMEWHERE \n" );
		}
		printf("NOW CONNECTED TO CLIENT \n");
	
		parseMsg(clientServConnect); //parse client message		
	}


}
/**********************************************************************
Function: dataConnection()
Param: Takes the command, data port number, filename, and socket from the server
Description: Makes a new a data connection where the directory contents
and file are sent over

**********************************************************************/
void dataConnection(char *commands, char *dataPort, char *filename, int servSocket)
{	
	
	int dataConnect;
	int dataSocket;
	int dataPort1;

	dataPort1 = atoi(dataPort);													//converts string to int
	
	dataSocket = create(dataPort1); 							           		//create a data socket	
	dataConnect = connect(dataSocket);											//ftserver initiates a TCP data connection with ftclient on <DATA_PORT>. (Call this connection Q) 
		if (strcmp(commands, "-l") == 0)										//ftserver sends its directory to ftclient on connection Q, and ftclient displays the directory on-screen. 
		{
			listDirectory(dataConnect);
			

		}
		else if (strcmp(commands, "-g") == 0)									//user 
		{
			printf("Welcome to g \n");
			printf(filename, "\n");
			sendFile(dataConnect, filename);
		}
		else
		{
			printf("THAT'S NOT A VALID COMMAND!");
		}
		close(dataConnect);
		close(dataSocket);
}
/**********************************************************************
Function: listDirectory(dataConnect)
Param: Takes the data connection
Description: Sends the contents of the current directory to the client

**********************************************************************/
void listDirectory(int dataConnect)
{
	printf("Here's the directory contents \n");			
	DIR           *d;													//http://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
	struct dirent *dir;
	d = opendir(".");													//this comment is more for myself. 
	if (d)																//. represents the current directory and .. represents the parent directory. when output into console
	{
		while ((dir = readdir(d)) != NULL)
		{
			//printf("%s\n", dir->d_name);								//debug message that prints contents on server terminal
			sprintf(buffer, "%s \n", dir->d_name);

				if(send(dataConnect, buffer, strlen(buffer), 0) == -1)	//taken from project 1
				{
					error("error on send");
				}
		}
		closedir(d);
	}	
	close(dataConnect);	
}
/**********************************************************************
Function: sendFile(dataconnect, *filename)
Param: Takes the data connection and the name of the file
Description: Takes the file from the server and sends it to the client
Reference: http://stackoverflow.com/questions/15856940/failed-to-send-file-via-socket/15857594

**********************************************************************/
void sendFile(int dataConnect, char *filename)
{														// Borrowed code from 
	char buf[1025];										//http://stackoverflow.com/questions/15856940/failed-to-send-file-via-socket/15857594
	FILE *file = fopen(filename, "r+");
	if (!file)
	{
		error("Can't open file for reading"); 			//display errors
		return;
	}
	while (!feof(file)) 
	{ 	
		int readBytes = fread(buf, 1, sizeof(buf), file); 
		if (readBytes < 1)
		{
			error("Can't read from file");
			fclose(file);
			return;
		}
		int offset = 0;
		do
		{
			int sentBytes = send(dataConnect, &buf[offset], readBytes - offset, 0);
			if (sentBytes < 1)
			{
                error("Can't write to socket");
                fclose(file);
                return;
            }
            offset += sentBytes;
        }
        while (offset < readBytes);
	} 
			fclose(file);
}
/**********************************************************************
Function: main(int argc, char *argv[])
Param: Takes optional parameters(port needed or program won't run)
Description: This function runs the program

**********************************************************************/
int main(int argc, char *argv[])
{
    int portNum;
	int portData;
    int initialSocket;

    if (argc < 2) {															//validate parameters
        fprintf(stderr,"Proper Usage: ./testserv.c. <port_number>n");
        exit(1);
    }
    portNum = atoi(argv[1]);

   initialSocket = create(portNum);

   ftpServer(initialSocket);

    return 0;
}