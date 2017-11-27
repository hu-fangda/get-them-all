#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

//Code by: Fangda (Kenny) Hu

static void die(const char *s) { perror(s); exit(1); }

int main(int argc, char **argv)
{
    if (argc != 4) {
        fprintf(stderr, "usage: %s <hostname> <server-port> <filename>\n",
                argv[0]);
        exit(1);
    
    }
    //Storing command line arguments
    const char *hostname = argv[1];
    unsigned short port = atoi(argv[2]);
    const char *filename = argv[3];
    //char ipaddress;

    //struct representing entry into hosts database
    struct hostent* hostInfo = gethostbyname(hostname);
    if(!hostInfo) {
	    die("DNS lookup failed!");
    }
    
    // Construct a server address structure

    struct sockaddr_in servaddr;
    
    memset(&servaddr, 0, sizeof(servaddr)); // must zero out the structure
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *)hostInfo->h_addr));
    servaddr.sin_port        = htons(port); // must be in network byte order


    // Creating the request
    int MAX_BUFFER = 5000;
    char request[MAX_BUFFER];

    int requestLength = snprintf(request, MAX_BUFFER, "GET %s HTTP/1.0\r\n"
		    "Host: %s:%d\r\n"
		    "\r\n",filename,hostname,(unsigned int)port);

    char realRequest[requestLength];
    strcpy(realRequest,request);


    // Create a socket for TCP connection

    int sock; // socket descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        die("socket failed");

    // Establish a TCP connection to the server

    if (connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        die("connect failed");

    unsigned int last_read_bytes;
    unsigned int total = 0;

    while (total < (requestLength)) {
	last_read_bytes = write(sock, realRequest, requestLength);
	
        total += last_read_bytes;
    }

    char buffer[MAX_BUFFER];
    char noheaders[MAX_BUFFER];
    size_t bytes;

    /*
     * recv(int socket, void *buffer, size_t length, int flags)
     *
     *   - normally, recv() blocks until it has received at least 1 byte
     *   - returns num bytes received, 0 if connection closed, -1 if error
     *   - recv(sock,buf,len,0) is equivalent to read(sock,buf,len)
     *   
     *   - With TCP sockets, we can receive less data than we requested;
     *     MSG_WAITALL flag changes this behavior -- it requests that the 
     *     operation block until the full request is satisfied.
     */

    
	FILE *response = fdopen(sock, "r");

	char *outputName;
	outputName = strrchr(filename,'/');
	FILE *output = fopen(outputName+1,"w+");

	if (output==NULL)
	{
		die("error opening file!");
	}
	
	int isBody = 0;
	int isFirstLine = 1;
	const char valid[30] = "200";
	

	while (fgets(buffer, sizeof(buffer), response))
	{
		if (isFirstLine == 1)
		{
		char *errorCode;
		errorCode = strstr(buffer,valid);
		if (errorCode == NULL)
		{
			printf("%s\n",buffer);
			remove(outputName+1);
			fclose(output);
			fclose(response);
			close(sock);
			return 0;

		}

		isFirstLine = 0;
		}

		if (strcmp(buffer,"\r\n")==0) {isBody = 1;}
		
		if (isBody == 1)
		
		{			
			while (0 < (bytes = fread(noheaders, 1, sizeof(noheaders),response)))
			{
				fwrite(noheaders, 1,bytes,output);
			}
			

		}
	}

	fclose(output);

	fclose(response);

    close(sock);
    return 0;

}
