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

#include "util/tc_option.h"
#include "util/tc_file.h"
#include "util/tc_common.h"
#include "tars2cpp.h"


void usage()
{
    cout << "Usage : tars2cpp [OPTION] tarsfile" << endl;
    // cout << "  --coder=Demo::interface1;Demo::interface2   create interface encode and decode api" << endl;
    cout << "  --dir=DIRECTORY                             generate source file to DIRECTORY(create tars protocol file to DIRECTORY, default is current directory)" << endl;
    cout << "  --check-default=<true,false>                optional field with default value not do package(default: true)" << endl;
    cout << "  --unjson                                    not json interface" << endl;
    cout << "  --os                                        only create struct(not create interface) " << endl;
    cout << "  --include=\"dir1;dir2;dir3\"                set search path of tars protocol" << endl;
    // cout << "  --unknown                                   create unkown field" << endl;
    cout << "  --tarsMaster                                create get registry info interface" << endl;
    cout << "  --currentPriority						   use current path first." << endl;
    cout << "  tars2cpp support type: bool byte short int long float double vector map" << endl;
    exit(0);
}

void check(vector<string> &vTars)
{
    for(size_t i  = 0; i < vTars.size(); i++)
    {
        string ext  = tars::TC_File::extractFileExt(vTars[i]);
        if(ext == "tars")
        {
            if(!tars::TC_File::isFileExist(vTars[i]))
            {
                cerr << "file '" << vTars[i] << "' not exists" << endl;
				usage();
                exit(0);
            }
        }
        else
        {
            cerr << "only support tars file." << endl;
            exit(0);
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        usage();
    }

    tars::TC_Option option;
    option.decode(argc, argv);
    vector<string> vTars = option.getSingle();

    check(vTars);

    if(option.hasParam("help"))
    {
        usage();
    }

    // bool bCoder = option.hasParam("coder");
    // vector<string> vCoder;
    // if(bCoder)
    // {
    //     vCoder = tars::TC_Common::sepstr<string>(option.getValue("coder"), ";", false);
    //     if(vCoder.size() == 0)
    //     {
    //         usage();
    //         return 0;
    //     }
    // }

    Tars2Cpp t2c;

    if (option.hasParam("dir"))
    {
        t2c.setBaseDir(option.getValue("dir"));
    }
    else
    {
        t2c.setBaseDir(".");
    }

    t2c.setCheckDefault(tars::TC_Common::lower(option.getValue("check-default")) == "false"?false:true);

    t2c.setOnlyStruct(option.hasParam("os"));

    //默认支持json
    t2c.setJsonSupport(true);

    if (option.hasParam("unjson"))
    {
        t2c.setJsonSupport(false);
    }

    if (option.hasParam("sql"))
    {
        t2c.setSqlSupport(true);
        t2c.setJsonSupport(true);
    }

    if (option.hasParam("xml"))
    {
        vector<string> vXmlIntf;
        string sXml = tars::TC_Common::trim(option.getValue("xml"));
        sXml = tars::TC_Common::trimleft(tars::TC_Common::trimright(sXml, "]"), "[");
        if (!sXml.empty())
        {
            vXmlIntf = tars::TC_Common::sepstr<string>(sXml, ",", false);
        }
        t2c.setXmlSupport(true, vXmlIntf);
    }

    // if (option.hasParam("json"))
    // {
    //     t2c.setJsonSupport(true);
    //     string sJson = tars::TC_Common::trim(option.getValue("json"));
    //     sJson = tars::TC_Common::trimleft(tars::TC_Common::trimright(sJson, "]"), "[");
    //     if (!sJson.empty())
    //     {
    //         t2c.setJsonSupport(tars::TC_Common::sepstr<string>(sJson, ",", false));
    //     }
    // }

    t2c.setTarsMaster(option.hasParam("tarsMaster"));

    try
    {
        //增加include搜索路径
        g_parse->addIncludePath(option.getValue("include"));

        //是否可以以tars开头
        g_parse->setTars(option.hasParam("with-tars"));
        g_parse->setHeader(option.getValue("header"));
        g_parse->setCurrentPriority(option.hasParam("currentPriority"));

        // t2c.setUnknownField(option.hasParam("unknown"));
        for(size_t i = 0; i < vTars.size(); i++)
        {

            g_parse->parse(vTars[i]);
            t2c.createFile(vTars[i]);//, vCoder);
        }
    }
    catch(exception& e)
    {
	    cerr<<e.what()<<endl;
    }

    return 0;
}

