import socket

def hex_print(data):
    hex_data = ' '.join(hex(byte)[2:].zfill(2) for byte in data)
    print(f"Received data (hexadecimal): {hex_data}")

def start_server():
    # Create a TCP/IP socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Bind the socket to a specific address and port
    server_address = ('localhost', 9090)
    server_socket.bind(server_address)

    # Listen for incoming connections
    server_socket.listen(1)

    print("Server is listening on port 9090...")

    while True:
        # Wait for a connection
        connection, client_address = server_socket.accept()

        try:
            print("New connection from:", client_address)

            # Receive data from the client
            data = connection.recv(1024)
            if data:
                hex_print(data)
            else:
                print("No data received.")
        
        except:
            continue

if __name__ == '__main__':
    start_server()

