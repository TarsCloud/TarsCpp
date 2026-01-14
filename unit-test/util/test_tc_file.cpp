//
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
}

TEST_F(UtilFileTest, scanDir_default)
{
	vector<string> files;
	size_t count = TC_File::scanDir(".", files);

	ASSERT_TRUE(count > 0);
	ASSERT_TRUE(count == files.size());

	for (size_t i = 0; i < files.size(); ++i)
	{
		ASSERT_TRUE(files[i].at(0) != '.');
	}
}

TEST_F(UtilFileTest, scanDir_ignoreHide_false)
{
	vector<string> files;
	size_t count = TC_File::scanDir(".", files, NULL, 0, false);

	ASSERT_TRUE(count > 0);
	ASSERT_TRUE(count == files.size());

	bool hasDot = false;
	bool hasDotDot = false;
	bool hasHidden = false;

	for (size_t i = 0; i < files.size(); ++i)
	{
		if (files[i] == ".")
		{
			hasDot = true;
		}
		if (files[i] == "..")
		{
			hasDotDot = true;
		}
		if (files[i].at(0) == '.' && files[i] != "." && files[i] != "..")
		{
			hasHidden = true;
		}
	}

	ASSERT_TRUE(hasDot);
	ASSERT_TRUE(hasDotDot);
}

TEST_F(UtilFileTest, scanDir_maxSize)
{
	vector<string> files;

	size_t count = TC_File::scanDir(".", files, NULL, 5, true);
	ASSERT_TRUE(count == 5);
	ASSERT_TRUE(files.size() == 5);

	count = TC_File::scanDir(".", files, NULL, 10, true);
	ASSERT_TRUE(count == 10);
	ASSERT_TRUE(files.size() == 10);
}

TEST_F(UtilFileTest, scanDir_callback)
{
	vector<string> files;

	auto selectOnlyDirs = [](const struct dirent* ent) -> int {
		return ent->d_type == 4;
	};

	size_t count = TC_File::scanDir(".", files, selectOnlyDirs, 0, true);

	ASSERT_TRUE(count > 0);
	ASSERT_TRUE(count == files.size());

	for (size_t i = 0; i < files.size(); ++i)
	{
		string fullPath = string(".") + FILE_SEP + files[i];
		ASSERT_TRUE(TC_File::isFileExist(fullPath, S_IFDIR));
		ASSERT_TRUE(files[i].at(0) != '.');
	}
}

TEST_F(UtilFileTest, scanDir_callback_with_ignoreHide)
{
	vector<string> files;

	auto selectAll = [](const struct dirent* ent) -> int {
		(void)ent;
		return 1;
	};

	size_t count1 = TC_File::scanDir(".", files, selectAll, 0, true);
	size_t count2 = TC_File::scanDir(".", files, selectAll, 0, false);

	ASSERT_TRUE(count2 > count1);
	ASSERT_TRUE(count2 == files.size());
}

TEST_F(UtilFileTest, scanDir_sorted)
{
	vector<string> files1;
	TC_File::scanDir(".", files1, NULL, 0, true);

	vector<string> files2;
	TC_File::scanDir(".", files2, NULL, 0, true);

	ASSERT_TRUE(files1.size() == files2.size());

	for (size_t i = 0; i < files1.size(); ++i)
	{
		ASSERT_TRUE(files1[i] == files2[i]);
	}
}

TEST_F(UtilFileTest, scanDir_empty_directory)
{
	string dir = "test_scanDir_empty";
	TC_File::makeDir(dir);

	vector<string> files;
	size_t count = TC_File::scanDir(dir, files);

	ASSERT_TRUE(count == 0);
	ASSERT_TRUE(files.size() == 0);

	TC_Port::rmdir(dir.c_str());
}

TEST_F(UtilFileTest, scanDir_with_hidden_files)
{
	string dir = "test_scanDir_hidden";
	TC_File::makeDir(dir);

	TC_File::save2file(dir + FILE_SEP + "normal.txt", "content");
	TC_File::save2file(dir + FILE_SEP + ".hidden.txt", "hidden");

	vector<string> files;
	size_t count = TC_File::scanDir(dir, files, NULL, 0, true);

	ASSERT_TRUE(count == 1);
	ASSERT_TRUE(files.size() == 1);
	ASSERT_TRUE(files[0] == "normal.txt");

	files.clear();
	count = TC_File::scanDir(dir, files, NULL, 0, false);

	ASSERT_TRUE(count >= 2);
	bool hasNormal = false;
	bool hasHidden = false;
	for (size_t i = 0; i < files.size(); ++i)
	{
		if (files[i] == "normal.txt")
		{
			hasNormal = true;
		}
		if (files[i] == ".hidden.txt")
		{
			hasHidden = true;
		}
	}
	ASSERT_TRUE(hasNormal);
	ASSERT_TRUE(hasHidden);

	TC_File::removeFile(dir + FILE_SEP + "normal.txt", false);
	TC_File::removeFile(dir + FILE_SEP + ".hidden.txt", false);
	TC_Port::rmdir(dir.c_str());
}

TEST_F(UtilFileTest, scanDir_consistency_with_listDirectory)
{
	string dir = "test_scanDir_consistency";
	TC_File::makeDir(dir);

	TC_File::save2file(dir + FILE_SEP + "file1.txt", "content1");
	TC_File::save2file(dir + FILE_SEP + "file2.txt", "content2");
	TC_File::save2file(dir + FILE_SEP + ".hidden.txt", "hidden");
	TC_File::makeDir(dir + FILE_SEP + "subdir");

	vector<string> scanResult;
	TC_File::scanDir(dir, scanResult, NULL, 0, true);

	cout << TC_Common::tostr(scanResult.begin(), scanResult.end()) << endl;

	vector<string> listResult;
	TC_File::listDirectory(dir, listResult, false, true);

	cout << TC_Common::tostr(listResult.begin(), listResult.end()) << endl;

	ASSERT_TRUE(scanResult.size() == listResult.size());

	for (size_t i = 0; i < scanResult.size(); ++i)
	{
		bool found = false;
		for (size_t j = 0; j < listResult.size(); ++j)
		{
			string fileName = TC_File::extractFileName(listResult[j]);
			if (fileName == scanResult[i])
			{
				found = true;
				break;
			}
		}
		ASSERT_TRUE(found);
	}

	TC_File::removeFile(dir + FILE_SEP + "file1.txt", false);
	TC_File::removeFile(dir + FILE_SEP + "file2.txt", false);
	TC_File::removeFile(dir + FILE_SEP + ".hidden.txt", false);
	TC_Port::rmdir((dir + FILE_SEP + "subdir").c_str());
	TC_Port::rmdir(dir.c_str());
}
