import socket
import json
SERVER_IP = "127.0.0.1"
SERVER_PORT = 8826

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connecting to remote computer
server_address = (SERVER_IP, SERVER_PORT)
sock.connect(server_address)

# Receive message
#server_msg = sock.recv(1024)
#server_msg = server_msg.decode()
#print(server_msg)

msgType = input("Enter message type: 0 - signup, else - login:")
userNameInput = input("Enter your user name:")
passwordInput = input("Enter your password:")
if msgType == "0":
	code = "2"
	mailInput = input("Enter your mail:")
	data = {"username": userNameInput, "password": passwordInput, "mail": mailInput}
else:
	code = "1"
	data = {"username": userNameInput, "password": passwordInput}

length = str(len(json.dumps(data)))
msg = code + length.rjust(4, '0') + json.dumps(data)
print(msg)
# Send message to server
sock.sendall(msg.encode())
