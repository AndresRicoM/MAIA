import socket
import time

def send_udp(message):
    UDP_IP = "10.31.67.9"
    UDP_PORT = 1206
    MESSAGE = message

    print("UDP target IP:", UDP_IP)
    print("UDP target port:", UDP_PORT)
    print("message:", MESSAGE)

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
    sock.sendto(bytes(MESSAGE, "utf-8"), (UDP_IP, UDP_PORT))


#Set Temperature to 37  Celcius
print('Hello, Welcome to MAIA!')
print('Running Example Growth and Light Stimulation Protocol')
print('Setting Temperature')
send_udp('T50')
print('Stimulating with Red Light')
send_udp('L2')
