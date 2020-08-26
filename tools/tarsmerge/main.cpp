#include "util/tc_file.h"
#include "util/tc_option.h"
#include "parse.h"
#include <set>
#include <list>

using namespace tars;

void usage()
{
    cout << "Usage : tarsmerge [OPTION] tarsfile " << endl;
	cout << "--out: output merge file" << endl;
	cout << endl;
    exit(100);
}

void check(vector<string> &vTars)
{
    for(size_t i  = 0; i < vTars.size(); i++)
    {
        string ext  = TC_File::extractFileExt(vTars[i]);
        if(ext == "tars")
        {
            if(!TC_File::isFileExist(vTars[i]))
            {
                cerr << "file '" << vTars[i] << "' not exists" << endl;
                usage();
            }
        }
        else
        {
            cerr << "only support tars file." << endl;
            exit(100);
        }
    }
}

vector<string> doParseAll(TC_Option& option, const vector<string>& vTars)
{
	set<string> data;

	for(size_t i = 0; i < vTars.size(); i++) {

		data.insert(vTars[i]);

		g_parse->parse(vTars[i]);

		std::vector<ContextPtr> contexts = g_parse->getContexts();
		for (auto c : contexts) {
			if(c->getFileName()	!= vTars[i])
			{
				if(TC_File::isAbsolute(c->getFileName()))
				{
					data.insert(c->getFileName());
				}
				else if(TC_File::isAbsolute(vTars[i]))
				{
					data.insert(TC_File::simplifyDirectory(TC_File::extractFilePath(vTars[i]) + FILE_SEP + c->getFileName()));
				}
				else
				{
					data.insert(c->getFileName());
				}
			}
		}
	}

	return vector<string>(data.begin(), data.end());
}

string doTarsMerge(TC_Option& option, const vector<string>& vTars)
{
	//把所有依赖都分析出来
	vector<string> allTars = doParseAll(option, vTars);

	list<string> orderFileNames;

	map<string, list<string>::iterator> fileNames;

	for(size_t i = 0; i < allTars.size(); i++) {

		string fileName = TC_File::extractFileName(allTars[i]);

		orderFileNames.push_front(fileName);

		fileNames[fileName] = orderFileNames.begin();
	}

	//按照依赖顺序排序
	for(size_t i = 0; i < allTars.size(); i++) {

		g_parse->parse(allTars[i]);

		string currFileName = TC_File::extractFileName(allTars[i]);

		std::vector<ContextPtr> contexts = g_parse->getContexts();
		for (auto c : contexts) {

			string fileName = TC_File::extractFileName(c->getFileName());

			if(fileName != currFileName)
			{
				//引用的文件 挪到 自己文件前面
				auto it = fileNames.find(fileName);
				if(it != fileNames.end()) {
					orderFileNames.erase(it->second);

					auto currIt = fileNames.find(currFileName);
					if(currIt != fileNames.end()) {
						orderFileNames.insert(currIt->second, fileName);
					}
				}
			}
		}
	}

	string buff;

    for(auto it = orderFileNames.begin(); it != orderFileNames.end(); ++it) {

    	string f = *it;

	    std::ifstream in(f);
	    if (!in) {
		    std::cout << "read error" << std::endl;
		    exit(-1);
	    }

	    string file;

	    std::string line;
	    while (getline(in, line)) {
		    if (TC_Common::trim(line).compare(0, 8, "#include") == 0) {
			    continue;
		    }

		    file += line + "\r\n";
	    }
	    in.close();

	    buff += file;

    }

    return buff;
}

int main(int argc, char* argv[]){

    if(argc < 2)
    {
        usage();
		return 100;
    }

    try
    {
        TC_Option option;
        option.decode(argc, argv);
        vector<string> vTars = option.getSingle();

        check(vTars);

        if (option.hasParam("help"))
        {
            usage();
        }
        string out = doTarsMerge(option, vTars);

        if(option.hasParam("out"))
        {
        	TC_File::save2file(option.getValue("out"), out);
        }
        else
        {
        	cout << out << endl;
        }
    }
    catch(exception& e)
    {
        cerr<<e.what()<<endl;
    }

    return 0;
}