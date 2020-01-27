
#include "util/tc_spin_lock.h"
#include <thread>
#include <iostream>
#include <cassert>
using namespace std;

namespace tars
{

//thread_local TC_SpinLock::ticket_lock TC_SpinLock::_lock;

//thread_local shared_ptr<TC_SpinLock::node> TC_SpinLock::_currNode = NULL;

TC_SpinLock::TC_SpinLock()
{
    _flag.clear(std::memory_order_release);
}

TC_SpinLock::~TC_SpinLock()
{
}

void TC_SpinLock::lock() const
{
//	int my_ticket = _lock.next_ticket.fetch_add(1);
//	while(my_ticket != _lock.now_serving){ };

    for (; _flag.test_and_set(std::memory_order_acquire);) {
//        asm volatile("rep; nop":: : "memory");
    }
//
//	node* newNode = new node;
//
//	//把当前节点加入到链表中, 返回之前的节点
//	node* prevNode = _tail.exchange(newNode);
//
//	if(prevNode)
//	{
//		//如果之前节点已经存在, 则需要等待, 直到之前节点解锁
//		while(prevNode->isLocked) { }
//	}
//
//	//不存在前驱节点, 表示该锁没有被其他线程占用，则当前线程获得锁
//	_currNode.reset(newNode);

}

void TC_SpinLock::unlock() const
{
//	assert(_currNode != NULL);
//
//	node *old = _currNode.get();
////	如果tail节点指向的currNode, 则将tail节点设置为NULL, 同时把currNode唤醒
//	_tail.compare_exchange_strong(old, NULL);
//
//	_currNode->isLocked = false;

//	_lock.now_serving++;
    _flag.clear(std::memory_order_release);
}
//
bool TC_SpinLock::tryLock() const
{
    int trys = 100;
//
//	node* newNode = new node;
//	node* p = NULL;
//
//	while(!_tail.next.compare_exchange_weak(p, newNode)){}
//
//	if(p != NULL)
//	{
//		//有prev
//		//如果之前节点已经存在, 则需要等待, 直到之前节点解锁
//		for(; trys>0 && prevNode->isLocked; --trys);
//
//		if(trys > 0)
//		{
//			//等到了锁
//			_currNode.reset(newNode);
//
//		}
//		else
//		{
//			return false;
//		}
//	}
//	else
//	{
//		//没有prev
//		_currNode.reset(newNode);
//		return true;
//	}
//
////	//把当前节点加入到链表中, 返回之前的节点
////	node* prevNode = _tail.next.exchange(newNode);
////
////	if(prevNode)
////	{
////		//如果之前节点已经存在, 则需要等待, 直到之前节点解锁
////		for(; trys>0 && prevNode->isLocked; --trys);
////	}
////
////	if(trys>0) {
////		_currNode.reset(newNode);
////		return true;
////	}
////
////	return false;
//
    for (; trys > 0 && _flag.test_and_set(std::memory_order_acquire); --trys)
    {
#if TARGET_PLATFORM_LINUX
        asm volatile("rep; nop" ::: "memory");
#endif

    }

    if (trys > 0)
        return true;

    return false;
}

}
