#include "../RFC1459.hpp"
#include "../Server.hpp"

void RFC1459_PASS(Client* sender, parsed_cmd_t& pc)
{
    REGISTER_COMMAND("PASS", sender);

    if (sender->hasPwd())
        RFC1459_ERR_ALREADYREGISTRED(sender);
    else if (!pc.has_words)
        RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
    else if (pc.words[0] != srv->getPwd())
        RFC1459_ERR_PASSWDMISMATCH(sender);
    else
        sender->hasPwd(true);
}
