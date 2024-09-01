#!/bin/bash

input=".botchatlogs"

HOST="$1"
PORT="$2"
PASS="$3"
NAME="Bot"
CHAN="#RockPaperScissors"

if [ "$#" -ne 3 ]; then
    echo "Please launch the bot using: 'sh $0 <host> <port> <password>'"
    exit 1
fi

log_n_join()
{
    printf "PASS ${PASS}\r\n"
    printf "NICK ${NAME}\r\n"
    printf "USER ${NAME} * * :${NAME}\r\n"
    printf "JOIN ${CHAN}\r\n"
}

log_n_join > $input
trap "rm -f $input;exit 0" INT TERM EXIT
tail -f $input | nc $HOST $PORT | while read line; do
    if ( echo "$line" | grep "PRIVMSG ${CHAN}"); then
        RANDOM=$(shuf -i 0-999 -n1)
        rps=$((1 + RANDOM % 3))
        if [ "$rps" -eq 1 ]; then
            printf "NOTICE ${CHAN} :ROCK\r\n" >> $input
            if ( echo "$line" | grep "ROCK" >> /dev/null); then
                printf "NOTICE ${CHAN} :It's a tie!\r\n" >> $input
            elif ( echo "$line" | grep "PAPER" >> /dev/null); then
                printf "NOTICE ${CHAN} :You win!\r\n" >> $input
            elif ( echo "$line" | grep "SCISSORS" >> /dev/null); then
                printf "NOTICE ${CHAN} :You lose!\r\n" >> $input
            else
                printf "NOTICE ${CHAN} :Apprends a jouer\r\n" >> $input
            fi
        elif [ "$rps" -eq 2 ]; then
            printf "NOTICE ${CHAN} :PAPER\r\n" >> $input
            if ( echo "$line" | grep "PAPER" >> /dev/null); then
                printf "NOTICE ${CHAN} :It's a tie!\r\n" >> $input
            elif ( echo "$line" | grep "SCISSORS" >> /dev/null); then
                printf "NOTICE ${CHAN} :You win!\r\n" >> $input
            elif ( echo "$line" | grep "ROCK" >> /dev/null); then
                printf "NOTICE ${CHAN} :You lose!\r\n" >> $input
            else
                printf "NOTICE ${CHAN} :Apprends a jouer\r\n" >> $input
            fi
        elif [ "$rps" -eq 3 ]; then
            printf "NOTICE ${CHAN} :SCISSORS\r\n" >> $input
            if ( echo "$line" | grep "SCISSORS" >> /dev/null); then
                printf "NOTICE ${CHAN} :It's a tie!\r\n" >> $input
            elif ( echo "$line" | grep "ROCK" >> /dev/null); then
                printf "NOTICE ${CHAN} :You win!\r\n" >> $input
            elif ( echo "$line" | grep "PAPER" >> /dev/null); then
                printf "NOTICE ${CHAN} :You lose!\r\n" >> $input
            else
                printf "NOTICE ${CHAN} :Apprends a jouer\r\n" >> $input
            fi
        fi
    fi
done
