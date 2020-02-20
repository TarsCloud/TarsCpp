#include <cassert>
#include <iostream>
#include <vector>
#include "test_base.h"

using namespace std;

int main(int argc, char** argv)
{
	try
	{
		vector<Test_Base*> tests;
		tests.push_back(new Test_TC_File());

		for(auto t : tests)
		{
			t->test();
		}

		for(auto t: tests)
		{
			delete t;
		}
		tests.clear();

	}
	catch (exception &ex)
	{
		cerr << "HttpServer::run ex:" << ex.what() << endl;
	}

	cout << "HttpServer::run http server thread exit." << endl;
}