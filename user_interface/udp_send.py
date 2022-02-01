import socket

while True:
    UDP_IP = "10.31.67.9"
    UDP_PORT = 1206
    MESSAGE = input("Enter your value: ")

    print("UDP target IP:", UDP_IP)
    print("UDP target port:", UDP_PORT)
    print("message:", MESSAGE)

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
    sock.sendto(bytes(MESSAGE, "utf-8"), (UDP_IP, UDP_PORT))
