
// -----------------------------------
// CSCI 340 - Operating Systems
// Fall 2017
// server.h header file
// Homework 1
//
// -----------------------------------

#include <string.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include "server.h"

// ------------------------------------
// Function prototype that creates a socket and 
// then binds it to the specified port_number 
// for incoming client connections
// 
//
// Arguments:	port_number = port number the server 
//				socket will be bound to.
//
// Return:      Socket file descriptor (or -1 on failure)
//

int bind_port( unsigned int port_number ) {

	// -------------------------------------
	// NOTHING TODO HERE :)
	// -------------------------------------
	// Please do not modify

	int socket_fd;
	int set_option = 1;

    struct sockaddr_in server_address;
     
    socket_fd = socket( AF_INET, SOCK_STREAM, 0 );

    setsockopt( socket_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&set_option, sizeof( set_option ) );

    if (socket_fd < 0) return FAIL;

    bzero( (char *) &server_address, sizeof(server_address) );

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons( port_number );

    if ( bind( socket_fd, (struct sockaddr *) &server_address, sizeof(server_address) ) == 0 ) {

    	return socket_fd;

    } else {

    	return FAIL;

    }

} // end bind_port function


char* get_next_word(char** p1, char delimiter){

    char* p2 = *p1;
    int len = strlen(*p1);
    int i;
    char* word;
    for(i = 0; i < len; i++){

        if(*(*p1) == delimiter){
            *(*p1) = '\0';
            word = p2;
            (*p1) = (*p1) + 1; 
            p2 = *p1;
            break;
            
        }
 //       printf("p**: %c\n", *(*p1));

        (*p1) = (*p1) + 1; 
    }
    return word;
}


char* get_html_row(char* key, char* value){

   sprintf(row, "<tr><th>%s</th><th>%s</th></tr>", key, value); 
   printf("row: %s\n", row); 
   return row;
}



// ------------------------------------
// Function prototype that accepts a client
// socket connection
// 
//
// Arguments:	server file descriptor
//
// Return:      Termination status of client
//				( 0 = No Errors, -1 = Error )
int accept_client( int server_socket_fd ) {

                int exit_status = OK;

                int client_socket_fd = -1;

                socklen_t client_length; 

                struct sockaddr_in client_address;

                char request[512];

                client_length = sizeof( client_address );

                client_socket_fd = accept( server_socket_fd, (struct sockaddr *) &client_address, &client_length );
                                        
                                    // -------------------------------------
                                    //  // TODO:
                                    //          // -------------------------------------
                                    //                  // Modify code to fork a child process
                                    //                          // -------------------------------------
                                    //                                  
                                    //
              if ( client_socket_fd >= 0 ) {
                                    
		bzero( request, 512 );
		
		read( client_socket_fd, request, 511 );
		
		if ( DEBUG ) printf("Here is the http message:\n%s\n\n", request );
		
		// -------------------------------------
		// TODO:
		// -------------------------------------
		// Generate the correct http response when a GET or POST method is sent
		// from the client to the server.
		// 
		// In general, you will parse the request character array to:
		// 1) Determine if a GET or POST method was used
		// 2) Then retrieve the key/value pairs (see below)
		// -------------------------------------
		
		/*
		 ------------------------------------------------------
		 GET method key/values are located in the URL of the request message
		 ? - indicates the beginning of the key/value pairs in the URL
		 & - is used to separate multiple key/value pairs 
		 = - is used to separate the key and value
		 
		 Example:
		 
		 http://localhost/?first=brent&last=munsell
		 
		 two &'s indicated two key/value pairs (first=brent and last=munsell)
		 key = first, value = brent
		 key = last, value = munsell
		 ------------------------------------------------------
		 */
		
		/*
		 ------------------------------------------------------
		 POST method key/value pairs are located in the entity body of the request message
                 */
                char* p1 = request;
                char table[240]= "<table border=1 width=\"50%\">"; 

		char* method = strtok(p1, " ");

		if(strcmp(method,"GET") == 0){

                    printf("PROCESS GET REQUEST\n");
                    // get the full uri
                    char* resource  = strtok(NULL, " ");
                    
                    // see if their is key value pairs to process 
                    char* tok  = strtok(resource, "?");
                    
                    // get unparsed key value pairs
                    char* pairs = strtok(NULL, " ");

                    char* token;
                    // seperate all the key value pairs
                    while ((token = strsep(&pairs, "&"))){
                        char* key = strtok(token, "=");
                        printf("key: %s\n", key);
                        char* value = strtok(NULL, "\0");
                        printf("value: %s\n", value);
                        add_to_table(key, value, &table);
                    
                    }

		}
		else if(strcmp(method, "POST") == 0){

			printf("PROCESS GET REQUEST\n");
		}

		else{
			printf("INVALID HTTP REQUEST\n");
			exit(-1);
		}
		// THIS IS AN EXAMPLE ENTITY BODY
		
		char* entity_body = "<html><body><h2>CSCI 340 (Operating Systems) Project 1</h2><table border=1 width=\"50%\"><tr><th>Key</th><th>Value</th></tr></table></body></html>";
		
		char response[512];
		sprintf( response, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s", (int)strlen( entity_body ), entity_body );
		
		if ( DEBUG ) printf( "%s\n", response );
		
		send( client_socket_fd, response, strlen( response ), 0 );
		
		close( client_socket_fd );
		
		
	} else {
		
		exit_status = FAIL;
		
	}
	
	if ( DEBUG ) printf("Exit status = %d\n", exit_status );
	
	return exit_status;
	
} // end accept_client function
