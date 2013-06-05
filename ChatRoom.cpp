#include "stdafx.h"
#include "ChatRoom.h"

#include "ChatBot.h"
#include "IRCSocket.h"
#include "IRCParser.h"

#include "PythonWrapper.h"

ChatRoom::ChatRoom(const ChatBot & chatBot, 
                   const QString & channelName) :
    channelName_(channelName), 
    socket_(new IRCSocket)
{
    QObject::connect(socket_.get(), &IRCSocket::messageReceived, 
    [](const QString & message)
    {
        auto parsedMessage = IRC::parseMessage(message);

    });

    if(socket_->connect(channelName_) && chatBot.isReady())
    {
        write(AUTHENTICATE(chatBot.chatToken(), chatBot.nickName()));
        write(JOIN_CHANNEL(channelName_));
    }
}

ChatRoom::~ChatRoom()
{
    write(LEAVE_CHANNEL(channelName_));
}

void ChatRoom::write(const QString & command)
{
    socket_->write(QString("%1\n").arg(command).toLocal8Bit());
}

void ChatRoom::sendPrivateMessage(const QString & message)
{
    write(PRIVATE_MESSAGE(channelName_, message));
}