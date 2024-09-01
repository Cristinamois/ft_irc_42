#include "../RFC1459.hpp"
#include "../Server.hpp"

static bool is_valid_nick(const string& s) {
    return s.find_first_of(RFC1459_FORBIDDEN) == string::npos;
}

static bool fix_invalid_nick(string& s)
{
    size_t pos = 0, cpy = s.size();

    for (size_t i = 0; i < cpy; ++i)
    {
        pos = s.find_first_of(RFC1459_FORBIDDEN);
        if (pos != string::npos)
            s.erase(pos, 1);
    }

    return (s.empty() == true) ? false : true;
}

void RFC1459_NICK(Client* sender, parsed_cmd_t& pc)
{
	if (!sender->hasPwd())
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
    REGISTER_COMMAND("NICK", sender);
    string unused;

    if (!pc.has_words)
    {
        RFC1459_ERR_NONICKNAMEGIVEN(sender);
        return;
    }
    else if (!is_valid_nick(pc.words[0]))
    {
        args.push_back(pc.words[0]);
        srv->sendString(sender, format(ERR_ERRONEUSNICKNAME, args));

        if (!fix_invalid_nick(pc.words[0]))
            goto random_nick;
    }

    if (srv->findClient(pc.words[0]))
    {
        REFRESH_REPLY(sender);
        args.push_back(pc.words[0]);
        srv->sendString(sender, format(ERR_NICKNAMEINUSE, args));

        if (!sender->getUser().compare("*"))
        {
random_nick:
            unused = pc.words[0] + "_";
            while (srv->findClient(unused))
                unused += "_";
            pc.words[0] = unused;
        }
        else
            return;
    }


    if (sender->getUser().compare("*"))
    {
        args.clear();
        args.push_back(sender->getNick());
        args.push_back(sender->getUser());
        args.push_back(sender->getHost());
        args.push_back(pc.words[0]);
        srv->sendString(sender, format(RPL_NICK, args));
    }

    sender->setNick(pc.words[0]);
    cout << "nick has been set to "  << pc.words[0] << endl;
}
