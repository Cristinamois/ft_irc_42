#include "Server.hpp"

Server::Server(uint16_t _port, string _pwd)
    : port(_port), pwd(_pwd), fd(-1), size(0), stop(false)
{
    bzero(pfd, sizeof(pfd));
}

Server::~Server()
{
    size_t i;
    map<string, Channel*>::iterator it;

    // close server
    close(pfd[0].fd);
    pfd[0].fd = -1;

    // close clients
    for (i = 1; i < size; ++i)
        disconnectClient(pfd + i);

    // close channels
    for (it = channels.begin(); it != channels.end(); ++it)
        delete it->second;
}

uint16_t Server::getPort() {
    return port;
}

string Server::getPwd() {
    return pwd;
}

int Server::getFd() {
    return fd;
}

pollfd* Server::getPfd(int i)
{
    if (i < 0 || i >= SOMAXCONN)
        return NULL;
    return pfd + i;
}

size_t Server::getSize() {
    return size;
}

bool Server::getStop() {
    return  stop;
}

void Server::setStop(bool _stop) {
    stop = _stop;
}

bool Server::start()
{
	struct addrinfo hints, *ai, *p;
	int opt = 1;
	string _hostname = "0.0.0.0";
	stringstream ss;
	ss << port;
	string _port = ss.str();
	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if ((getaddrinfo(_hostname.c_str(), _port.c_str(), &hints, &ai)) != 0) {
		cerr << "getaddrinfo failed\n";
		return false;
	}

	for (p = ai; p != NULL; p = p->ai_next)
	{

		if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			cerr << "Socket failed !\n";
			return false;
		}
		if (fd < 0)
			continue;
		if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
			cerr <<  "setsocket error\n";
			return false;
		}
		if (bind(fd, p->ai_addr, p->ai_addrlen) < 0) {
			close(fd);
			continue;
		}
		break;
	}
	freeaddrinfo(ai);
	if (p == NULL)
		return false;
    
	SERVER_ASSERT(listen(fd, SOMAXCONN));

	pfd[size].fd = fd;
	pfd[size].events = POLLIN;
	++size;

    cmds["PASS"] = &RFC1459_PASS;
    cmds["NICK"] = &RFC1459_NICK;
    cmds["USER"] = &RFC1459_USER;
    cmds["MOTD"] = &RFC1459_MOTD;
	cmds["TIME"] = &RFC1459_TIME;
    cmds["PING"] = &RFC1459_PING;
    cmds["QUIT"] = &RFC1459_QUIT;
    cmds["TOPIC"] = &RFC1459_TOPIC;
    cmds["NAMES"] = &RFC1459_NAMES;
    cmds["JOIN"] = &RFC1459_JOIN;
    cmds["PART"] = &RFC1459_PART;
    cmds["MODE"] = &RFC1459_MODE;
    cmds["OPER"] = &RFC1459_OPER;
    cmds["KILL"] = &RFC1459_KILL;
    cmds["KICK"] = &RFC1459_KICK;
    cmds["PRIVMSG"] = &RFC1459_PRIVMSG;
    cmds["NOTICE"] = &RFC1459_NOTICE;
    cmds["INVITE"] = &RFC1459_INVITE;
    cmds["LIST"] = &RFC1459_LIST;

    return true;
}

void Server::acceptClient()
{
    if (size + 1 >= SOMAXCONN)
    {
        cerr << SERVER_NAME << ": cannot accept another client" << endl;
        return;
    }

    pfd[size].fd = accept(fd, NULL, NULL);
    if (pfd[size].fd == -1)
    {
        cerr << SERVER_NAME << ": could not accept client" << endl;
        return;
    }

    cout << SERVER_NAME << ": accepted connection " << pfd[size].fd << endl;

    pfd[size].events = POLLIN;
    clients[pfd[size].fd] = new Client(pfd + size);
    ++size;
}

void Server::handleRequest(pollfd* pfd)
{
    int retval;
    char buf[RFC1459_MAX];
    vector<string> cmdvec;
    parsed_cmd_t parsed;
    size_t i;
	static string bufs[SOMAXCONN];
    bzero(buf, sizeof(buf));
	
    retval = recv(pfd->fd, buf, sizeof(buf), 0);

    if (retval == -1)
        return;
    else if (!retval)
    {
        disconnectClient(pfd); 
        return;
    }

	bufs[pfd->fd] += buf;

	if (bufs[pfd->fd].find(RFC1459_END) == string::npos)
		return;

    cout << "(" << pfd->fd << ") " << "received:\n" << bufs[pfd->fd] << endl<< endl;
 
    // get and iterate received commands
    cmdvec = split(bufs[pfd->fd], RFC1459_END);
	bufs[pfd->fd].clear();
    for (i = 0; i < cmdvec.size(); ++i)
    {
        parsed = parse(cmdvec[i]);

        if (cmds.count(parsed.cmd) == 0)
            ;
        else
            cmds[parsed.cmd](clients[pfd->fd], parsed);
    }
}

void Server::disconnectClient(pollfd* pfd)
{
	map<string, Channel*>* channels = srv->getChannels();
    map<string, Channel*>::iterator it;
	parsed_cmd_t pc;
	string pc_word;

    if (pfd->fd != -1 && clients.count(pfd->fd) != 0)
    {
        cout << SERVER_NAME << ": lost connection " << pfd->fd << endl;

		for (it = channels->begin(); it != channels->end(); ++it)
        {
			if (it->second->isConnected(clients[pfd->fd]))
			{
				pc_word += it->second->getName();
				pc_word += ",";
			}
        }

		pc.has_words = true;
		pc.words.push_back(pc_word);
		RFC1459_PART(clients[pfd->fd], pc);

        delete clients[pfd->fd];
        clients.erase(pfd->fd);
        close(pfd->fd);
        pfd->fd = -1;
    }
}

void Server::sendString(Client* c, const string& s) {
	cout << "(" << c->getFd() << ") " << "sent:\n" << s << endl<< endl;
    send(c->getFd(), s.c_str(), s.size(), 0);
}

Client* Server::findClient(string name)
{
    map<int, Client*>::iterator it;

    for (it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->second->getNick() == name)
            return it->second;
    }
    return NULL;
}

Channel* Server::findChannel(string name)
{
    map<string, Channel*>::iterator it;

    for (it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->second->getName() == name)
            return it->second;
    }
    return NULL;
}

Channel* Server::createChannel(string name, Channel* ins, Client* owner)
{
    channels[name] = ins;
    ins->addOperator(owner);
    return ins;
}

map<string, Channel*>* Server::getChannels() {
    return &channels;
}

map<int, Client*>* Server::getClients() {
	return &clients;
}

Server* srv;
