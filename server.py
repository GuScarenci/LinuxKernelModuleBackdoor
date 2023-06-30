import socket

def start_server():
    # Create a TCP socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    # Set the server IP address and port
    server_address = ('', 9090)  # Set the desired port number

    # Bind the socket to the server address
    server_socket.bind(server_address)

    # Listen for incoming connections
    server_socket.listen(1)
    print(f"Server listening on {server_address[0]}:{server_address[1]}")

    while True:
        # Accept a new connection
        client_socket, client_address = server_socket.accept()
        print(f"Accepted connection from {client_address[0]}:{client_address[1]}")

        try:
            # Receive data from the client
            data = client_socket.recv(1024)
            if data:
                print(f"Received message: {data.decode()}")

        finally:
            # Close the connection
            client_socket.close()


if __name__ == '__main__':
    start_server()
