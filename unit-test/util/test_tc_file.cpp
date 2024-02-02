﻿//
// Created by jarod on 2020/2/20.
//

#include "util/tc_file.h"
#include "util/tc_config.h"
#include "gtest/gtest.h"

using namespace tars;

class UtilFileTest : public testing::Test
{
public:
    //添加日志
    static void SetUpTestCase()
    {
//        cout<<"SetUpTestCase"<<endl;
    }
    static void TearDownTestCase()
    {
//        cout<<"TearDownTestCase"<<endl;
    }
    virtual void SetUp()   //TEST跑之前会执行SetUp
    {
//        cout<<"SetUp"<<endl;
    }
    virtual void TearDown() //TEST跑完之后会执行TearDown
    {
//        cout<<"TearDown"<<endl;
    }
};

TEST_F(UtilFileTest, file)   //此时使用的是TEST_F宏
{
    string file = "./test.dat";
    string data = "helloword";
    data[3] = '\0';

    TC_File::save2file(file, data);
    size_t fileSize = TC_File::getFileSize(file);
//    cout << "file size:" << fileSize  << endl;
    ASSERT_TRUE(fileSize == data.size());

    string load = TC_File::load2str(file);
//    cout << "load file size:" << load.size() << endl;
    ASSERT_TRUE(load == data);

    bool fileExists = TC_File::isFileExist(file, S_IFREG);
//    cout << "file exists:" << fileExists << endl;
    ASSERT_TRUE(fileExists);

    string dir = "test";
    TC_File::makeDir(dir);

	fileExists = TC_File::isFileExist(dir, S_IFDIR);
//	cout << "dir exists:" << fileExists << endl;
	ASSERT_TRUE(fileExists);

	string newFile = dir + FILE_SEP + file;
	TC_File::save2file(newFile, data);
	fileExists = TC_File::isFileExist(newFile, S_IFREG);
//	cout << "newFile exists:" << fileExists << endl;
	ASSERT_TRUE(fileExists);

	TC_File::makeDir(dir + FILE_SEP + "test1");
	TC_File::makeDir(dir + FILE_SEP + "test2");

	vector<string> v;
	TC_File::listDirectory(dir, v, true);
//	cout << "listDirectory:" << endl;
//	for(auto s : v)
//	{
//		cout << "    " << s << endl;
//	}

	TC_File::removeFile(dir, true);
	fileExists = TC_File::isFileExist(file, S_IFDIR);
//	cout << "dir exists:" << fileExists << endl;
	ASSERT_TRUE(!fileExists);
}

TEST_F(UtilFileTest, simplifyDirectory)
{
#if TARGET_PLATFORM_WINDOWS
//    cout << "simplifyDirectory:" << TC_File::simplifyDirectory("/./ab/tt//t///t//../tt/") << endl;

	ASSERT_TRUE(TC_File::simplifyDirectory("/./ab/tt//t///t//../tt/") == "ab\\tt\\t\\tt");
//	cout << "simplifyDirectory:" << TC_File::simplifyDirectory("c:/ab/tt//t///t//../tt/") << endl;
	ASSERT_TRUE(TC_File::simplifyDirectory("c:/ab/tt//t///t//../tt/") == "c:\\ab\\tt\\t\\tt");
#else

//	cout << "simplifyDirectory:" << TC_File::simplifyDirectory("/./ab/tt//t///t//../tt/") << endl;
	ASSERT_TRUE(TC_File::simplifyDirectory("/./ab/tt//t///t//../tt/") == "/ab/tt/t/tt");
//	cout << "simplifyDirectory:" << TC_File::simplifyDirectory("/ab/tt//t///t//../tt/") << endl;
	ASSERT_TRUE(TC_File::simplifyDirectory("/ab/tt//t///t//../tt/") == "/ab/tt/t/tt");
#endif
}

TEST_F(UtilFileTest, nameAndPath)
{

	ASSERT_TRUE(TC_File::extractFileExt("/usr/local/app/bin.exe") == "exe");
	ASSERT_TRUE(TC_File::extractFileExt("/usr/local/app/bin") == "");
	ASSERT_TRUE(TC_File::extractFileExt("/usr/local/app.dir/bin.exe") == "exe");
	ASSERT_TRUE(TC_File::extractFileExt("c:\\usr\\local\\app.dir\\bin.exe") == "exe");
	ASSERT_TRUE(TC_File::extractFileExt("c:\\usr\\local\\app.dir\\bin") == "");

	ASSERT_TRUE(TC_File::extractFileName("/usr/local/app/bin.exe") == "bin.exe");
	ASSERT_TRUE(TC_File::extractFileName("/usr/local/app/bin") == "bin");
	ASSERT_TRUE(TC_File::extractFileName("/usr/local/app.dir/bin.exe") == "bin.exe");
	ASSERT_TRUE(TC_File::extractFileName("c:\\usr\\local\\app.dir\\bin.exe") == "bin.exe");
	ASSERT_TRUE(TC_File::extractFileName("c:\\usr\\local\\app.dir\\bin") == "bin");
	ASSERT_TRUE(TC_File::extractFileName("bin.exe") == "bin.exe");

	ASSERT_TRUE(TC_File::extractFilePath("/usr/local/app/bin.exe") == "/usr/local/app/");
	ASSERT_TRUE(TC_File::extractFilePath("/usr/local/app/bin") == "/usr/local/app/");
	ASSERT_TRUE(TC_File::extractFilePath("/usr/local/app.dir/bin.exe") == "/usr/local/app.dir/");
	ASSERT_TRUE(TC_File::extractFilePath("c:\\usr\\local\\app.dir\\bin.exe") == "c:\\usr\\local\\app.dir\\");
	ASSERT_TRUE(TC_File::extractFilePath("c:\\usr\\local\\app.dir\\bin") == "c:\\usr\\local\\app.dir\\");
	ASSERT_TRUE(TC_File::extractFilePath("temp.gif") == string(".") + FILE_SEP);

	ASSERT_TRUE(TC_File::excludeFileExt("/usr/local/app/bin.exe") == "/usr/local/app/bin");

	ASSERT_TRUE(TC_File::excludeFileExt("/usr/local/app/bin") == "/usr/local/app/bin");
	ASSERT_TRUE(TC_File::excludeFileExt("/usr/local/app.dir/bin.exe") == "/usr/local/app.dir/bin");
	ASSERT_TRUE(TC_File::excludeFileExt("c:\\usr\\local\\app.dir\\bin.exe") == "c:\\usr\\local\\app.dir\\bin");
	ASSERT_TRUE(TC_File::excludeFileExt("c:\\usr\\local\\app.dir\\bin") == "c:\\usr\\local\\app.dir\\bin");
	ASSERT_TRUE(TC_File::excludeFileExt("temp.gif") == "temp");
}

#define CONFIG "<tars> \r\n \
<application>\r\n \
<volumes>\r\n \
/Volumes/MyData/centos/=/data\r\n \
/Volumes/MyData/=/mnt/data\r\n \
</volumes>\r\n\
<ports>\r\n\
8080/tcp=0.0.0.0:8080\r\n \
8081/tcp=0.0.0.0:8081\r\n \
</ports>\r\n \
</application>\r\n \
</tars>"

TEST_F(UtilFileTest, config)
{
	TC_Config conf;
	conf.parseString(CONFIG);

	auto volumes = conf.getDomainKey("/tars/application/volumes");

	ASSERT_TRUE(volumes.size() == 2);
}

TEST_F(UtilFileTest, join)
{
    string v1 = TC_File::joinPaths("abc", string("def"), string("ddd"));

    ASSERT_TRUE(v1 == string("abc") + FILE_SEP + "def" + FILE_SEP + "ddd");

    string v2 = TC_File::joinPaths("abc/", string("def"), string("ddd"));

    ASSERT_TRUE(v2 == string("abc") + FILE_SEP + "def" + FILE_SEP + "ddd");

    string v3 = TC_File::joinPaths("abc/", string("def/"), string("ddd"));

    ASSERT_TRUE(v3 == string("abc") + FILE_SEP + "def" + FILE_SEP + "ddd");


}
