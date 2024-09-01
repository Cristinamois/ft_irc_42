#include "../RFC1459.hpp"
#include "../Server.hpp"

void RFC1459_PART(Client* sender, parsed_cmd_t& pc)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
    REGISTER_COMMAND("PART", sender);
    vector<string> canals;
    size_t i;
    Channel* chan;

    if (!pc.has_words)
        RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
    else
    {
        canals = split(pc.words[0], ",");

        for (i = 0; i < canals.size(); ++i)
        {
            chan = srv->findChannel(canals[i]);

            if (!chan)
            {
                RFC1459_ERR_NOSUCHCHANNEL(sender, canals[i]);
                continue;
            }

            if (!chan->isConnected(sender))
            {
                RFC1459_ERR_NOTONCHANNEL(sender, canals[i]);
                continue;
            }

            args.clear();
            args.push_back(sender->getNick());
            args.push_back(sender->getUser());
            args.push_back(sender->getHost());
            args.push_back(canals[i]);
            chan->Broadcast(format(RPL_PART, args), NULL);

            chan->disconnectUser(sender);
        }
    }
}
