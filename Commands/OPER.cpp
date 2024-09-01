#include "../RFC1459.hpp"
#include "../Server.hpp"

void RFC1459_OPER(Client* sender, parsed_cmd_t& pc)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
    REGISTER_COMMAND("OPER", sender);
    Client* cli;

    if (!pc.has_words || pc.words.size() != 2)
        RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
    else if (pc.words[1].compare(srv->getPwd()))
        RFC1459_ERR_PASSWDMISMATCH(sender);
    else
    {
        cli = srv->findClient(pc.words[0]);

        if (!cli)
            RFC1459_ERR_NOSUCHNICK(sender, pc.words[0]);
        else
        {
            cli->getMode()->o = true;
            srv->sendString(sender, format(RPL_YOUREOPER, args));    
        }
    }
}
