// server
// linux version
// Used http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html

// socket headers
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>

// other headers
#include <cstdio>
#include <cerrno>
#include <iostream>
#include <string.h>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

#include "template-functions.h"

#include "client.h"
#include "server.h"

// main

#define SOCKET int // linux uses int instead of a SOCKET struct

struct Listing
{
    string neighborhood;
    string price;
};

bool find_free_port(int &port)
{
    Client client;
    SOCKET server_socket = client.connect_server("localhost", port);
    while (server_socket != INVALID_SOCKET)
    {
        ++port;
        server_socket = client.connect_server("localhost", port);
    }
    return client.error_state() == false;
}

string prompt(string const &text)
{
    string input;
    cout << text;
    getline(cin, input);
    return input;
}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

#define LEN 256 // recv buffer length
vector<vector<string>> table;

int main(int argc, char **argv)
{
    // string filename = prompt("Enter a file name: ");
    string filename = "2.txt";
    Server server;
    // string port = prompt("Enter server port number: ");
    // int port_number = to<int>(port);
    int port_number = 1111;
    cout << port_number
         << endl;
    if (find_free_port(port_number) == false)
    {
        cout << "Could not connect to port number: " << port_number << "."
             << endl;
        return 1;
    }
    cout << "Listening to port number: " << port_number
         << endl
         << endl;

    int result = 0;

    ifstream in(filename);
    if (!in)
    {
        cout << "Cannot open input file. Program terminates!!!"
             << endl;
        exit(1);
    }

    // Getting number of lines in file
    string data;
    int count = 0;
    while (getline(in, data))
    {
        // cout << "Are you running?" << endl;
        count++;
    }
    // cout << "before clear" << endl;
    in.clear();
    // cout << "after clear" << endl;
    in.seekg(0, ios::beg);
    // cout << "after seek" << endl;
    in.close();

    Listing listing[count];
    string line;
    // cout << "top count: " << count << endl;
    count = 0;
    in.open(filename);
    while (getline(in, line))
    {
        // cout << "line " << line << endl;
        vector<string> line_data;
        if (line.empty() || line.at(0) == '\n')
        {
            continue;
        }
        string neighborhood;
        string price;
        int comma = line.find(",");
        neighborhood = line.substr(0, comma);
        // cout << "neighborhood " << neighborhood << endl;
        price = line.substr(comma + 2, '\n');
        // cout << "price " << price << endl;
        listing[count].neighborhood = neighborhood;
        listing[count].price = price;
        // string face = listing[count].neighborhood + " " + listing[count].price;
        count++;
        // cout << " dd";
    }
    in.close();
    while (true)
    {
        //cout << "connect_client" << endl;
        SOCKET client_socket = server.connect_client(port_number);
        //cout << "connect_client end" << endl;
        if (client_socket == INVALID_SOCKET)
        {
            break;
        }

        // cout << "Accepted a client." << endl;

        while (true)
        {
            // send/receive
            int n;
            string recv_message;
            // cout << "waiting " << endl;
            while (true)
            {
                char buffer[LEN + 1];
                bzero(buffer, LEN);
                n = recv(client_socket, buffer, LEN, 0);
                if (n < 0)
                    error("ERROR reading from socket");
                recv_message += buffer;
                if (buffer[n - 1] == 0)
                {
                    break;
                }
            }
            if (recv_message.size() == 0)
            {
                // cout << "recv_message == 0" << endl;
                server.release_client();
                break;
            }
            //  cout << "find neighborhood " << recv_message << endl;
            // Find neighborhood in vector from recv_message
            string send_message;
            // cout << "bottom count: " << count << endl;
            for (int i = 0; i < count; i++)
            {
                // cout << recv_message << endl;
                if (recv_message == listing[i].neighborhood)
                {
                    send_message = listing[i].price;
                    // cout << send_message << endl;
                }
            }
            n = send(client_socket, send_message.c_str(), send_message.size() + 1, 0);
            // cout << "send_message: " << send_message << endl;
            if (n < 0)
                error("ERROR writing to socket");
            // recv ID
        }
        cout << endl;
        server.release_client();
    } // while ( true )
    return 0;
}