// client
// linux version
// Used http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html

// socket headers
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>

// other headers
#include <cstdio>
#include <cerrno>
#include <iostream>
#include <string.h>
#include <string>
#include <sstream>

using namespace std;

#include "template-functions.h"

#include "client.h"

// main

bool is_terminate_message(string const &message) {
    if (message == "Terminate.") {
        return true;
    }
    if (message == "Terminate") {
        return true;
    }
    if (message == "terminate.") {
        return true;
    }
    if (message == "terminate") {
        return true;
    }

    return false;
}

string prompt(string const &text) {
    string input;
    cout << text;
    getline(cin, input);
    return input;
}

void error(const char * msg) {
    perror(msg);
    exit(1);
}

#define LEN 256 // recv buffer length

void dostuff(int sock) {
    while (true) {
        string send_message = prompt("Enter a Houston neighborhood: ");
        int n;
        string recv_message;
        n = send(sock, send_message.c_str(), send_message.size() + 1, 0);
        if (n < 0)
            error("ERROR writing to socket");
        if (send_message.size() == 0)
            break;
        cout << "waiting: " << endl;
        while (true) {
            cout << "while: " << endl;
            char buffer[LEN + 1];
            bzero(buffer, LEN);
            n = recv(sock, buffer, LEN, 0);
            if (n < 0)
                error("ERROR reading from socket");
            recv_message += buffer;
            if (buffer[n - 1] == 0) {
                break;
            }
        }
        cout << "recvd: " << recv_message << endl;
        if (recv_message.size() == 0) {
            cout << "That neighborhood is not in the table" << endl;
        }
        else
            cout << "The average price of houses in " << send_message << " is $" << recv_message << endl;
    }
}

int main(int argc, char **argv) {
    // Start client
    Client client;

    char local_hostname[256] = {};
    gethostname(local_hostname, 256);

    // string hostname = prompt("Enter a server host name: ");
    string hostname = "Flagship";
    while (hostname != local_hostname) {
        cout << "Error. By requirement, the host name must equal '"
             << local_hostname
             << "'."
             << endl;
        hostname = prompt("Enter a server host name: ");
    }
    // string port = prompt("Enter server port number: ");
    string port = "1111";
    cout << endl;
    int port_number = to<int>(port);

    //cout << "connect_server" << endl;
    SOCKET server_socket = client.connect_server(hostname, port_number);
    if (server_socket == INVALID_SOCKET) {
        cout << "Could not connect to the server."
             << "\n"
             << "Please check the port number."
             << endl;
    }
    //cout << "connect_server end" << endl;
    if (server_socket != INVALID_SOCKET) {
        cout << "Connected to the server."
             << endl;
        // send/receive
        int result = 0;

        dostuff(server_socket);

        cout << endl;
        client.release_server();

    } // if ( server_socket != INVALID_SOCKET )
    return 0;
}
