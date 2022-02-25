/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <thread>
#include <pthread.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sstream>

#include "CLI.h"


using namespace std;

// edit your ClientHandler interface here:
class ClientHandler {
public:
    virtual void handle(int clientID) = 0;
};


class SocketIO : public DefaultIO {
    int clientID;
public:
    explicit SocketIO(int clientID) : clientID(clientID) {
    }

    string read() override;

    void write(string text) override;

    void write(float f) override;

    void read(float *f) override;

    ~SocketIO() override = default;

};
// you can add helper classes here and implement on the cpp file


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler : public ClientHandler {
public:
    virtual void handle(int clientID) {

        // Create the socket and start the CLI.
        SocketIO socket(clientID);
        CLI cli(&socket);
        cli.start();
    }
};


// implement on Server.cpp
class Server {
    int port;
    int sockID;
    bool isStopped;
    sockaddr_in serverAddr;
    sockaddr_in clientAddr;
    thread *t; // the thread to run the start() method in

    // you may add data members

public:
    explicit Server(int port) noexcept(false);

    virtual ~Server();

    void start(ClientHandler &ch) noexcept(false);

    void stop();
};

#endif /* SERVER_H_ */
