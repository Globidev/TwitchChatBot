#ifndef CHATROOM_H
#define CHATROOM_H

class ChatBot;
class IRCSocket;

Constant AUTHENTICATE = [](const std::string & token, const std::string & nick) 
                        { return QString("PASS oauth:%1\nNICK %2")
                                    .arg(token.c_str(), nick.c_str()); };
Constant JOIN_CHANNEL = [](const QString & channel) 
                        { return QString("JOIN #%1").arg(channel); };

Constant LEAVE_CHANNEL = [](const QString & channel)
                         { return QString("PART #%1").arg(channel); };

Constant PRIVATE_MESSAGE = [](const QString & channel, const QString & message)
                           { return QString("PRIVMSG #%1 :%2")
                                        .arg(channel, message); };

class ChatRoom : boost::noncopyable
{
    public :
        ChatRoom(const ChatBot &, const QString &);
        ~ChatRoom();

        void write(const QString &);
        void sendPrivateMessage(const QString &);

    private :
        std::unique_ptr<IRCSocket> socket_;
        QString channelName_;
};

#endif // CHATROOM_H