NAME			:= ircserv
SOURCES			:= main.cpp Server.cpp Channel.cpp Client.cpp Utilities.cpp Commands/LIST.cpp Commands/INVITE.cpp Commands/NOTICE.cpp Commands/PRIVMSG.cpp Commands/KICK.cpp Commands/KILL.cpp Commands/OPER.cpp Commands/MODE.cpp Commands/PART.cpp Commands/JOIN.cpp Commands/NAMES.cpp Commands/TOPIC.cpp Commands/PING.cpp Commands/PASS.cpp Commands/TIME.cpp Commands/QUIT.cpp Commands/USER.cpp Commands/NICK.cpp RFC1459.cpp Commands/MOTD.cpp
OBJECTS			:= $(SOURCES:.cpp=.o)
DEPENDENCIES	:= $(SOURCES:.cpp=.d)

CXX 			:= c++
CXXFLAGS		:= -Wall -Wextra -Werror -std=c++98
DXXFLAGS		= -MMD -MF $(@:.o=.d)

.PHONY: all
all: $(NAME)

-include $(DEPENDENCIES)
$(NAME): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(DXXFLAGS)

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(DEPENDENCIES)

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)

.PHONY: re
re: fclean all
