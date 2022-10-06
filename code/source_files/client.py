import socket
import requests

HOST = "127.0.0.1"  # The server's hostname or IP address
PORT = 8248  # The port used by the server

def gen():
  yield b'hello'
  yield b'frankrijk'
  yield b'this is an emergency'

def sendRequest(host, port):
  requests.get(f'http://{host}:{port}')
  # requests.post(f'http://{host}:{port}', data=gen())
  # print(r)

  # print(r)
  # with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
  #     s.connect((HOST, PORT))
  #     s.sendall(b'hello world')
  #     data = s.recv(1024)

if  __name__ == '__main__':
  sendRequest(HOST, PORT)