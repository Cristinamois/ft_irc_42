#include "../RFC1459.hpp"
#include "../Server.hpp"

void RFC1459_LIST(Client* sender, parsed_cmd_t& pc)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
    REGISTER_COMMAND("LIST", sender);
    vector<string> canals;
    size_t i;
    map<string, Channel*>* channels;
    map<string, Channel*>::iterator it;
    channel_mode_t* cm;
    Channel* chan;

    srv->sendString(sender, format(RPL_LISTSTART, args));

    if (!pc.has_words)
    {
        channels = srv->getChannels();

        for (it = channels->begin(); it != channels->end(); ++it)
        {
            cm = it->second->getMode();

            REFRESH_REPLY(sender);
            if (it->second->isConnected(sender) || (!cm->p && !cm->s))
            {
                args.push_back(it->second->getName());
                args.push_back(it->second->getVisibleCount());
                args.push_back(it->second->getTopic());
            }
            else if (cm->p)
            {
                args.push_back("Prv");
                args.push_back(it->second->getVisibleCount());
                args.push_back("");
            }
            else
                continue;

            srv->sendString(sender, format(RPL_LIST, args));
        }
    }
    else
    {
        canals = split(pc.words[0], ",");

        for (i = 0; i < canals.size(); ++i)
        {
            chan = srv->findChannel(canals[i]);

            if (!chan)
                continue;

            cm = chan->getMode();

            REFRESH_REPLY(sender);
            if (chan->isConnected(sender) || (!cm->p && !cm->s))
            {
                args.push_back(chan->getName());
                args.push_back(chan->getVisibleCount());
                args.push_back(chan->getTopic());
            }
            else if (cm->p)
            {
                args.push_back("Prv");
                args.push_back(chan->getVisibleCount());
                args.push_back("");
            }
            else
                continue;

            srv->sendString(sender, format(RPL_LIST, args));
        }
    }

    REFRESH_REPLY(sender);
    srv->sendString(sender, format(RPL_LISTEND, args));
}
