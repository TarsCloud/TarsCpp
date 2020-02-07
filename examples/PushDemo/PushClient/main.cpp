#include "servant/Application.h"
#include "TestRecvThread.h"
#include <iostream>

using namespace std;
using namespace tars;

int main(int argc,char**argv)
{
    try
    {
        int second = 5;
        
        if(argc > 1)
            second = TC_Common::strto<int>(argv[1]);
        
        if(second <=0 )
            second = 1;

		RecvThread thread(second);
		thread.start();

		thread.getThreadControl().join();
    }
    catch(std::exception&e)
    {
        cerr<<"std::exception:"<<e.what()<<endl;
    }
    catch(...)
    {
        cerr<<"unknown exception"<<endl;
    }
    return 0;
}
