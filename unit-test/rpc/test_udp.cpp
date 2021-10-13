
#include "hello_test.h"

#define UDP_ASYNC_COUNT 100

TEST_F(HelloTest, udpSyncGlobalCommunicator)
{
	shared_ptr<Communicator> c = getCommunicator();

	transGlobalCommunicator([&](Communicator *comm){
		checkSync(comm, "UdpObjAdapter");
	}, c.get());
}

TEST_F(HelloTest, udpASyncGlobalCommunicator)
{
	shared_ptr<Communicator> c = getCommunicator();

	int count = _count;
	_count = UDP_ASYNC_COUNT;

	//异步udp, 发送太快了会丢包
	transGlobalCommunicator([&](Communicator *comm){
		checkASync(comm, "UdpObjAdapter");
	}, c.get());

	_count = count;
}

TEST_F(HelloTest, udpASyncGlobalCommunicatorInCoroutine)
{
	shared_ptr<Communicator> c = getCommunicator();

	int count = _count;
	_count = UDP_ASYNC_COUNT;

	transInCoroutineGlobalCommunicator([&](Communicator *comm){
		checkASync(comm, "UdpObjAdapter");
	}, c.get());

	_count = count;

}


TEST_F(HelloTest, udpIpV6SyncGlobalCommunicator)
{
	shared_ptr<Communicator> c = getCommunicator();

	transGlobalCommunicator([&](Communicator *comm){
		checkSync(comm, "UdpIpv6Adapter");
	}, c.get());
}

TEST_F(HelloTest, udpIpV6ASyncGlobalCommunicatorInCoroutine)
{
	shared_ptr<Communicator> c = getCommunicator();

	int count = _count;
	_count = UDP_ASYNC_COUNT;

	transInCoroutineGlobalCommunicator([&](Communicator *comm){
		checkASync(comm, "UdpIpv6Adapter");
	}, c.get());

	_count = count;

}