//
// Created by quiks on 13.10.2019.
//

#include "Server.h"
#include <cstring>
#include <iostream>
#include <mutex>
#include "ServerUtils/ServerUtils.h"

Server::Server(int max_connection) {
    for (auto i : client_message) {
        i.reserve(MESSAGE_SIZE);
    }
    ServerUtils::startServer();
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Unable to create server socket" << std::endl;
        throw ServerException();
    }
    ServerUtils::setSockOpt(server_socket);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(18666);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(server_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        std::cerr << "Unable to bind server socket" << std::endl;
        ServerUtils::closeServer(server_socket);
        throw ServerException();
    }

    if (listen(server_socket, 5)) {
        std::cerr << "Unable to listen" << std::endl;
        throw ServerException();
    }
    communication_socket.resize(max_connection, 0);
}
int Server::ListenSocket(int id) {
    char recieved_message[MESSAGE_SIZE];
    memset(recieved_message, 0, sizeof(char) * MESSAGE_SIZE);
    client_message[id].clear();

    /*следует помнить, что данные поступают неравномерно*/
    int rc = recv(communication_socket[id], recieved_message, MESSAGE_SIZE, 0);
    if (rc <= 0) {
        return 1;
    }
    client_message[id] = recieved_message;
    return 0;
}
void Server::SendMessage(std::string response, int id) {
    if (sendto(communication_socket[id], response.c_str(), response.size(), 0, (struct sockaddr *)&addr, sizeof(addr)) <
        0) {
        std::cerr << "Send error" << std::endl;
        throw ServerException();
    }
}

int Server::AcceptSocket(int id) {
    communication_socket[id] = accept(server_socket, NULL, NULL);
    if (communication_socket[id] < 0) {
        std::cerr << "Unable to create communication socket" << std::endl;
        throw ServerException();
    }
    return 0;
}

Server::~Server() { ServerUtils::closeServer(server_socket); }