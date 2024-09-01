#pragma once

#include <string>
#include <map>
#include "Client.hpp"

using namespace std;

typedef struct channel_mode_s
{
    bool p, s, i, t, n, m, lbool, kbool;
    int l;
    string k;
} channel_mode_t;

class Channel
{
private:
    channel_mode_t modes;
    string name, topic, creation;
    map<int, Client*> users;
    map<int, Client*> operators;
    map<int, Client*> invited;
    map<int, Client*> voiced; 
    
public:
    Channel(string _name);
    ~Channel();

    string getName();
    string getTopic();
    void setTopic(string _topic);
    channel_mode_t* getMode();
    bool onChannel(int client_fd);
    void addUser(Client* c);
    void addOperator(Client* c);
    void addVoiced(Client* c);
    void removeUser(Client* c);
    void removeOperator(Client* c);
    void removeVoiced(Client* c);
    bool isUser(Client* c);
    bool isOperator(Client* c);
    bool isVoiced(Client* c);
    bool isConnected(Client* c);
    bool isInvited(Client* c);
    void addInvited(Client* c);
    void removeInvitation(Client* c);
    string getUserList(bool connected);
    int getSize();
    void Broadcast(const string& s, Client* ignore);
    string getModeString();
    string getCreationTime();
    void setMode(Client* sender, const string& cmd_name, vector<string>& words);
    void disconnectUser(Client* c);
    string getVisibleCount();
};


/* Channel mode :

	+p :	The channel is private and won't be shown in the channel list or user listing
			who are not members of the channel
	+s :	The channel is secret and won't be shown in the channel list even for the
			members of the channel
	
	+i : 	The channel is accessible only on invitation

	+t : 	The channel is topic only, only the operators of the channel can change its topic

	+n :	The channel does not accept external messages. Only users who are part of the
			channel can send messages

	+m :	The channel is moderated, it means that only certain users or operators can send
			messages.
*/
