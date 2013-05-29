#ifndef CHATBOT_H
#define CHATBOT_H

Constant GET_ACCES_TOKEN_FUNC_NAME = "getAccessToken";

class ChatBot : boost::noncopyable
{
    public :
        ChatBot(const std::string &, const std::string &, const std::string &);

        bool isReady() const;
        std::string chatToken() const;
        std::string nickName() const;

    private :
        std::string nickName_, clientId_, clientSecret_;
        std::unique_ptr<std::string> oauthAccessToken_;
};

#endif // CHATBOT_H