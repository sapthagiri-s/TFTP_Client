#include "Client_header.h" // Include the header file for client-specific declarations

void main_menu(Client_struct *client, tftp_packet *packet) // Function to display the main menu and handle user input
{
    while (1) // Infinite loop to keep the menu running
    {
        short int option = 0; // Variable to store the user's menu choice
        // Display the menu options to the user
        printf("Enter the option you want execute in this project : \n1. Connect\n2. Get\n3. Put\n4. Mode\n5. Exit\n");

        // Read the user's choice and store it in the variable 'option'
        scanf("%hd", &option);
        packet->body.error_packet.error_code = 0; // Initialize the error code to 0
        switch (option) // Handle the user's choice
        {
        case 1: // If the user selects option 1
        {
            if (Connect(client)) // Call the Connect function and check if it succeeds
                printf("Connection successfull\n"); // Print success message
            else
                printf("Connection failed\n"); // Print failure message
        }
        break; // Exit the switch statement
        case 2: // If the user selects option 2
        {
            printf("Enter the file name : "); // Prompt the user to enter the file name
            __fpurge(stdin); // Clear the input buffer
            scanf("%[^\n]", packet->body.request.filename); // Read the file name from the user
            get_file(client, packet); // Call the get_file function to download the file
            __fpurge(stdin); // Clear the input buffer
        }
        break; // Exit the switch statement
        case 3: // If the user selects option 3
            printf("Enter the file name : "); // Prompt the user to enter the file name
            __fpurge(stdin); // Clear the input buffer
            scanf("%[^\n]", packet->body.request.filename); // Read the file name from the user
            put_file(client, packet); // Call the put_file function to upload the file
            __fpurge(stdin); // Clear the input buffer
            break; // Exit the switch statement
        case 4: // If the user selects option 4
        {
            short int mode_selection = 0; // Variable to store the user's mode selection
            // Display the mode options to the user
            printf("Select the mode you want to use : \n1. Octet\n2. Netascii\n3. Normal\n");
            scanf("%hd", &mode_selection); // Read the user's choice for mode
            switch (mode_selection) // Handle the user's choice for mode
            {
            case 1: // If the user selects octet mode
                strcpy(packet->body.request.mode, "octet"); // Set the mode to octet
                break; // Exit the switch statement
            case 2: // If the user selects netascii mode
                strcpy(packet->body.request.mode, "netascii"); // Set the mode to netascii
                break; // Exit the switch statement
            case 3: // If the user selects normal mode
                strcpy(packet->body.request.mode, "normal"); // Set the mode to normal
                break; // Exit the switch statement
            }
            break; // Exit the outer switch statement
        }
        case 5: // If the user selects option 5
            printf("Exiting the program...\n"); // Print "Exiting the program..."
            packet->body.error_packet.error_code = 2; // Set the error code to 2 to indicate disconnection
            // Send a message to the server to indicate disconnection
            sendto(client->sockfd, packet, sizeof(tftp_packet), 0, (struct sockaddr *)&client->server_addr, client->server_addr_len);
            exit(0); // Exit the program
        default: // If the user enters an invalid option
            printf("Invalid option please anyone of the option above.\n"); // Inform the user of invalid input
            break; // Exit the switch statement
        }
    }
}

int main() // Main function
{
    Client_struct client; // Declare a client structure
    tftp_packet packet; // Declare a TFTP packet structure
    memset(&client, 0, sizeof(client)); // Initialize client structure to zero
    memset(&packet, 0, sizeof(packet)); // Initialize packet structure to zero
    client.sockfd = socket(AF_INET, SOCK_DGRAM, 0); // Create a socket using UDP
    if (client.sockfd < 0) // Check if socket creation failed
    {
        perror("Socket creation failed"); // Print error message
        exit(1); // Exit the program with an error code
    }
    client.server_addr.sin_family = AF_INET; // Set the address family to IPv4
    client.server_addr.sin_port = htons(9091); // Set the port number to 9091
    main_menu(&client, &packet); // Call the main_menu function to start the program
}
