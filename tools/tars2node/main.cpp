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
#include "code_generator.h"

void version()
{
    cout << "v" << GENERATOR_VERSION << endl;
}

void usage()
{
    cout << "Version : " << GENERATOR_VERSION << endl;
    cout << "Usage   : " << EXECUTE_FILENAME << " [OPTION] " << "*." << TC_Common::lower(IDL_TYPE) << " file" << endl;
    cout << "  --version                                   print " << EXECUTE_FILENAME << " version" << endl;
    cout << "  --rpc-path=[DIRECTORY]                      specify the path of rpc module." << endl;
    cout << "  --stream-path=[DIRECTORY]                   specify the path of stream module." << endl;
    cout << "  --allow-reserved-namespace                  allow you to use reserved word as a namespace." << endl;
    cout << "  --dir=DIRECTORY                             generate source file to DIRECTORY." << endl;
    cout << "  --relative                                  use current path." << endl;
    cout << "  --base=DIRECTORY                            where to search " << IDL_TYPE << " files." << endl;
    cout << "  --r                                         generate source all " << IDL_TYPE << " files." << endl;
    cout << "  --r-minimal                                 minimize the dependent members." << endl;
    cout << "  --r-reserved                                list of names(split by \",\") that should be keeped." << endl;
    cout << "  --client                                    just for client side source file." << endl;
    cout << "  --server                                    just for server side source file." << endl;
    cout << "  --ts                                        generate typescript file." << endl;
    cout << "  --dts                                       generate d.ts file." << endl;
    cout << "  --long-type=[number|string|bigint]          use <Number|String|BigInt> represent <long> type, default is <Number>." << endl;
    cout << "  --string-binary-encoding                    get string raw bytes <BinBuffer>." << endl;
    cout << "  --enum-reverse-mappings                     reverse mapping from enum values to enum names." << endl;
    cout << "  --optimize=[0|s]                            support \"s\" to reduce code size, default is 0." << endl;
    cout << endl;
    cout << EXECUTE_FILENAME << " support type: boolean char short int long float double list map" << endl;

    exit(0);
}

void check(vector<string> &vFiles)
{
    for(size_t i  = 0; i < vFiles.size(); i++)
    {
        string ext = TC_File::extractFileExt(vFiles[i]);
        if(ext == TC_Common::lower(IDL_TYPE))
        {
            if(!TC_File::isFileExist(vFiles[i]))
            {
                cerr << "file '" << vFiles[i] << "' not exists" << endl;
                usage();
                exit(0);
            }
        }
        else
        {
            cerr << "only support "<< TC_Common::lower(IDL_TYPE) << " file." << endl;
            usage();
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

    try
    {
        TC_Option option;
        option.decode(argc, argv);
        vector<string> vFiles = option.getSingle();
        check(vFiles);

        if (option.hasParam("version"))
        {
            version();
            return 0;
        }

        if(option.hasParam("help"))
        {
            usage();
            return 0;
        }

        if(option.hasParam("base"))
        {
            if (::chdir(option.getValue("base").c_str()) != 0) {
                return -1;
            }
        }

    #define ALLOW_USE_RESERVED_NAMESPACE_V(name, keeped) \
        g_parse->set##name(keeped);
    #define ALLOW_USE_RESERVED_NAMESPACE_BASE(name, keeped) \
        ALLOW_USE_RESERVED_NAMESPACE_V(name, keeped)
    #define ALLOW_USE_RESERVED_NAMESPACE(keeped) \
        ALLOW_USE_RESERVED_NAMESPACE_BASE(IDL_NAMESPACE, keeped)

        ALLOW_USE_RESERVED_NAMESPACE(option.hasParam("allow-reserved-namespace"));

    #undef ALLOW_USE_RESERVED_NAMESPACE
    #undef ALLOW_USE_RESERVED_NAMESPACE_BASE
    #undef ALLOW_USE_RESERVED_NAMESPACE_V

	    g_parse->setTars(option.hasParam("with-tars"));
        g_parse->setUseCurrentPath(option.hasParam("relative"));

        CodeGenerator generator;
        generator.setRpcPath(option.hasParam("rpc-path")?option.getValue("rpc-path"):RPC_MODULE_PATH);
        generator.setStreamPath(option.hasParam("stream-path")?option.getValue("stream-path"):STREAM_MODULE_PATH);
        generator.setEnableClient(option.hasParam("client"));
        generator.setEnableServer(option.hasParam("server"));
        generator.setTargetPath(option.hasParam("dir")?option.getValue("dir"):"./");
        generator.setUseSpecialPath(option.hasParam("relative"));
        generator.setStringBinaryEncoding(option.hasParam("string-binary-encoding"));
        generator.setEnumReverseMappings(option.hasParam("enum-reverse-mappings"));
        generator.setEnableTS(option.hasParam("ts"));
        generator.setEnableDTS(option.hasParam("dts"));

        if (option.hasParam("long-type"))
        {
            string longType = TC_Common::lower(option.getValue("long-type"));
            if (longType == "string")
            {
                generator.setLongType(CodeGenerator::String);
            }
            else if (longType == "bigint")
            {
                generator.setLongType(CodeGenerator::BigInt);
            }
            
        }

        if (option.hasParam("optimize"))
        {
            string level = TC_Common::lower(option.getValue("optimize"));
            if (level == "s")
            {
                generator.setOptimize(CodeGenerator::Os);
            }
            else
            {
                generator.setOptimize(CodeGenerator::O0);
            }
        }

        bool _bRecursive = option.hasParam("r");
        bool _bMinimalMembers = option.hasParam("r-minimal");

        if (option.hasParam("r-reserved"))
        {
            set<string> vMembers;
            vector<string> vReserved = TC_Common::sepstr<string>(option.getValue("r-reserved"), ",");
            for (size_t i = 0; i < vReserved.size(); i++)
            {
                if (vReserved[i].empty())
                {
                    continue;
                }

                if (TC_Common::sepstr<string>(vReserved[i], "::").size() == 2)
                {
                    vMembers.insert(TC_Common::trim(vReserved[i]));
                } else {
                    cerr << "reserved member must be match <Module>::<Member> pattern" << endl;
                    return 0;
                }
            }

            if (vMembers.size() > 0)
            {
                _bMinimalMembers = true;
                generator.setDependent(vMembers);
            }
        }

        if (!_bRecursive && _bMinimalMembers)
        {
            cerr << "Missing --r flag" << endl;
            return 0;
        }

        generator.setRecursive(_bRecursive);
        generator.setMinimalMembers(_bMinimalMembers);

        for (size_t i = 0; i < vFiles.size(); i++)
        {
            generator.createFile(vFiles[i], true);
        }
    }
    catch(exception& e)
    {
        cerr<<e.what()<<endl;
    }

    return 0;
}