//
// Created by quiks on 13.10.2019.
//

#ifndef UNTITLED7_LINUXCLIENT_H
#define UNTITLED7_LINUXCLIENT_H

#include <sys/socket.h>
#include <sys/types.h>
#include <memory>

#include <netinet/in.h>
#include "../Exception.h"
static const int MESSAGE_SIZE = 256;
class Client {
    int client_socket;
    sockaddr_in peer;
    int server_connection;

   public:
    explicit Client();
    int SendMessage(std::string message);
    int GetMessage();
    char recieved_message[MESSAGE_SIZE];
};

#endif  // UNTITLED7_LINUXCLIENT_H
