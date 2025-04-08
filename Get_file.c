#include "Client_header.h" // Include the client header file

void get_file(Client_struct *client, tftp_packet *packet) // Function to get a file from the TFTP server
{
	packet->opcode = RRQ; // Set the opcode to RRQ (Read Request)
	char filename[20]; // Declare a buffer to store the filename
	strcpy(filename, packet->body.request.filename); // Copy the filename from the packet
	if (sendto(client->sockfd, packet, sizeof(tftp_packet), 0, (struct sockaddr *)&client->server_addr, client->server_addr_len) > 0) // Send the read request to the server
	{
		printf("Request sent\n"); // Print success message for request sent
		recvfrom(client->sockfd, packet, sizeof(tftp_packet), 0, (struct sockaddr *)&client->server_addr, &client->server_addr_len); // Receive the server's response
		if (packet->opcode == ERROR) // Check if the server responded with an error
		{
			printf("Failure : %s\n", packet->body.error_packet.error_msg); // Print the error message
			return; // Exit the function
		}
		else
		{
			printf("Success : %s\n", packet->body.error_packet.error_msg); // Print success message
			int fd = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0644); // Try to create a new file for writing
			if (fd < 0) // Check if the file already exists
			{
				fd = open(filename, O_TRUNC | O_WRONLY, 0644); // Open the file and truncate its content
				printf("File is already present. Clearing the previous content.\n"); // Notify that the file content is cleared
			}
			else
			{
				close(fd); // Close the file descriptor
				fd = open(filename, O_CREAT | O_WRONLY, 0644); // Reopen the file for writing
				printf("Success : Client side file has beed created.\n"); // Notify that the file is created
			}
			packet->opcode = ACK; // Set the opcode to ACK (Acknowledgment)
			if (sendto(client->sockfd, packet, sizeof(tftp_packet), 0, (struct sockaddr *)&client->server_addr, client->server_addr_len) < 0) // Send acknowledgment to the server
			{
				printf("Failure : Acknowledgement not sent\n"); // Notify failure to send acknowledgment
				return; // Exit the function
			}
			else
			{
				printf("Success : Acknowledgement sent\n"); // Notify success in sending acknowledgment
				int set_bit = 1, block_number = 0; // Initialize variables for loop control and block number
				while (set_bit) // Loop to receive data packets
				{
					recvfrom(client->sockfd, packet, sizeof(tftp_packet), 0, (struct sockaddr *)&client->server_addr, &client->server_addr_len); // Receive a data packet
					if (packet->opcode == DATA) // Check if the packet is a data packet
					{
						write(fd, packet->body.data_packet.data, packet->body.data_packet.size); // Write the data to the file
						packet->body.data_packet.block_number = block_number++; // Increment the block number
						printf("Success : Packet %d received\n", packet->body.data_packet.block_number); // Notify success in receiving the packet
						sendto(client->sockfd, packet, sizeof(tftp_packet), 0, (struct sockaddr *)&client->server_addr, client->server_addr_len); // Send acknowledgment for the received packet
					}
					else if (packet->opcode == ERROR) // Check if the packet is an error packet
					{
						set_bit = packet->body.error_packet.error_code; // Set the loop control variable to the error code
					}
				}
				close(fd); // Close the file descriptor
				if (sendto(client->sockfd, packet, sizeof(tftp_packet), 0, (struct sockaddr *)&client->server_addr, client->server_addr_len) < 0) // Send final acknowledgment
				{
					printf("Failure : Acknowledgement not sent\n"); // Notify failure to send acknowledgment
					return; // Exit the function
				}
				else
				{
					printf("Success : Acknowledgement sent\n"); // Notify success in sending acknowledgment
					printf("Success : File received\n"); // Notify success in receiving the file
				}
			}
		}
	}
	else
		printf("Failure : Request not sent\n"); // Notify failure to send the request
}
