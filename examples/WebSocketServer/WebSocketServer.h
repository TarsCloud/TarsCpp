#pragma once

#include "servant/Application.h"

class WebSocketServer : public tars::Application
{
public:
    virtual ~WebSocketServer() {}

    virtual void initialize();

    virtual void destroyApp();
};

extern WebSocketServer g_app;
