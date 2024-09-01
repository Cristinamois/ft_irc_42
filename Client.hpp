#pragma once

#include <string>
#include <vector>

using namespace std;

typedef struct client_mode_s
{
    bool i, s, o;
} client_mode_t;

class Client
{
private:
    struct pollfd* pfd;
    bool has_pwd, has_authd;
    string nick, user, host, real_name;
    client_mode_t modes;

public:
    Client(pollfd* _pfd);
    ~Client();

    pollfd* getPfd();
    int getFd();
    bool hasPwd();
    void hasPwd(bool _has_pwd);
    bool hasAuthd();
    void hasAuthd(bool _has_authd);
    string getNick();
    void setNick(const string& _nick);
    string getUser();
    void setUser(const string& _user);
    string getHost();
    void setHost(const string& _host);
    string getRealName();
    void setRealName(const string& _real_name);
    client_mode_t* getMode();
    string getModeString();
    void setMode(vector<string>& words);
};

/* User mode :

	+i :	The user is invisible. Their presence and information are hidden from other users

	+s :	This user get the notification from the server

	+w :	This user is operator of a specific channel

*/
