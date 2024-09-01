#include "../RFC1459.hpp"
#include "../Server.hpp"
#include <sstream>

void RFC1459_USER(Client* sender, parsed_cmd_t& pc)
{
	if (!sender->hasPwd() || !sender->getNick().compare("*"))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
    REGISTER_COMMAND("USER", sender);
    stringstream ss;
    string date, time;
    
    if (sender->hasAuthd())
        RFC1459_ERR_ALREADYREGISTRED(sender);
    else if (!pc.has_words || pc.words.size() != 3  || !pc.has_suffix)
        RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
    else
    {
        sender->setUser(pc.words[0]);
        sender->setHost(pc.words[1]);
        sender->setRealName(pc.suffix);

        args.push_back(sender->getNick());
        srv->sendString(sender, format(RPL_WELCOME, args));

        REFRESH_REPLY(sender);
        args.push_back(SERVER_NAME);
        ss << srv->getPort();
        args.push_back(ss.str());
        srv->sendString(sender, format(RPL_YOURHOST, args));

        date = getDateAndTime(true, false);
        time = getDateAndTime(false, true);

        REFRESH_REPLY(sender);
        args.push_back(date);
        args.push_back(time);
        srv->sendString(sender, format(RPL_CREATED, args));

        REFRESH_REPLY(sender);
        args.push_back(SERVER_NAME);
        args.push_back(RFC1459_USER_MODES);
        args.push_back(RFC1459_CHANNEL_MODES);
        srv->sendString(sender, format(RPL_MYINFO, args));

		RFC1459_MOTD(sender, pc);

        sender->hasAuthd(true);
    }
}
