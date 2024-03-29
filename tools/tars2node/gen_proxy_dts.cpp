﻿/**
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

#include "code_generator.h"

string CodeGenerator::generateDTSProxy(const InterfacePtr &pPtr)
{
    vector<OperationPtr> &vOperation = pPtr->getAllOperationPtr();
    ostringstream str;

    //class
    INC_TAB;
    str << TAB << "class " << pPtr->getId() << "Proxy {" << endl;
    INC_TAB;
    str << TAB << "setTimeout(iTimeout: number): void;" << endl;
    str << TAB << "getTimeout(): number;" << endl;
    str << TAB << "setVersion(iVersion: number): void;" << endl;
    str << TAB << "getVersion(): number;" << endl;
    for (size_t i = 0; i < vOperation.size(); i++)
    {
        OperationPtr &oPtr = vOperation[i];

        string funcReturnGeneric = "<";
        if (oPtr->getReturnPtr()->getTypePtr())
        {
            funcReturnGeneric += getTsType(oPtr->getReturnPtr()->getTypePtr()) + ", ";
        }
        else
        {
            funcReturnGeneric += "undefined, ";
        }

        str << TAB << oPtr->getId() << "(";

        string argType = "";
        vector<ParamDeclPtr> &vParamDecl = oPtr->getAllParamDeclPtr();
        for (size_t j = 0; j < vParamDecl.size(); j++)
        {
            if(vParamDecl[j]->isOut())
            {
                argType += (argType.empty() ? "" : ", ") + vParamDecl[j]->getTypeIdPtr()->getId() + ": " + getTsType(vParamDecl[j]->getTypeIdPtr()->getTypePtr());
                continue;
            }
            str << vParamDecl[j]->getTypeIdPtr()->getId() << ": " << getTsType(vParamDecl[j]->getTypeIdPtr()->getTypePtr());
            str << ", ";
        }

        if (!argType.empty())
        {
            funcReturnGeneric += "{ " + argType + " }>";
        }
        else
        {
            funcReturnGeneric += "undefined>";
        }

        str << "tars_options?: " << IDL_NAMESPACE_STR << "Rpc.InvokeProperty): Promise<" << IDL_NAMESPACE_STR << "Rpc.ProxyResponse" << funcReturnGeneric << ">;" << endl;

        str << TAB << "static " << oPtr->getId() << ": " << IDL_NAMESPACE_STR << "Rpc.SharedFunctionInfo" << ";" << endl;
    }
    DEL_TAB;
    str << TAB << "}" << endl;

    DEL_TAB;

    return str.str();
}

string CodeGenerator::generateDTSProxy(const NamespacePtr &nPtr, bool &bNeedStream, bool &bNeedRpc)
{
    ostringstream str;
    vector<InterfacePtr> &is = nPtr->getAllInterfacePtr();
    if (is.size() > 0)
    {
        bNeedStream = true;
        bNeedRpc = true;
    }
    for (size_t i = 0; i < is.size(); i++)
    {
        str << generateDTSProxy(is[i]) << endl;
    }
    return str.str();
}

void CodeGenerator::generateDTSProxy(const ContextPtr &cPtr)
{
    vector<NamespacePtr> namespaces = cPtr->getNamespaces();

    // generate proxy classes with encoders and decoders
    ostringstream estr;
    bool bNeedStream = false;
    bool bNeedRpc = false;
    for(size_t i = 0; i < namespaces.size(); i++)
    {
        ostringstream kstr;

        kstr << generateDTS(namespaces[i], bNeedStream);
        kstr << generateDTSProxy(namespaces[i], bNeedStream, bNeedRpc);

        estr << generateDTS(namespaces[i], kstr.str());
    }
    if (estr.str().empty())
    {
        return;
    }

    // generate module imports
    ostringstream ostr;
    for (map<string, ImportFile>::iterator it = _mapFiles.begin(); it != _mapFiles.end(); it++)
    {
        if (it->second.sModule.empty()) continue;

        if (estr.str().find(it->second.sModule + ".") == string::npos) continue;

        ostr << "import * as " << it->second.sModule << " from \"" << TC_File::excludeFileExt(it->second.sFile) << "\";" << endl;
    }

    // concat generated code
    ostringstream sstr;
    sstr << printHeaderRemark("Client");
    sstr << DISABLE_TSLINT << endl;
    sstr << DISABLE_ESLINT << endl;
    sstr << endl;
    if (bNeedStream)
    {
        sstr << "import * as " << IDL_NAMESPACE_STR << "Stream from \"" << _sStreamPath << "\";" << endl;
    }
    if (bNeedRpc)
    {
        sstr << "import * as " << IDL_NAMESPACE_STR << "Rpc from \"" << _sRpcPath << "\";" << endl;
    }

    sstr << ostr.str() << endl;
    sstr << estr.str() << endl;

    string sFileName = TC_File::excludeFileExt(_sToPath + TC_File::extractFileName(cPtr->getFileName())) + "Proxy.d.ts";
    TC_File::makeDirRecursive(_sToPath);
    makeUTF8File(sFileName, sstr.str());
}