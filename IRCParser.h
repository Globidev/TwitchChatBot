#ifndef IRCPARSER_H
#define IRCPARSER_H

#include <deque>

namespace IRC
{
    Global NICKNAME_REGEX = QRegExp("^:(.*)!.*$");
    struct Message
    {
        Message();

        QString nickName;
        QString command;
        std::deque<QString> arguments;
        bool isValid;
    };

    Message parseMessage(const QString &);
    QString makeMessage(const Message &);
};

#endif // IRCPARSER_H