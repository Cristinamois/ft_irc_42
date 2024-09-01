#include "../RFC1459.hpp"
#include "../Server.hpp"

void RFC1459_KILL(Client* sender, parsed_cmd_t& pc)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
    REGISTER_COMMAND("KILL", sender);
    Client* target;

    if (!pc.has_words || !pc.has_suffix)
        RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
    else
    {
        if (!sender->getMode()->o)
        {
            srv->sendString(sender, format(ERR_NOPRIVILEGES, args));
            return;
        }
        
        target = srv->findClient(pc.words[0]);

        if (!target)
            RFC1459_ERR_NOSUCHNICK(sender, pc.words[0]);
        else
        {
            args.push_back(pc.suffix);
            srv->sendString(target, format(RPL_KILL, args));

            srv->disconnectClient(target->getPfd());
        }
    }
}
