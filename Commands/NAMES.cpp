#include "../RFC1459.hpp"
#include "../Server.hpp"

static bool is_channel_visible(Channel* chan, Client* cli)
{
	channel_mode_t* cm = chan->getMode();
	return (!cm->p && !cm->s) || chan->isConnected(cli);
}

void RFC1459_NAMES(Client* sender, parsed_cmd_t& pc)
{	
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
    REGISTER_COMMAND("NAMES", sender);
	vector<string> canals;
	size_t i;
	Channel* chan;
	map<string, Channel*>* channels = srv->getChannels();
    map<string, Channel*>::iterator it;
	map<int, Client*>* clients = srv->getClients();
	map<int, Client*>::iterator it2;
	string user = "";

    if (!pc.has_words)
    {
        for (it = channels->begin(); it != channels->end(); ++it)
        {
			if (!is_channel_visible(it->second, sender))
				continue;
			
			REFRESH_REPLY(sender);
			args.push_back(it->second->getName());
			args.push_back(it->second->getUserList(it->second->isConnected(sender)));
			srv->sendString(sender, format(RPL_NAMREPLY, args));

			REFRESH_REPLY(sender);
			args.push_back(it->second->getName());
			srv->sendString(sender, format(RPL_ENDOFNAMES, args));
        }

		for (it2 = clients->begin(); it2 != clients->end(); ++it2)
        {
			for (it = channels->begin(); it != channels->end(); ++it)
			{
				if (is_channel_visible(it->second, sender))
				{
					if (it->second->isConnected(it2->second))
						break;
				}
			}

			if (it == channels->end() && !it2->second->getMode()->i)
			{
				user += it2->second->getNick();
				user += " ";
			}
        }

		if (!user.empty())
		{
			REFRESH_REPLY(sender);
			args.push_back("#*");
			args.push_back(user);
			srv->sendString(sender, format(RPL_NAMREPLY, args));

			REFRESH_REPLY(sender);
			args.push_back("#*");
			srv->sendString(sender, format(RPL_ENDOFNAMES, args));
		}
    }
    else
    {
		canals = split(pc.words[0], ",");

		for (i = 0; i < canals.size(); ++i)
		{
			chan = srv->findChannel(canals[i]);

			if (!chan || !is_channel_visible(chan, sender))
				continue;

			REFRESH_REPLY(sender);
			args.push_back(canals[i]);
			args.push_back(chan->getUserList(chan->isConnected(sender)));
			srv->sendString(sender, format(RPL_NAMREPLY, args));

			REFRESH_REPLY(sender);
			args.push_back(canals[i]);
			srv->sendString(sender, format(RPL_ENDOFNAMES, args));
		}
    }
}
