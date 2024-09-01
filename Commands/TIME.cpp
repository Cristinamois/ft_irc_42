#include "../RFC1459.hpp"
#include "../Server.hpp"

void RFC1459_TIME(Client* sender, parsed_cmd_t& pc)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
    REGISTER_COMMAND("TIME", sender);

    if (!pc.has_words || !pc.words[0].compare(SERVER_NAME))
    {
        args.push_back(SERVER_NAME);
        args.push_back(getDateAndTime(true, true));
        srv->sendString(sender, format(RPL_TIME, args));
    }
    else
        RFC1459_ERR_NOSUCHSERVER(sender, pc.words[0]);
}
