#ifndef IRCSOCKET_H
#define IRCSOCKET_H

Constant TWITCH_CHANNEL_IRC = [](const QString & channel) 
                              { return QString("%1.jtvirc.com").arg(channel); };
Constant TWITCH_IRC_PORT = quint16(6667);
Constant CONNECT_TIMEOUT = 10000;

class IRCSocket : public QTcpSocket
{
    Q_OBJECT

    public :
        IRCSocket();
        virtual ~IRCSocket();

        bool connect(const QString &);

        Q_SIGNAL void messageReceived(const QString &);

    private :
        Q_SLOT void onReadyRead();
        QString buffer_;
};

#endif // IRCSOCKET_H