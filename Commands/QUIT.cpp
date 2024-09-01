#include "../RFC1459.hpp"
#include "../Server.hpp"

void RFC1459_QUIT(Client* sender, parsed_cmd_t& pc)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}

    REGISTER_COMMAND("QUIT", sender);
    string reason = "Client Quit";
	map<string, Channel*> *chan;
	map<string, Channel*>::iterator it;

    if (pc.has_suffix)
		reason = pc.suffix;
    
    args.clear();
    args.push_back(sender->getNick());
    args.push_back(sender->getUser());
    args.push_back(sender->getHost());
    args.push_back(reason);
	srv->sendString(sender, format(RPL_QUIT, args));
    
	chan = srv->getChannels();
	it = chan->begin();
	for (; it != chan->end(); ++it) {
		
		if (it->second->onChannel(sender->getFd()))
			it->second->Broadcast(format(RPL_QUIT, args), sender);
	}

    srv->disconnectClient(sender->getPfd());

}
