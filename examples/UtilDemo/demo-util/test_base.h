//
// Created by jarod on 2020/2/20.
//

#ifndef TARS_CPP_TEST_BASE_H
#define TARS_CPP_TEST_BASE_H

class Test_Base
{
public:
	virtual void test() = 0;
	virtual ~Test_Base(){}
};

class Test_TC_File : public Test_Base
{
public:
	virtual void test();
};

#endif //TARS_CPP_TEST_BASE_H
