# TFTP Client

This project implements a TFTP (Trivial File Transfer Protocol) client and server in C. The client and server communicate using UDP sockets to transfer files between them.

## Project Structure

### Key Components

- **TFTP_Client**: Contains the client-side implementation.
  - `Client.c`: Main client logic and menu interface.
  - `Connect.c`: Handles connecting to the server.
  - `Get_file.c`: Handles downloading files from the server.
  - `put_file.c`: Handles uploading files to the server.
  - `Client_header.h`: Header file for client-specific declarations.
  - `Tftp.h`: Shared TFTP packet structure and opcode definitions.

- **.vscode/settings.json**: Configures file associations for the project in Visual Studio Code.

## Features

- **File Upload**: The client can upload files to the server using the `put` command.
- **File Download**: The client can download files from the server using the `get` command.
- **Modes**: Supports different transfer modes (`octet`, `netascii`, `normal`).
- **Error Handling**: Handles errors such as file not found, invalid requests, and connection issues.

## How to Build and Run

### Prerequisites

- GCC compiler
- Visual Studio Code (optional)
- Linux environment (recommended)

### Build Instructions

1. compile the server:
   ```sh
   gcc -o client Client.c Connect.c Get_file.c put_file.c
   ./client
   # TFTP_Client
