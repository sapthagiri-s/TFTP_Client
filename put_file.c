#include "Client_header.h" // Include the client header file

void put_file(Client_struct *client, tftp_packet *packet) // Function to handle file upload
{
    packet->opcode = WRQ; // Set the opcode to WRQ (Write Request)
    char filename[20]; // Buffer to store the filename
    strcpy(filename, packet->body.request.filename); // Copy the filename from the packet
    if (sendto(client->sockfd, packet, sizeof(tftp_packet), 0, (struct sockaddr *)&client->server_addr, client->server_addr_len) > 0) // Send the write request to the server
    {
        printf("Request sent\n"); // Print success message
        int read_fd = open(filename, O_RDONLY); // Open the file in read-only mode
        if (read_fd < 0) // Check if the file exists
        {
            printf("Failure : File not found\n"); // Print error message if file not found
            return; // Exit the function
        }
        else
        {
            close(read_fd); // Close the file descriptor
            read_fd = open(filename, O_RDONLY); // Reopen the file in read-only mode
            printf("Success : Client side file has been opened.\n"); // Print success message
        }
        int read_by_mode = 512; // Default read size
        if (!strcmp(packet->body.request.mode, "octet")) // Check if mode is "octet"
        {
            read_by_mode = 1; // Set read size to 1 byte
        }
        else if (!strcmp(packet->body.request.mode, "netascii")) // Check if mode is "netascii"
        {
            read_by_mode = 1; // Set read size to 1 byte
        }
        else if (!strcmp(packet->body.request.mode, "normal")) // Check if mode is "normal"
        {
            read_by_mode = 512; // Set read size to 512 bytes
        }
        packet->opcode = DATA; // Set the opcode to DATA
        int bytes_read = 0, pos = 0, flag = 0; // Initialize variables for reading and tracking
        char *buffer1 = malloc(read_by_mode); // Allocate memory for the buffer
        while (bytes_read = read(read_fd, buffer1, read_by_mode)) // Read the file in chunks
        {
            packet->opcode = DATA; // Set the opcode to DATA
            packet->body.data_packet.size = bytes_read; // Set the size of the data packet
            if (!strcmp(packet->body.request.mode, "netascii")) // Check if mode is "netascii"
            {
                flag = 1; // Set flag for netascii mode
                if (pos == 512) // Check if position is 512
                {
                    packet->body.data_packet.size = 512; // Set the size of the data packet to 512
                    pos = 0; // Reset position
                    for (int i = 0; i < packet->body.data_packet.size; i++) // Iterate through the buffer
                    {
                        if (buffer1[i] == '\n') // Check if character is newline
                        {
                            buffer1[i] = '\r'; // Replace newline with carriage return
                            buffer1 = realloc(buffer1, bytes_read); // Reallocate memory for the buffer
                            buffer1[i] = '\n'; // Add newline after carriage return
                        }
                    }
                    memcpy(packet->body.data_packet.data, buffer1, bytes_read); // Copy data to the packet
                    sendto(client->sockfd, packet, sizeof(tftp_packet), 0, (struct sockaddr *)&client->server_addr, client->server_addr_len); // Send the packet to the server
                    recvfrom(client->sockfd, packet, sizeof(tftp_packet), 0, (struct sockaddr *)&client->server_addr, &client->server_addr_len); // Receive acknowledgment from the server
                    printf("INFO: Packet %d sent\n", packet->body.data_packet.block_number); // Print packet sent info
                }
            }
            if (!flag) // If not in netascii mode
            {
                memcpy(packet->body.data_packet.data, buffer1, bytes_read); // Copy data to the packet
                sendto(client->sockfd, packet, sizeof(tftp_packet), 0, (struct sockaddr *)&client->server_addr, client->server_addr_len); // Send the packet to the server
                recvfrom(client->sockfd, packet, sizeof(tftp_packet), 0, (struct sockaddr *)&client->server_addr, &client->server_addr_len); // Receive acknowledgment from the server
                printf("INFO: Packet %d sent\n", packet->body.data_packet.block_number); // Print packet sent info
            }
            pos++; // Increment position
        }
        if (flag) // If in netascii mode
        {
            memcpy(packet->body.data_packet.data, buffer1, pos); // Copy remaining data to the packet
            packet->body.data_packet.size = pos; // Set the size of the data packet
            sendto(client->sockfd, packet, sizeof(tftp_packet), 0, (struct sockaddr *)&client->server_addr, client->server_addr_len); // Send the packet to the server
            recvfrom(client->sockfd, packet, sizeof(tftp_packet), 0, (struct sockaddr *)&client->server_addr, &client->server_addr_len); // Receive acknowledgment from the server
            printf("INFO: Packet %d sent\n", packet->body.data_packet.block_number); // Print packet sent info
        }
        free(buffer1); // Free the allocated memory
        close(read_fd); // Close the file descriptor
        packet->opcode = ERROR; // Set the opcode to ERROR
        packet->body.error_packet.error_code = 0; // Set the error code to 0
        sendto(client->sockfd, packet, sizeof(tftp_packet), 0, (struct sockaddr *)&client->server_addr, client->server_addr_len); // Send the error packet to the server
        printf("INFO: File transfer completed\n"); // Print file transfer completion message
    }
    else
    {
        printf("Failure : Request not sent\n"); // Print error message if request not sent
        return; // Exit the function
    }
}