#include <iostream>
#include "servant/Communicator.h"
#include "servant/Application.h"
#include "Hello.h"
using namespace std;
using namespace TestApp;
using namespace tars;


class Demo : public TC_Singleton<Demo>, public TC_ThreadLock{
    private:
        TestApp::HelloPrx _hello1Prx;
        TestApp::HelloPrx _hello2Prx;
        CommunicatorPtr _comm1 = CommunicatorFactory::getInstance()->getCommunicator("comm1");
        CommunicatorPtr _comm2 = CommunicatorFactory::getInstance()->getCommunicator("comm2");
    public:
        Demo(){
            _comm1->setProperty("locator", "taf.tafregistry.QueryObj@tcp -h 123.123.123.123 -p 8080 -t 60000");
            _comm2->setProperty("locator", "taf.tafregistry.QueryObj@tcp -h 123.123.123.123 -p 8080 -t 60000");
            _hello1Prx = _comm1->stringToProxy<TestApp::HelloPrx>("Hello.HelloServer.HelloObj");
            _hello2Prx = _comm2->stringToProxy<TestApp::HelloPrx>("Hello.HelloServer.HelloObj");
        }
        void sendOneWay1(){
            try{
                _hello1Prx->async_test(NULL);
                cout<<"sent one one way request in thread:" << std::this_thread::get_id() << "\n" << endl;
            }catch(...){
                cout<<"ent one one way request in thread:" << std::this_thread::get_id() << " error" << "\n" << endl;
            }
        }
        void sendOneWay2(){
            try{
                _hello2Prx->async_test(NULL);
                cout<<"sent one one way request in thread:" << std::this_thread::get_id() << "\n" << endl;
            }catch(...){
                cout<<"ent one one way request in thread:" << std::this_thread::get_id() << " error" << "\n" << endl;
            }
        }
};

#define DemoMgr (Demo::getInstance())

void callByMixObjSingleton(){
    while (true)
    {
        DemoMgr->sendOneWay1();
        DemoMgr->sendOneWay2();
        usleep(10000);
    }
};
//void callByMixObj(){
//    Demo demo;
//    while (true)
//    {
//        demo.sendOneWay1();
//        demo.sendOneWay2();
//        usleep(10000);
//    }
//};
int main(){
    try
    {
        std::thread t1(callByMixObjSingleton);
//        std::thread t2(callByMixObjSingleton);
        // std::thread t1(callByMixObj);
        // std::thread t2(callByMixObj);
        t1.join();
//        t2.join();
    }
    catch(exception& e)
    {
        cerr << "exception:" << e.what() << endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << endl;
    }

    return 0;
}
