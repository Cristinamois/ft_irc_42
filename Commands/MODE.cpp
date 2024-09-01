#include "../RFC1459.hpp"
#include "../Server.hpp"

void RFC1459_MODE(Client* sender, parsed_cmd_t& pc)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
    REGISTER_COMMAND("MODE", sender);
    Channel* chan;
    Client* cli;
    
    if (!pc.has_words)
        RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
    else if (pc.words[0][0] == '&' || pc.words[0][0] == '#')
    {
        chan = srv->findChannel(pc.words[0]);

        if (!chan)
            RFC1459_ERR_NOSUCHCHANNEL(sender, pc.words[0]);
        else if (!chan->isConnected(sender))
            RFC1459_ERR_NOTONCHANNEL(sender, pc.words[0]);
        else if (pc.words.size() == 1)
        {
            args.push_back(pc.words[0]);
            args.push_back(chan->getModeString());
            srv->sendString(sender, format(RPL_CHANNELMODEIS, args));

            REFRESH_REPLY(sender);
            args.push_back(pc.words[0]);
            args.push_back(chan->getCreationTime());
            srv->sendString(sender, format(RPL_CREATIONTIME, args));
        }
        else if (!chan->isOperator(sender))
            RFC1459_ERR_CHANOPRIVSNEEDED(sender, pc.words[0]);
        else
            chan->setMode(sender, cmd_name, pc.words);
    }
    else
    {
        cli = srv->findClient(pc.words[0]);

        if (!cli)
            RFC1459_ERR_NOSUCHNICK(sender, pc.words[0]);
        else if (sender->getNick().compare(pc.words[0]))
            srv->sendString(sender, format(ERR_USERSDONTMATCH, args));
        else if (pc.words.size() == 1)
        {
            args.push_back(cli->getModeString());
            srv->sendString(sender, format(RPL_UMODEIS, args));
        }
        else
            cli->setMode(pc.words);
    }
}
