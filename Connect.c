#include "Client_header.h" // Include the client header file for necessary declarations

int Connect(Client_struct *client)                         // Function to establish a connection
{                                                          // Pointer to store the IP address entered by the user
    printf("Enter the ip address you want to connect : "); // Prompt the user to enter an IP address
    __fpurge(stdin);                                       // Clear the input buffer
    scanf("%[^\n]", client->ip_address);                  // Read the IP address from the user
    if (validate_ip_address(client))                      // Validate the entered IP address
    {
        client->server_addr.sin_addr.s_addr = inet_addr(client->ip_address); // Convert and assign the IP address to the server structure
        client->server_addr_len = sizeof(client->server_addr);               // Set the length of the server address structure
        return 1;                                                            // Return success
    }
    else
    {
        printf("Invalid ip address\n"); // Print an error message for invalid IP address
        return 0;                       // Return failure
    }
}

int validate_ip_address(Client_struct *client) // Function to validate the format of an IP address
{
    int i = 0, dot_count = 0;     // Initialize index and dot counter
    while (client->ip_address[i]) // Loop through each character in the IP address
    {
        if (isdigit(client->ip_address[i]))    // Check if the character is a digit
            ;                                  // Do nothing
        else if (client->ip_address[i] == '.') // Check if the character is a dot
            dot_count++;                       // Increment the dot counter
        else
            return 0; // Return failure for invalid character
        i++;          // Move to the next character
    }
    if (dot_count == 3) // Check if there are exactly 3 dots in the IP address
        return 1;       // Return success
    return 0;           // Return failure if the dot count is not 3
}