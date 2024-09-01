#include "../RFC1459.hpp"
#include "../Server.hpp"

void RFC1459_TOPIC(Client* sender, parsed_cmd_t& pc)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
    REGISTER_COMMAND("TOPIC", sender);
    Channel* chan;

    if (!pc.has_words)
    {
        RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
        return;
    }
    
    chan = srv->findChannel(pc.words[0]);
    
    if (!chan)
        RFC1459_ERR_NOSUCHCHANNEL(sender, pc.words[0]);
    else
    {
        if (!chan->onChannel(sender->getFd()))
        {
            RFC1459_ERR_NOTONCHANNEL(sender, pc.words[0]);
            return;
        }

        if (!pc.has_suffix)
        {
            if (chan->getTopic().empty())
            {
                args.push_back(pc.words[0]);
                srv->sendString(sender, format(RPL_NOTOPIC, args));
                return;
            }
        }
        else
        {
            // if (chan->getMode()->t && !chan->isOperator(sender))
            if (chan->getMode()->t && (!chan->isOperator(sender) && !chan->isVoiced(sender)))
            {
                RFC1459_ERR_CHANOPRIVSNEEDED(sender, pc.words[0]);
                return;
            }
            
            chan->setTopic(pc.suffix);
        }

        args.clear();
        args.push_back(sender->getNick());
        args.push_back(sender->getUser());
        args.push_back(sender->getHost());
        args.push_back(pc.words[0]);
        args.push_back(chan->getTopic());
        chan->Broadcast(format(RPL_TOPIC, args), NULL);
    }
}
