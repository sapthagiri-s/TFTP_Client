#include <stdio.h>      // Standard I/O library for input and output functions
#include <sys/stat.h>   // Header for file status and mode constants
#include <sys/wait.h>   // Header for process waiting functions
#include <fcntl.h>      // Header for file control options
#include <string.h>     // Header for string manipulation functions
#include <unistd.h>     // Header for POSIX operating system API
#include <stdlib.h>     // Header for general utilities like memory allocation
#include <sys/types.h>  // Header for data types used in system calls
#include <sys/socket.h> // Header for socket programming
#include <arpa/inet.h>  // Header for internet operations (e.g., IP addresses)
#include <ctype.h>      // Header for character handling functions
#include <stdio_ext.h>  // Header for extended I/O functions
#include "Tftp.h"       // Header for TFTP-specific definitions and functions

// Structure to hold client-related information
typedef struct
{
    int sockfd;                              // Socket file descriptor
    struct sockaddr_in server_addr;         // Server address structure
    unsigned int server_addr_len;           // Length of the server address structure
    char ip_address[100];                   // IP address of the server
} Client_struct;

// Function declaration for displaying the main menu
void main_menu(Client_struct *client, tftp_packet *packet);

// Function declaration for establishing a connection to the server
int Connect(Client_struct *client);

// Function declaration for downloading a file from the server
void get_file(Client_struct *client, tftp_packet *packet);

// Function declaration for uploading a file to the server
void put_file(Client_struct *client, tftp_packet *packet);

// Function declaration for validating the client's IP address
int validate_ip_address(Client_struct *client);
