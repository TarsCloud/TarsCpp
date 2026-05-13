#pragma once

#include "servant/Application.h"

class WebSocketServantImp : public tars::Servant
{
public:
    virtual ~WebSocketServantImp() {}

    virtual void initialize();

    virtual void destroy();

    int doRequest(tars::TarsCurrentPtr current, std::vector<char> &response);

    int doClose(tars::TarsCurrentPtr current);
};
