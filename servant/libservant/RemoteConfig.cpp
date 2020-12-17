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

#include "servant/RemoteConfig.h"
#include "util/tc_file.h"
#include "servant/Communicator.h"
#include "servant/RemoteNotify.h"
#include "servant/Application.h"
#include <fstream>

namespace tars
{

int RemoteConfig::setConfigInfo(const CommunicatorPtr &comm, const string &obj, const string & app, const string &serverName, const string& basePath,const string& setdivision, int maxBakNum)
{
    _comm           = comm;
    if(!obj.empty())
    {
        _configPrx      = _comm->stringToProxy<ConfigPrx>(obj);
    }
    _app           = app;
    _serverName    = serverName;
    _basePath      = basePath;
    _maxBakNum     = maxBakNum;
    _setdivision   = setdivision;
    return 0;
}

bool RemoteConfig::addConfig(const string & sFileName, string &buffer, bool bAppConfigOnly)
{
    TC_LockT<TC_ThreadMutex> lock(_mutex);

    try
    {
        string sFullFileName = _basePath + FILE_SEP + sFileName;

        string newFile = getRemoteFile(sFileName, bAppConfigOnly);

        if (newFile.empty() || !TC_File::isFileExist(newFile))//拉取不到配置中心的配置文件
        {
            if(!TC_File::isFileExist(newFile)) //获取本地配置成功，返回成功，但需要告警一下。
            {
                buffer = "[fail] get remote config:" + sFileName + "fail,use the local config.";

                return true;
            }
            throw runtime_error("access file error:" + newFile);
        }

        if (TC_File::load2str(newFile) != TC_File::load2str(sFullFileName))
        {
            for (int i = _maxBakNum - 1; i >= 1; --i)
            {
                if (TC_File::isFileExist(index2file(sFullFileName, i)))
                {
                    localRename(index2file(sFullFileName, i), index2file(sFullFileName, i+1));
                }
            }

            if (TC_File::isFileExist(sFullFileName))
            {
                localRename(sFullFileName, index2file(sFullFileName, 1));
            }
        }

        localRename(newFile, sFullFileName);

		assert(TC_File::isFileExist(sFullFileName));
        //assert(!access(sFullFileName.c_str(), R_OK));

        buffer = "[succ] get remote config:" + sFileName;

        return true;
    }
    catch (std::exception& e)
    {
        buffer = "[fail] get remote config '" + sFileName + "' error:" + string(e.what());
    }
    catch (...)
    {
        buffer = "[fail] get remote config '" + sFileName + "' unknown error";
    }

    return false;
}

string RemoteConfig::getRemoteFile(const string &sFileName, bool bAppConfigOnly)
{
    if (_configPrx)
    {
       string stream;
       int ret = -1;


       for(int i = 0; i < 2;i++)
       {
           try
           {
                if(_setdivision.empty())
                {
                    ret = _configPrx->loadConfig(_app, (bAppConfigOnly ? "" : _serverName), sFileName, stream, ServerConfig::Context);
                }
                else
                {
                    struct ConfigInfo confInfo;
                    confInfo.appname     = _app;
                    confInfo.servername  = (bAppConfigOnly ? "" : _serverName);
                    confInfo.filename    = sFileName;
                    confInfo.bAppOnly    = bAppConfigOnly;
                    confInfo.setdivision = _setdivision;
                    ret = _configPrx->loadConfigByInfo(confInfo,stream, ServerConfig::Context);
                }
                
                break;
           }catch(std::exception& e){
            //
           }catch (...){
            //
           }
       }
       
       if (ret != 0 || stream.empty())
       {
           throw runtime_error("remote config file is empty:" + sFileName);
       }


        string newFile = _basePath + FILE_SEP + sFileName + "." + TC_Common::tostr(time(NULL));

        std::ofstream out(newFile.c_str());
        
        string result;
        if (out)
        {
            out << stream;//如果硬盘满了，是否能写入成功需要进行判断。
            out.flush();
            if(out.bad()) 
            {
                out.close();
                result = "[fail] copy stream to disk error." ;
                RemoteNotify::getInstance()->report(result);
                return "";
            }
            else
            {
                out.close();
                return newFile;
            }
        }
    }
    return "";
}

string RemoteConfig::index2file(const string & sFullFileName, int index)
{
    return   sFullFileName + "." + TC_Common::tostr(index) + ".bak";
}

void RemoteConfig::localRename(const string& oldFile, const string& newFile)
{
#if TARGET_PLATFORM_WINDOWS
    //by goodenpei，windows下面先remove后rename，否则rename会失败
    if(TC_File::isFileExist(oldFile) && TC_File::isFileExist(newFile))
    {
        ::remove(newFile.c_str());
    }
#endif
    if (::rename(oldFile.c_str(), newFile.c_str()) != 0)
    {
        throw runtime_error("rename file error:" + oldFile + "->" + newFile);
    }
}

string RemoteConfig::recoverSysConfig(const string & sFullFileName)
{
    try
    {
        for (int i = 1; i <= _maxBakNum; ++i)
        {
            if (TC_File::isFileExist(index2file(sFullFileName, i)))
            {
                localRename(index2file(sFullFileName, i), sFullFileName);

                return "[succ] recover file:" + index2file(sFullFileName, i);
            }
        }
    }
    catch (std::exception& e)
    {
        return "[fail] recover config error:" + string(e.what());
    }
    catch (...)
    {
        return "[fail] recover config error";
    }

    return "[fail] no backup file.";
}

}


