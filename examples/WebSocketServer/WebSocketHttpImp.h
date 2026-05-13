#pragma once

#include <string>

#include "servant/Application.h"

class WebSocketHttpImp : public tars::Servant
{
public:
    virtual ~WebSocketHttpImp() {}

    virtual void initialize();

    virtual void destroy();

    int doRequest(tars::TarsCurrentPtr current, std::vector<char> &buffer);

private:
    std::string _clientPageContent;
};
