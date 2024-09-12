from socket import *
host = "192.168.86.227"
port = 60000
server=socket(AF_INET,SOCK_STREAM)
server.bind((host,port))
server.listen(1)
print("Server has started")
client,addr = server.accept()
print("Connected to Client")
print("Address:",addr)
file=open("xyz2Data.txt","a")
try:
    while 1:
        data=client.recv(100).decode() 
        print(data)
        file.write(data)
except Exception as e:
    print("Server is closed")
    print("Exception: ",e)