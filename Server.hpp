#pragma once

#include <iostream>
#include <stdint.h>
#include <string>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <map>
#include "Utilities.hpp"
#include "RFC1459.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <netdb.h>
#include <sstream>

using namespace std;

#define SERVER_NAME         "ircserv"
#define SERVER_ASSERT(cond) if ((cond) == -1)    return false

class Server
{
private:
    uint16_t port;
    string pwd;
    int fd;
    pollfd pfd[SOMAXCONN]; //le max de pending connexions en attente pour un socket
    size_t size;
    bool stop;
    map<string, cmd_t> cmds;
    map<int, Client*> clients;
    map<string, Channel*> channels;
    
public:
    Server(uint16_t _port, string _pwd);
    ~Server();

    uint16_t getPort();
    string getPwd();
    int getFd();
    pollfd* getPfd(int i);
    size_t getSize();
    bool getStop();
    void setStop(bool _stop);

    bool start();
    void acceptClient();
    void handleRequest(pollfd* pfd);
    void disconnectClient(pollfd* pfd);
    void sendString(Client* c, const string& s);
    Client* findClient(string name);
    Channel* findChannel(string name);
    Channel* createChannel(string name, Channel* ins, Client* owner);
    map<string, Channel*>* getChannels();
	map<int, Client*>* getClients();
};

extern Server* srv;
