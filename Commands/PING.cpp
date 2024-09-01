#include "../RFC1459.hpp"
#include "../Server.hpp"

void RFC1459_PING(Client* sender, parsed_cmd_t& pc)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
    REGISTER_COMMAND("PING", sender);
    size_t i;

    if (!pc.has_words)
        srv->sendString(sender, format(ERR_NOORIGIN, args));
    else
    {
        for (i = 0; i < pc.words.size(); ++i)
        {
            if (pc.words[i].compare(SERVER_NAME))
                RFC1459_ERR_NOSUCHSERVER(sender, pc.words[i]);
            else
            {
                args.clear();
                args.push_back(SERVER_NAME);
                args.push_back(SERVER_NAME);
                args.push_back(SERVER_NAME);
                srv->sendString(sender, format(RPL_PING, args));
                cout << "PING/PONG" << endl;
            }
        }
    }
}
