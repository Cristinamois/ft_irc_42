#pragma once

#include "Client.hpp"
#include "Utilities.hpp"
#include <iostream>

/*
    Les messages IRC sont toujours des lignes de caractères terminés par une paire CR-LF (retour chariot - saut de ligne), 
    et ces messages ne doivent pas dépasser 512 caractères de long, en comptant tous les caractères y compris le CR-LF final.
*/
#define RFC1459_MAX             512
#define RFC1459_END             "\x0D\x0A"

/*
    o - donne/retire les privilèges d'opérateur de canal
    p - drapeau de canal privé
    s - drapeau de canal secret
    i - drapeau de canal accessible uniquement sur invitation
    t - drapeau de sujet de canal modifiable uniquement par les opérateurs
    n - pas de messages dans un canal provenant de clients à l'extérieur du canal
    m - canal modéré
    l - définit le nombre maximal de personnes dans un canal
    v - donne/retire la possibilité de parler dans un canal modéré
    k - définit la clé du canal (mot de passe)
*/
#define RFC1459_CHANNEL_MODES   "opsitnmlvk"

/*
    i - marque un utilisateur comme invisible ;
    s - marque un utilisateur comme recevant les notifications du serveur ;
    o - drapeau d'opérateur.
*/
#define RFC1459_USER_MODES      "iso"

/*
    Les caractères interdits dans un pseudo.
*/
#define RFC1459_FORBIDDEN       "\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F\x3A\x3B\x3C\x3D\x3E\x3F\x40\x5B\x5C\x5D\x5E\x60\x7B\x7C\x7D\x7E"

/*
    La plupart des messages envoyés aux serveurs génèrent une réponse. 
    Les réponses les plus courantes sont des réponses numériques, aussi bien pour les messages d'erreurs que pour les réponses normales.
*/
#define ERR_NEEDMOREPARAMS      ":<server> 461 <snick> <command> :Not enough parameters"
#define ERR_ALREADYREGISTERED   ":<server> 462 <snick> :You may not reregister"
#define ERR_PASSWDMISMATCH      ":<server> 464 <snick> :Password incorrect"
#define ERR_NONICKNAMEGIVEN     ":<server> 431 <snick> :No nickname given"
#define ERR_ERRONEUSNICKNAME    ":<server> 432 <snick> <tnick> :Erroneus nickname"
#define ERR_NICKNAMEINUSE       ":<server> 433 <snick> <tnick> :Nickname is already in use"
#define RPL_NICK                ":<old_nick>!~<user>@<host> NICK :<new_nick>"
#define RPL_WELCOME             ":<server> 001 <snick> :Welcome to the Libera.Chat Internet Relay Chat Network <snick>"
#define RPL_MOTDSTART			":<server> 375 <snick> :-<server> Message of the day-"
#define RPL_MOTD				":<server> 372 <snick> :-\n<text>"
#define RPL_ENDOFMOTD			":<server> 376 <snick> :End of /MOTD command"
#define RPL_YOURHOST            ":<server> 002 <snick> :Your host is <server>[0.0.0.0/<port>], running version 1.0.0"
#define RPL_CREATED             ":<server> 003 <snick> :This server was created <date> at <time>"
#define RPL_MYINFO              ":<server> 004 <snick> <server> 1.0.0 <user_modes> <chan_modes>"
#define ERR_NOSUCHSERVER        ":<server> 402 <snick> <tserver> :No such server"
#define RPL_TIME                ":<server> 391 <snick> <server> :<dt>"
#define ERR_NOORIGIN            ":<server> 409 <snick> :No origin specified"
#define RPL_PING                ":<server> PONG <server> :<server>"
#define RPL_QUIT                ":<snick>!~<user>@<host> QUIT :<reason>"
#define ERR_NOSUCHNICK          ":<server> 401 <snick> <pseudonyme> :No such nick/channel"
#define ERR_NOSUCHCHANNEL       ":<server> 403 <snick> <canal> :No such channel"
#define ERR_NOTONCHANNEL        ":<server> 442 <snick> <canal> :You're not on that channel"
#define ERR_CHANOPRIVSNEEDED    ":<server> 482 <snick> <canal> :You're not a channel operator"
#define RPL_TOPIC               ":<snick>!~<user>@<host> TOPIC <chan> :<topic>"
#define RPL_NOTOPIC             ":<server> 331 <snick> <channel> :No topic is set"
#define RPL_NAMREPLY            ":<server> 353 <snick> @ <chan> :<chan_nicks>"
#define RPL_ENDOFNAMES          ":<server> 366 <snick> <chan> :End of /NAMES list."
#define ERR_INVITEONLYCHAN      ":<server> 473 <snick> <canal> :Cannot join channel (+i)"
#define ERR_BADCHANNELKEY       ":<server> 475 <snick> <canal> :Cannot join channel (+k)"
#define ERR_CHANNELISFULL       ":<server> 471 <snick> <canal> :Cannot join channel (+l)"
#define RPL_JOIN                ":<snick>!~<user>@<host> JOIN <chan>"
#define RPL_PART                ":<snick>!~<user>@<host> PART <chan>"
#define ERR_USERSDONTMATCH      ":<server> 502 <snick> :Cant change mode for other users"
#define RPL_CHANNELMODEIS       ":<server> 324 <snick> <canal> <chan_mode>"
#define RPL_CREATIONTIME        ":<server> 329 <snick> <canal> <timestamp>"
#define RPL_UMODEIS             ":<server> 221 <snick> <user_mode>"
#define ERR_UNKNOWNMODE         ":<server> 472 <snick> <mode> :is unknown mode to me"
#define RPL_USER_MODE           ":<snick> MODE <snick> :<mode_changes>"
#define RPL_CHANNEL_MODE        ":<snick>!~<user>@<host> MODE <canal> <mode_changes>"
#define RPL_YOUREOPER           ":<server> 381 <snick> :You are now an IRC operator"
#define RPL_KILL                ":<server> KILL <target> :<reason>"
#define ERR_NOPRIVILEGES        ":<server> 481 <snick> :Permission Denied- You're not an IRC operator"
#define ERR_NORECIPIENT         ":<server> 411 <snick> :No recipient given"
#define ERR_NOTEXTTOSEND        ":<server> 412 <snick> :No text to send"
#define ERR_TOOMANYTARGETS      ":<server> 407 <snick> <destination> :Duplicate recipients. No message delivered"
#define ERR_CANNOTSENDTOCHAN    ":<server> 404 <snick> <canal> :Cannot send to channel"
#define RPL_PRIVMSG             ":<snick>!~<user>@<host> PRIVMSG <target> :<msg>"
#define RPL_NOTICE              ":<snick>!~<user>@<host> NOTICE <target> :<msg>"
#define ERR_USERONCHANNEL       ":<server> 443 <snick> <utilisateur> <channel> :is already on channel"
#define RPL_INVITING            ":<server> 341 <snick> <client> <channel>"
#define RPL_INVITE              ":<snick>!~<user>@<host> INVITE <client> :<channel>"
#define RPL_LISTSTART           ":<server> 321 <snick> Channel :Users  Name"
#define RPL_LIST                ":<server> 322 <snick> <channel> <vis_num> :<topic>"
#define RPL_LISTEND             ":<server> 323 <snick> :End of /LIST"
#define ERR_NOTREGISTERED       ":<server> 451 <snick> :You have not registered"

/*
    Les macros qui nous feront gagner du temps sur chaque commande.
*/
#define REFRESH_REPLY(sender)   args.clear(); args.push_back(SERVER_NAME); args.push_back(sender->getNick())
#define REGISTER_REPLY(sender)  vector<string> args; REFRESH_REPLY(sender)
#define REGISTER_COMMAND(n, s)  string cmd_name = (n); REGISTER_REPLY(s);

typedef void(*cmd_t)(Client*, parsed_cmd_t&);

/*
    Les wrappers pour les différentes réponses des commandes qui sont 
    utilisées assez fréquemment.
*/
void RFC1459_ERR_NEEDMOREPARAMS(Client* sender, const string& command);
void RFC1459_ERR_ALREADYREGISTRED(Client* sender);
void RFC1459_ERR_PASSWDMISMATCH(Client* sender);
void RFC1459_ERR_NONICKNAMEGIVEN(Client* sender);
void RFC1459_ERR_NOSUCHSERVER(Client* sender, const string& server);
void RFC1459_ERR_NOSUCHNICK(Client* sender, const string& nick);
void RFC1459_ERR_NOSUCHCHANNEL(Client* sender, const string& channel);
void RFC1459_ERR_NOTONCHANNEL(Client* sender, const string& channel);
void RFC1459_ERR_CHANOPRIVSNEEDED(Client* sender, const string& channel);
void RFC1459_ERR_UNKNOWNMODE(Client* sender, string mode);
void RFC1459_ERR_NOTREGISTERED(Client* sender);
bool ensureAuth(Client* c);

/*
    Les commandes demandées par le sujet.
*/
void RFC1459_PASS(Client* sender, parsed_cmd_t& pc);
void RFC1459_NICK(Client* sender, parsed_cmd_t& pc);
void RFC1459_USER(Client* sender, parsed_cmd_t& pc);
void RFC1459_MOTD(Client* sender, parsed_cmd_t& pc);
void RFC1459_TIME(Client* sender, parsed_cmd_t& pc);
void RFC1459_PING(Client* sender, parsed_cmd_t& pc);
void RFC1459_QUIT(Client* sender, parsed_cmd_t& pc);
void RFC1459_TOPIC(Client* sender, parsed_cmd_t& pc);
void RFC1459_NAMES(Client* sender, parsed_cmd_t& pc);
void RFC1459_JOIN(Client* sender, parsed_cmd_t& pc);
void RFC1459_PART(Client* sender, parsed_cmd_t& pc);
void RFC1459_MODE(Client* sender, parsed_cmd_t& pc);
void RFC1459_OPER(Client* sender, parsed_cmd_t& pc);
void RFC1459_KILL(Client* sender, parsed_cmd_t& pc);
void RFC1459_KICK(Client* sender, parsed_cmd_t& pc);
void RFC1459_PRIVMSG(Client* sender, parsed_cmd_t& pc);
void RFC1459_NOTICE(Client* sender, parsed_cmd_t& pc);
void RFC1459_INVITE(Client* sender, parsed_cmd_t& pc);
void RFC1459_LIST(Client* sender, parsed_cmd_t& pc);
