/**
 * Tencent is pleased to support the open source community by making Tars available.
 *
 * Copyright (C) 2016THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
 * in compliance with the License. You may obtain a copy of the License at
 *
 * https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software distributed
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations under the License.
 */

#include "util/tc_file.h"
#include "util/tc_option.h"

#include "tars2case.h"

void usage()
{
    cout << "Usage : tars2case [OPTION] tarsfile" << endl;
	cout << "  		--json generate json casefile" << endl;
	cout << "  		--web generate webadmin casefile" << endl;
	cout << "  		--dir=DIRECTORY generate casefile to DIRECTORY" << endl;

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


void doTars2Test(TC_Option& option, const vector<string>& vTars)
{
	Tars2Case j2t;
	j2t.setJsonCase(option.hasParam("json") ? true : false);
	j2t.setWebSupport(option.hasParam("web") ? true : false);
	j2t.setBaseDir(option.getValue("dir").empty() ? "" : option.getValue("dir"));

	g_parse->setUseCurrentPath(true);
	for(size_t i = 0; i < vTars.size(); i++)
	{
		g_parse->parse(vTars[i]);
		j2t.createFile(vTars[i], option.getValue("out"));
	}
}

int main(int argc, char* argv[]){

    if (argc < 2)
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
		::chdir(TC_File::extractFilePath(vTars[0]).c_str());
		if (option.hasParam("help"))
		{
			usage();
		}
		doTars2Test(option, vTars);
	}
	catch(exception& e)
	{
		cerr<<e.what()<<endl;
	}
    return 0;
}


