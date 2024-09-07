#pragma once

#include <iostream>
#include <string>

class Client{

    private:
        int _clientSocketFd;
        std::string _clientIp;
        int _clientPort;
        std::string _nickname;
        std::string _username;
        std::string _realname;
        std::string _hostname;
        std::string _servername;
        bool        _isAuth;
        bool        _hasNick;
        // bool       	_isOperator;
        bool        _loged;

    public:
        Client();
        ~Client();
        Client(int socket, const std::string& ip, int port);

        //get
        int getFd() const;
        const std::string& getNickname() const;
        const std::string& getRealname() const;
        const std::string& getUsername() const;
        const std::string& getHostname() const;
        bool getHasNick()const;
        bool getLoged()const;
        std::string getPrefix() const;

        //set
        void setNickname(const std::string& nickname);
        void setRealname(const std::string& realname);
        void setUsername(const std::string& username);
        void setHostname(const std::string& hostname);
        void setClientIp(const std::string& clientIp);
        void setAuth(bool auth);
        void setLoged(bool loged);
        void setHasNick(bool auth);

        //Authenticate
        bool isAuth() const;
        void authenticate();
        void deauthenticate();
        
        //Actions
        void sendMessage( const std::string& message ) const;
        void sendReply( const std::string& reply ) const;


};