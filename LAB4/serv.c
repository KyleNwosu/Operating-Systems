#include <netinet/in.h> //structure for storing address information 
#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 
#include <sys/socket.h> //for socket APIs 
#include <sys/types.h> 
#include "list.h"

#define PORT 9001
#define ACK "ACK"

static int servSockD = -1;
static int clientSocket = -1;
static list_t *mylist = NULL;
  

void handle_sigint(int sig){
	printf("Caught signal %d\n", sig);

	if (mylist != NULL){
		list_free(mylist);
		mylist = NULL;
	}
	if (clientSocket != -1){
		close(clientSocket);
		clientSocket = -1;
	}

	if (servSockD != -1){
		close(servSockD);
		servSockD = -1;
	}

	printf("Exiting\n");
	exit(0);
}

int main(int argc, char const* argv[]) 
{ 
  
	  int n, val, idx;
    // create server socket similar to what was done in 
    // client program 
    int servSockD = socket(AF_INET, SOCK_STREAM, 0); 
	signal(SIGINT, handle_sigint);
	servSockD = socket(AF_INET, SOCK_STREAM, 0);
    // string store data to recv/send to/from client 
    char buf[1024];
	char sbuf[1024];
    char* token;

    // define server address 
    struct sockaddr_in servAddr; 
  
	  // list


    servAddr.sin_family = AF_INET; 
    servAddr.sin_port = htons(PORT); 
    servAddr.sin_addr.s_addr = INADDR_ANY; 
  
    // bind socket to the specified IP and port 
    if (bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) { 
		printf("Binding: Failed\n"); 
		exit(1); 
	}
  
    // listen for connections 
    listen(servSockD, 1); 
	printf("Served listening on port %d\n", PORT);
  
    // integer to hold client socket. 
    clientSocket = accept(servSockD, NULL, NULL); 
	printf("Client: Connected\n");

    mylist = list_alloc();  // create the list

    while(1){
        // recvs messages from client socket 
        n = recv(clientSocket, buf, sizeof(buf), 0);
		buf[n] = '\0';

        if(n > 0){
            token = strtok(buf, " ");
    
						if(strcmp(token,"exit") == 0){
							  list_free(mylist);
							  mylist = NULL;
							  close(clientSocket);
							  close(servSockD);
							  printf("Exiting\n");
							  exit(1);
						}
						else if(strcmp(token,"get_length") == 0){
								// Make a Call
								val = list_length(mylist);
								sprintf(sbuf,"%s%d", "Length = ", val);
						}
						else if(strcmp(token,"add_front") == 0){
							  token = strtok(NULL, " ");  // get next token (value)
								val = atoi(token);
								// Make a Call
								sprintf(sbuf,"%s%d", ACK, val);
								list_add_to_front(mylist,val);
						}
						else if(strcmp(token,"add_back") ==0){
							token = strrok(NULL, "");
							val = atoi(token);
							list_add_to_back(mylist, val);
							sprintf(sbuf,"%s%d", ACK, val);
						}
						else if(strcmp(token,"add_postion")==0){
							token = strtok(NULL, " ");
							idx = atoi(token);
							token = strtok(NULL, " ");
							val = atoi(token);
							list_add_at_index(mylist, val, idx);
							sprintf(sbuf,"%s%d", ACK, val);
						}
						else if (strcmp(token, "remove_front") == 0){
							val = list_remove_from_front(mylist);
							sprintf(sbuf,"%s%d", ACK, val);
						}
						else if (strcmp(token, "remove_back")==0){
							val = list_remove_from_back(mylist);
							sprintf(sbuf,"%s%d", ACK, val);
						}

						else if(strcmp(token,"remove_position") == 0){
							  token = strtok(NULL, " ");
								idx = atoi(token);
								// Make a Call
								val = list_remove_at_index(mylist,idx);
								sprintf(sbuf,"%s%d", ACK, val);
						}
						else if (strcmp(token,"get_element")==0){
							token = strtok(NULL, " ");
							idx = atoi(token);
							val = list_get_elem_at(mylist, idx);
							if (val == -1){
								sprintf(sbuf,"%s", "Error");
							}
							else{
								sprintf(sbuf,"%s%d", ACK, val);
							}
						}
						else if(strcmp(token,"print") == 0){
							  sprintf(sbuf,"%s", listToString(mylist));
						}
						// ADD THE OTHER OPERATIONS


                // send's messages to client socket 
            send(clientSocket, sbuf, sizeof(sbuf), 0);
        }
				memset(buf, '\0', 1024);
    }
  
    return 0; 
}
