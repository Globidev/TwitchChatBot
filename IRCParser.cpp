#include "stdafx.h"
#include "IRCParser.h"

IRC::Message::Message() : isValid(true)
{

}

IRC::Message IRC::parseMessage(const QString & messageString)
{
    QString str(messageString);
    Message parsedMessage;
    if(str.isEmpty()) parsedMessage.isValid = false;

    if(str[0] == ':')
    {
        auto splitted = str.split(' ');
        NICKNAME_REGEX.indexIn(splitted[0]);
        parsedMessage.nickName = NICKNAME_REGEX.cap(1);
        splitted.removeFirst();
        str = splitted.join(' ');
    }

    if(str.indexOf(" :") != -1)
    {
        auto splitted = str.split(" :");
        for(const auto & s : splitted[0].split(' '))
            parsedMessage.arguments.push_back(s);
        parsedMessage.arguments.push_back(splitted[1]);
    }
    else
    {
        for(const auto & s : str.split(' '))
            parsedMessage.arguments.push_back(s);
    }

    parsedMessage.command = parsedMessage.arguments.front();
    parsedMessage.arguments.pop_front();

    return parsedMessage;
}