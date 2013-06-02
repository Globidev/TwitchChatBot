#include "stdafx.h"
#include "ChatRoom.h"

#include "ChatBot.h"
#include "IRCSocket.h"

#include "PythonWrapper.h"

ChatRoom::ChatRoom(const ChatBot & chatBot, 
                   const QString & channelName) :
    channelName_(channelName), 
    socket_(new IRCSocket)
{
    QObject::connect(socket_.get(), &IRCSocket::messageReceived, 
    [](const QString & message)
    {
        PythonWrapper::exec([message](const bp::object & globals)
        {
            bp::object parseFunction = globals[PARSE_IRC_MESSAGE_FUNC_NAME];
            auto result = bp::call<bp::tuple>(parseFunction.ptr(), 
                                             message.toLocal8Bit().constData());
            std::string p, c;
            bp::list a;
            unpackPythonTuple(result, p, c, a);
            qDebug() << p.c_str() << c.c_str();
            for(int i = 0; i < bp::len(a); ++ i)
                qDebug() << bp::extract<char *>(a[i]);
        });
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