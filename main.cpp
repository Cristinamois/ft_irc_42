#include <iostream>
#include "Server.hpp"
#include <cstdlib>
#include <signal.h>

static void sighandler(int sig)
{
    if (sig == SIGINT)
        srv->setStop(true);
}

int main(int ac, const char* const av[])
{
    size_t cpy, i;
    pollfd* pfd;

    // wrong usage
    if (ac != 3)
    {
        cerr << "usage: ./" << SERVER_NAME << " <port> <password>" << endl;
        return 1;
    }

	int value = strtol(av[1], NULL, 10);
	if (value < 1024 || value > 65535) {
		std::cout << "ERROR: INVALID PORT VALUE" << std::endl;
		return 2;
	}
    // create server instance
    srv = new Server((uint16_t)atoi(av[1]), av[2]);

    // start it
    if (!srv->start())
    {
        cerr << SERVER_NAME << ": could not start server" << endl;
        delete srv;
		return 3;
    }
    else
    {
        cout << SERVER_NAME << ": server started" << endl;

        // setup signal handler
        signal(SIGINT, &sighandler);

        // main server loop
        while (!srv->getStop())
        {
            cpy = srv->getSize();

            if (poll(srv->getPfd(0), cpy, 0) == -1)
                break;

            for (i = 0; i < cpy; ++i)
            {
                pfd = srv->getPfd((int)i);

                if (!pfd || pfd->fd == -1 || pfd->revents != POLLIN)
				    continue;

                if (pfd->fd == srv->getFd())
                    srv->acceptClient();
                else
				{
                    srv->handleRequest(pfd);
				}
            }
        }

        cout << SERVER_NAME << ": server stopped" << endl;
    }

    delete srv;
    return 0;
}

/* static bool fix_invalid_nick(string& s)
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

int main(void)
{
    string test = "_fdgdfgfdgdf";
    fix_invalid_nick(test);
    cout << test << endl;
    return 0;   

} */
