#include "RFC1459.hpp"
#include "Server.hpp"

void RFC1459_ERR_NEEDMOREPARAMS(Client* sender, const string& command)
{
    REGISTER_REPLY(sender);

    args.push_back(command);
    srv->sendString(sender, format(ERR_NEEDMOREPARAMS, args));
}

void RFC1459_ERR_ALREADYREGISTRED(Client* sender)
{
    REGISTER_REPLY(sender);

    srv->sendString(sender, format(ERR_ALREADYREGISTERED, args));
}

void RFC1459_ERR_PASSWDMISMATCH(Client* sender)
{
    REGISTER_REPLY(sender);
    
    srv->sendString(sender, format(ERR_PASSWDMISMATCH, args));
}

void RFC1459_ERR_NONICKNAMEGIVEN(Client* sender)
{
    REGISTER_REPLY(sender);
    
    srv->sendString(sender, format(ERR_NONICKNAMEGIVEN, args));
}

void RFC1459_ERR_NOSUCHSERVER(Client* sender, const string& server)
{
    REGISTER_REPLY(sender);

    args.push_back(server);
    srv->sendString(sender, format(ERR_NOSUCHSERVER, args));
}

void RFC1459_ERR_NOSUCHNICK(Client* sender, const string& nick)
{
    REGISTER_REPLY(sender);

    args.push_back(nick);
    srv->sendString(sender, format(ERR_NOSUCHNICK, args));
}

void RFC1459_ERR_NOSUCHCHANNEL(Client* sender, const string& channel)
{
    REGISTER_REPLY(sender);

    args.push_back(channel);
    srv->sendString(sender, format(ERR_NOSUCHCHANNEL, args));
}

void RFC1459_ERR_NOTONCHANNEL(Client* sender, const string& channel)
{
    REGISTER_REPLY(sender);

    args.push_back(channel);
    srv->sendString(sender, format(ERR_NOTONCHANNEL, args));
}

void RFC1459_ERR_CHANOPRIVSNEEDED(Client* sender, const string& channel)
{
    REGISTER_REPLY(sender);

    args.push_back(channel);
    srv->sendString(sender, format(ERR_CHANOPRIVSNEEDED, args));
}

void RFC1459_ERR_UNKNOWNMODE(Client* sender, string mode)
{
    REGISTER_REPLY(sender);

    args.push_back(mode);
    srv->sendString(sender, format(ERR_UNKNOWNMODE, args));
}

void RFC1459_ERR_NOTREGISTERED(Client* sender)
{
	REGISTER_REPLY(sender);

    srv->sendString(sender, format(ERR_NOTREGISTERED, args));
}

bool ensureAuth(Client* c) {
	return (c->hasPwd() && c->hasAuthd());
}
