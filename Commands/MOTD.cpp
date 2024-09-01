#include "../RFC1459.hpp"
#include "../Server.hpp"
#include <fstream>

std::string	readMOTD(void) {

	std::ifstream	infile;
	std::string		motd;
	std::string		line;
	std::string		format = ":ircserv 372 archi :-";


	infile.open("bob_rc.motd");
	if (!infile.is_open())
		return ("");
	else {

		while (getline(infile, line)) {
			motd.append(format);
			motd.append(line);
			motd.append("\n");
		}
		return (motd);
	}
}
		
		
void RFC1459_MOTD(Client *sender, parsed_cmd_t& pc) {

	std::string motd = readMOTD();

	if (motd.empty())
		return;

	REGISTER_COMMAND("MOTD", sender);

	if (!sender->hasAuthd() || !pc.has_words) {

	REFRESH_REPLY(sender);
	args.clear();
	args.push_back(SERVER_NAME);
	args.push_back(sender->getNick());
	args.push_back(SERVER_NAME);
	srv->sendString(sender, format(RPL_MOTDSTART, args));

	REFRESH_REPLY(sender);
	args.clear();
	args.push_back(SERVER_NAME);
	args.push_back(sender->getNick());
	args.push_back(motd);
	srv->sendString(sender, format(RPL_MOTD, args));

	REFRESH_REPLY(sender);
	args.clear();
	args.push_back(SERVER_NAME);
	args.push_back(sender->getNick());
	srv->sendString(sender, format(RPL_ENDOFMOTD, args));
	
	}

	return ;
}
