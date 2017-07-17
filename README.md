# assign2
Simple C++ program using stream sockets

## Sockets Tutorial
* [Sockets Tutorial](http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html)

### The Client Server Model
Client needs to know of the existence of and the address of the server. Server does not need to know the address of or the existence of the client prior to the connection being established. Once a connection is established, both sides can send and receive information

Both client and server use socket. A socket is one end of an interprocess communication channel. The two processes each establish their own socket.

The steps involved in establishing a socket on the client side are as follows:
1. Create a socket with the socket() system call
2. Connect the socket to the address of the server using the connect() system call
3. Send and receive data. There are a number of ways to do this, but the simplest is to use the read() and write() system calls.

The steps involved in establishing a socket on the server side are as follows:
1. Create a socket with the socket() system call
2. Bind the socket to an address using the bind() system call. For a server socket on the Internet, an address consists of a port number on the host machine.
3. Listen for connections with the listen() system call
4. Accept a connection with the accept() system call. This call typically blocks until a client connects with the server.
5. Send and receive data

## Classes
1. Client
	* Socket
2. Server
	* Socket
3. Table
	* Neighborhood
	* Price
4. 

## Requirements