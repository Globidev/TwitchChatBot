#include "stdafx.h"
#include "ChatBot.h"

#include "PythonWrapper.h"

ChatBot::ChatBot(const std::string & nickName,
                 const std::string & clientId,
                 const std::string & clientSecret) :
    clientId_(clientId), 
    clientSecret_(clientSecret),
    nickName_(nickName)
{
    qDebug() << "retrieving access token ...";

    PythonWrapper::exec([this](const bp::object & globals)
    {
        bp::object getAccessFunc = globals[GET_ACCES_TOKEN_FUNC_NAME];
        bp::object result = getAccessFunc(clientId_, clientSecret_);
        oauthAccessToken_ = std::move(maybeExtract<std::string>(result));
    }, false);

    qDebug() << (oauthAccessToken_ ? "success" : "fail");
}

bool ChatBot::isReady() const
{
    return oauthAccessToken_;
}

std::string ChatBot::chatToken() const
{
    return isReady() ? *oauthAccessToken_ : std::string();
}

std::string ChatBot::nickName() const
{
    return nickName_;
}