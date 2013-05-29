#include "stdafx.h"
#include "IRCSocket.h"

IRCSocket::IRCSocket() : QTcpSocket()
{
    QObject::connect(this, &QTcpSocket::readyRead, 
                     this, &IRCSocket::onReadyRead);
}

IRCSocket::~IRCSocket()
{
    
}

bool IRCSocket::connect(const QString & channelName)
{
    connectToHost(TWITCH_CHANNEL_IRC(channelName), TWITCH_IRC_PORT);
    return waitForConnected(CONNECT_TIMEOUT);
}

void IRCSocket::onReadyRead()
{
    buffer_ += readAll();
    int endLineIndex = 0;

    while((endLineIndex = buffer_.indexOf("\n", 0)) != -1)
    {
        Q_EMIT messageReceived(buffer_.left(endLineIndex));
        buffer_.remove(0, endLineIndex + 1);
    }
}