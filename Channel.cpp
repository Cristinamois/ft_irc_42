#include "Channel.hpp"
#include <string>
#include "Server.hpp"
#include <sstream>
#include <cstdlib>

using namespace std;

Channel::Channel(string _name)
    : name(_name), topic("")
{
    modes.p = false;
    modes.s = false;
    modes.i = false;
    modes.t = true;
    modes.n = true;
    modes.m = false;
    modes.lbool = false;
    modes.kbool = false;
    modes.l = SOMAXCONN; // Maximal number of users in a channel
    modes.k = ""; // Password for the channel

    time_t seconds = time(NULL);
	std::stringstream ss;
	ss << seconds;

    creation = ss.str();
}

Channel::~Channel()
{
}

string Channel::getName() {
    return name;
}


string Channel::getTopic() {
    return topic;
}

void Channel::setTopic(string _topic) {
    topic = _topic;
}

channel_mode_t* Channel::getMode() {
    return &modes;
}

bool Channel::onChannel(int client_fd) {
    return (users.count(client_fd) || operators.count(client_fd) || voiced.count(client_fd)) ? true : false;
}

void Channel::addUser(Client* c)
{
    users[c->getFd()] = c;
    removeOperator(c);
    removeVoiced(c);
}

void Channel::addOperator(Client* c)
{
    operators[c->getFd()] = c;
    removeUser(c);
    removeVoiced(c);
}

void Channel::removeUser(Client* c)
{
    if (users.count(c->getFd()))
        users.erase(c->getFd());
}

void Channel::removeOperator(Client* c)
{
    if (operators.count(c->getFd()))
        operators.erase(c->getFd());
}

bool Channel::isUser(Client* c) {
    return users.count(c->getFd()) != 0;
}

bool Channel::isOperator(Client* c) {
    return operators.count(c->getFd()) != 0;
}

bool Channel::isConnected(Client* c) {
    return (isUser(c) || isOperator(c) || isVoiced(c));
}

bool Channel::isInvited(Client* c) {
    return invited.count(c->getFd()) != 0;
}

void Channel::addInvited(Client* c) {
    invited[c->getFd()] = c;
}

void Channel::removeInvitation(Client* c)
{
    if (invited.count(c->getFd()) != 0)
        invited.erase(c->getFd());
}

string Channel::getUserList(bool connected)
{
    string ret = "";
    map<int, Client*>::iterator it;

    for (it = users.begin(); it != users.end(); ++it)
    {
		if (!connected && it->second->getMode()->i)
			continue;
        ret += it->second->getNick();
        ret += " ";
    }
    for (it = operators.begin(); it != operators.end(); ++it)
    {
		if (!connected && it->second->getMode()->i)
			continue;
        ret += "@";
        ret += it->second->getNick();
        ret += " ";
    }
    for (it = voiced.begin(); it != voiced.end(); ++it)
    {
		if (!connected && it->second->getMode()->i)
			continue;
        ret += "+";
        ret += it->second->getNick();
        ret += " ";
    }

    return ret;
}

int Channel::getSize()
{
    int ret = 0;
    map<int, Client*>::iterator it;

    for (it = users.begin(); it != users.end(); ++it)
        ++ret;
    for (it = operators.begin(); it != operators.end(); ++it)
        ++ret;
    for (it = voiced.begin(); it != voiced.end(); ++it)
        ++ret;

    return ret;
}

void Channel::Broadcast(const string& s, Client* ignore)
{
    map<int, Client*>::iterator it;
    client_mode_t* cm;

    for (it = users.begin(); it != users.end(); ++it)
    {
        cm = it->second->getMode();
        if (cm->s && it->second != ignore)
            srv->sendString(it->second, s);
    }

    for (it = operators.begin(); it != operators.end(); ++it)
    {
        cm = it->second->getMode();
        if (cm->s && it->second != ignore)
            srv->sendString(it->second, s);
    }

    for (it = voiced.begin(); it != voiced.end(); ++it)
    {
        cm = it->second->getMode();
        if (cm->s && it->second != ignore)
            srv->sendString(it->second, s);
    }
}

string Channel::getModeString()
{
    string ret = "+";

    if (modes.p)
        ret += "p";
    if (modes.s)
        ret += "s";
    if (modes.i)
        ret += "i";
    if (modes.t)
        ret += "t";
    if (modes.n)
        ret += "n";
    if (modes.m)
        ret += "m";
    if (modes.lbool)
        ret += "l";
    if (modes.kbool && !modes.k.empty())
        ret += "k";
    
    return ret;
}

string Channel::getCreationTime() {
    return creation;
}

void Channel::setMode(Client* sender, const string& cmd_name, vector<string>& words)
{
    size_t i;
    parsed_mode_t pm = parse_mode(words[1], false);
    vector<string> args;
    string changes;
    Client* target;

    if (!pm.valid)
        RFC1459_ERR_UNKNOWNMODE(sender, pm.invalid_mode);
    else if (pm.modes.find_first_of("ov") != string::npos && (pm.modes.size() != 1 || words.size() < 3))
        RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
    else
    {
        for (i = 0; i < pm.modes.size(); ++i)
        {
            if (pm.modes[i] == 'o')
            {
                target = srv->findClient(words[2]);

                if (!target || !isConnected(target))
                {
                    RFC1459_ERR_NOSUCHNICK(sender, words[2]);
                    return;
                }

                if (pm.sign)
                    addOperator(target);
                else
                    addUser(target);
            }
            else if (pm.modes[i] == 'v')
            {
                target = srv->findClient(words[2]);

                if (!target || !isConnected(target))
                {
                    RFC1459_ERR_NOSUCHNICK(sender, words[2]);
                    return;
                }

                if (pm.sign)
                    addVoiced(target);
                else
                    addUser(target);
            }
            else if (pm.modes[i] == 'l') {
                if (pm.sign) {
                    if (pm.modes.size() != 1 || words.size() < 3)
                        RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
                    else
                        modes.l = std::atoi(words[2].c_str());
                }
                else
                    modes.l = SOMAXCONN;
                modes.lbool = pm.sign;
            }
            else if (pm.modes[i] == 'k') {
                if (pm.sign) {
                    if (pm.modes.size() != 1 || words.size() < 3)
                        RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
                    else
                        modes.k = words[2];
                }
                else
                    modes.k = "";
                modes.kbool = pm.sign;
            }
            else if (pm.modes[i] == 'p')
                modes.p = pm.sign;
            else if (pm.modes[i] == 's')
                modes.s = pm.sign;
            else if (pm.modes[i] == 'i')
                modes.i = pm.sign;
            else if (pm.modes[i] == 't')
                modes.t = pm.sign;
            else if (pm.modes[i] == 'n')
                modes.n = pm.sign;
            else if (pm.modes[i] == 'm')
                modes.m = pm.sign;
        }

        args.push_back(sender->getNick());
        args.push_back(sender->getUser());
        args.push_back(sender->getHost());
        args.push_back(name);
        changes = "-";
        if (pm.sign)
            changes = "+";
        changes += pm.modes;
        args.push_back(changes);
        Broadcast(format(RPL_CHANNEL_MODE, args), NULL);
    }
}

void Channel::disconnectUser(Client* c)
{
    removeUser(c);
    removeOperator(c);
    removeVoiced(c);

    if (!getSize())
    {
        srv->getChannels()->erase(name);
        delete this;
    }
}

string Channel::getVisibleCount()
{
    stringstream ss;
    int count = 0;
    map<int, Client*>::iterator it;

    for (it = users.begin(); it != users.end(); ++it)
    {
        if (!it->second->getMode()->i)
            ++count;
    }
    for (it = operators.begin(); it != operators.end(); ++it)
    {
        if (!it->second->getMode()->i)
            ++count;
    }
    for (it = voiced.begin(); it != voiced.end(); ++it)
    {
        if (!it->second->getMode()->i)
            ++count;
    }

    ss << count;
    return ss.str();
}

void Channel::addVoiced(Client* c)
{
    voiced[c->getFd()] = c;
    removeUser(c);
    removeOperator(c);
}

void Channel::removeVoiced(Client* c)
{
    if (voiced.count(c->getFd()))
        voiced.erase(c->getFd());
}

bool Channel::isVoiced(Client* c) {
    return voiced.count(c->getFd()) != 0;
}

