#include "../RFC1459.hpp"
#include "../Server.hpp"

void RFC1459_INVITE(Client* sender, parsed_cmd_t& pc)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
    REGISTER_COMMAND("INVITE", sender);
    Client* cli;
    Channel* chan;
    channel_mode_t* cm;

    if (!pc.has_words || pc.words.size() != 2)
        RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
    else
    {
        cli = srv->findClient(pc.words[0]);
        chan = srv->findChannel(pc.words[1]);
        cm = chan->getMode();

        if (!cli)
            RFC1459_ERR_NOSUCHNICK(sender, pc.words[0]);
        else if (!chan)
            return;
        // else if (cm->i && !chan->isOperator(sender))
        else if (cm->i && (!chan->isOperator(sender) && !chan->isVoiced(sender)))
            RFC1459_ERR_CHANOPRIVSNEEDED(sender, pc.words[1]);
        else if (!cm->i && !chan->isConnected(sender))
            RFC1459_ERR_NOTONCHANNEL(sender, pc.words[1]);
        else if (chan->isConnected(cli))
        {
            args.push_back(pc.words[0]);
            args.push_back(pc.words[1]);
            srv->sendString(sender, format(ERR_USERONCHANNEL, args));
        }
        else
        {
            args.push_back(pc.words[0]);
            args.push_back(pc.words[1]);
            srv->sendString(sender, format(RPL_INVITING, args));

            args.clear();
            args.push_back(sender->getNick());
            args.push_back(sender->getUser());
            args.push_back(sender->getHost());
            args.push_back(pc.words[0]);
            args.push_back(pc.words[1]);
            srv->sendString(cli, format(RPL_INVITE, args));

            chan->addInvited(cli);
        }
    }
}
