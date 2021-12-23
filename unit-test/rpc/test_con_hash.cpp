//
// Created by wuxiaofeng on 2021/12/23.
//
#include "hello_test.h"

//一致hash测试
TEST_F(HelloTest, rpcConHashInvoke) {
  shared_ptr<Communicator> c = getCommunicator();
  HelloServer server;
  startServer(server, (TC_EpollServer::SERVER_OPEN_COROUTINE) 0);

  HelloPrx prx = getObj<HelloPrx>(c.get(), "HelloAdapter");

  int64_t hashCode = 101;
  for (int i = 1; i <= 10; ++i) {
    std::set<std::string> servIps;
    for (int j = 0; j < 10; ++j) {
      std::string serverIp = prx->tars_consistent_hash(hashCode)->testConHash();
      LOG_CONSOLE_DEBUG << "hashCode:" << hashCode << ", servIp:" << serverIp << endl;
      servIps.emplace(serverIp);
      //servIps.emplace(prx->tars_consistent_hash(hashCode)->testConHash());
    }

    ASSERT_TRUE(servIps.size() == 1);

    hashCode *= i;
  }

  stopServer(server);
}