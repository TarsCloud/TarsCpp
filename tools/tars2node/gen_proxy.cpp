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

#include "code_generator.h"

#define INVOKE_RETURN(protocol, prefix, params) \
    str << TAB << "return this._worker." << TC_Common::lower(protocol) << "_invoke(\"" << oPtr->getId() << "\", "; \
    str << "__" << nPtr->getId() << "_" << pPtr->getId() << "$" << oPtr->getId() << "$" << prefix << "E"; \
    str << "(" << sParams << params << "), arguments[arguments.length - 1], "; \
    str << "__" << nPtr->getId() << "_" << pPtr->getId() << "$" << oPtr->getId() << "$IF" << ").then("; \
    str << "__" << nPtr->getId() << "_" << pPtr->getId() << "$" << oPtr->getId() << "$" << prefix << "D, "; \
    str << "__" << nPtr->getId() << "_" << pPtr->getId() << "$" << oPtr->getId() << "$ER);" << endl;
#define PROTOCOL_PARAMS (sParams.empty() ? "" : ", ") << "version"

struct SortOperation
{
    bool operator()(const OperationPtr &o1, const OperationPtr &o2)
    {
        return o1->getId() < o2->getId();
    }
};

string CodeGenerator::generateJSProxy(const NamespacePtr &nPtr, const InterfacePtr &pPtr, const OperationPtr &oPtr)
{
    ostringstream str;

    vector<ParamDeclPtr> & vParamDecl = oPtr->getAllParamDeclPtr();
    bool bHasParamOut = false;
    string sParams = "";

    for (size_t i = 0; i < vParamDecl.size(); i++)
    {
        if (vParamDecl[i]->isOut())
        {
            continue;
        }

        sParams += (sParams.empty()?"":", ") + vParamDecl[i]->getTypeIdPtr()->getId();
    }

    // generate function metadata (SharedFunctionInfo)
    str << TAB << "var __" << nPtr->getId() << "_" << pPtr->getId() << "$" << oPtr->getId() << "$IF = {" << endl;

    INC_TAB;

    str << TAB << "\"name\" : \"" << oPtr->getId() << "\"," << endl;
    str << TAB << "\"return\" : \"" << getClassName(oPtr->getReturnPtr()->getTypePtr()) << "\"," << endl;
    str << TAB << "\"arguments\" : [";
    for (size_t i = 0; i < vParamDecl.size(); i++)
    {
        str << (i > 0 ? ", {" : "{") << endl;

        INC_TAB;

        str << TAB << "\"name\" : \"" << vParamDecl[i]->getTypeIdPtr()->getId() << "\"," << endl;
        str << TAB << "\"class\" : \"" << getClassName(vParamDecl[i]->getTypeIdPtr()->getTypePtr()) << "\"," << endl;

        if (vParamDecl[i]->isOut())
        {
            bHasParamOut = true;
            str << TAB << "\"direction\" : \"out\"" << endl; 
        }
        else
        {
            str << TAB << "\"direction\" : \"in\"" << endl; 
        }

        DEL_TAB;

        str << TAB << "}";
    }
    str << "]" << endl;

    DEL_TAB;

    str << TAB << "};" << endl << endl;

    // generate IDL Encoder ($IE)
    str << TAB << "var __" << nPtr->getId() << "_" << pPtr->getId() << "$" << oPtr->getId() << "$IE = function (" << sParams << ") {" << endl;
    
    INC_TAB;

    str << TAB << "var os = new " << IDL_NAMESPACE_STR << "Stream." << IDL_TYPE << "OutputStream();" << endl;

    for (size_t i = 0; i < vParamDecl.size(); i++)
    {
        if (vParamDecl[i]->isOut()) continue;

        str << TAB << "os." << toFunctionName(vParamDecl[i]->getTypeIdPtr(), "write") << "(" 
            << (i + 1) << ", " << vParamDecl[i]->getTypeIdPtr()->getId()
            << representArgument(vParamDecl[i]->getTypeIdPtr()->getTypePtr()) << ");" << endl;

        // push the symbol into dependent list
        getDataType(vParamDecl[i]->getTypeIdPtr()->getTypePtr());
    }

    str << TAB << "return os.getBinBuffer();" << endl;

    DEL_TAB;

    str << TAB << "};" << endl << endl;

    // generate IDL Decoder ($ID)
    str << TAB << "var __" << nPtr->getId() << "_" << pPtr->getId() << "$" << oPtr->getId() << "$ID = function (data) {" << endl;
    INC_TAB;
    str << TAB << "try {" << endl;
    INC_TAB;

    if (oPtr->getReturnPtr()->getTypePtr() || bHasParamOut)
    {
        str << TAB << "var is = new " << IDL_NAMESPACE_STR << "Stream." << IDL_TYPE << "InputStream(data.response.sBuffer);" << endl;
    }

    str << TAB << "return {" << endl;
    INC_TAB;
    str << TAB << "\"request\" : data.request," << endl;
    str << TAB << "\"response\" : {" << endl;
    INC_TAB;
    str << TAB << "\"costtime\" : data.request.costtime";

    if (oPtr->getReturnPtr()->getTypePtr())
    {
        str << "," << endl;
        str << TAB << "\"return\""
            << " : is." << toFunctionName(oPtr->getReturnPtr(), "read") << "(0, true, ";

        if (isSimple(oPtr->getReturnPtr()->getTypePtr()))
        {
            str << getDefault(oPtr->getReturnPtr(), oPtr->getReturnPtr()->def(), nPtr->getId())
                << representArgument(oPtr->getReturnPtr()->getTypePtr());
        }
        else
        {
            str << getDataType(oPtr->getReturnPtr()->getTypePtr());
        }

        str << ")";
    }

    if (bHasParamOut)
    {
        str << "," << endl;
        str << TAB << "\"arguments\" : {" << endl;

        INC_TAB;

        for (size_t i = 0; i < vParamDecl.size(); i++)
        {
            if (!vParamDecl[i]->isOut()) continue;

            str << TAB << "\"" << vParamDecl[i]->getTypeIdPtr()->getId() << "\""
                << " : is." << toFunctionName(vParamDecl[i]->getTypeIdPtr(), "read") << "(" << (i + 1) << ", true, ";

            if (isSimple(vParamDecl[i]->getTypeIdPtr()->getTypePtr()))
            {
                str << getDefault(vParamDecl[i]->getTypeIdPtr(), vParamDecl[i]->getTypeIdPtr()->def(), nPtr->getId())
                    << representArgument(vParamDecl[i]->getTypeIdPtr()->getTypePtr());
            }
            else
            {
                str << getDataType(vParamDecl[i]->getTypeIdPtr()->getTypePtr());
            }

            str << ")";

            if (i == vParamDecl.size() - 1)
            {
                str << endl;
            }
            else
            {
                str << "," << endl;
            }

        }

        DEL_TAB;
        str << TAB << "}";
    }

    str << endl;
    DEL_TAB;
    str << TAB << "}" << endl;
    DEL_TAB;
    str << TAB << "};" << endl;
    DEL_TAB;
    str << TAB << "} catch (e) {" << endl;
    INC_TAB;
    str << TAB << "throw _makeError(data, e.message, " << IDL_NAMESPACE_STR << "Error.CLIENT.DECODE_ERROR);" << endl;
    DEL_TAB;
    str << TAB << "}" << endl;
    DEL_TAB;
    str << TAB << "};" << endl << endl;

    // generate Protocol Encoder ($PE)
    str << TAB << "var __" << nPtr->getId() << "_" << pPtr->getId() << "$" << oPtr->getId() << "$PE = function (" 
        << sParams << (sParams.empty() ? "" : ", ") << "__$PROTOCOL$VERSION) {" << endl;
    
    INC_TAB;

    str << TAB << "var " << PROTOCOL_VAR << " = new " << IDL_NAMESPACE_STR << "Stream.UniAttribute();" << endl;

    str << TAB << PROTOCOL_VAR << "." << PROTOCOL_VAR << "Version = __$PROTOCOL$VERSION;" << endl;

    for (size_t i = 0; i < vParamDecl.size(); i++)
    {
        if (vParamDecl[i]->isOut()) continue;

        str << TAB << PROTOCOL_VAR << "." << toFunctionName(vParamDecl[i]->getTypeIdPtr(), "write") << "(\"" 
            << vParamDecl[i]->getTypeIdPtr()->getId() << "\", " << vParamDecl[i]->getTypeIdPtr()->getId()
            << representArgument(vParamDecl[i]->getTypeIdPtr()->getTypePtr()) << ");" << endl;

        // push the symbol into dependent list
        getDataType(vParamDecl[i]->getTypeIdPtr()->getTypePtr());
    }

    str << TAB << "return " << PROTOCOL_VAR << ";" << endl;
    DEL_TAB;
    str << TAB << "};" << endl << endl;

    // generate Protocol Decoder ($PD)
    str << TAB << "var __" << nPtr->getId() << "_" << pPtr->getId() << "$" << oPtr->getId() << "$PD = function (data) {" << endl;
    INC_TAB;
    str << TAB << "try {" << endl;
    INC_TAB;
    if (oPtr->getReturnPtr()->getTypePtr() || bHasParamOut) {
        str << TAB << "var " << PROTOCOL_VAR << " = data.response." << PROTOCOL_VAR << ";" << endl;
    }
    str << TAB << "return {" << endl;
    INC_TAB;
    str << TAB << "\"request\" : data.request," << endl;
    str << TAB << "\"response\" : {" << endl;
    INC_TAB;
    str << TAB << "\"costtime\" : data.request.costtime";

    if (oPtr->getReturnPtr()->getTypePtr())
    {
        str << "," << endl;
        str << TAB << "\"return\" : " << PROTOCOL_VAR << "." << toFunctionName(oPtr->getReturnPtr(), "read") << "(\"\"";

        if (!isSimple(oPtr->getReturnPtr()->getTypePtr()) && !isBinBuffer(oPtr->getReturnPtr()->getTypePtr()))
        {
            str << ", " << getDataType(oPtr->getReturnPtr()->getTypePtr());
        }

        str << ", " << getDefault(oPtr->getReturnPtr(), "", nPtr->getId(), true)
                << representArgument(oPtr->getReturnPtr()->getTypePtr());
        
        str << ")";
    }

    if (bHasParamOut)
    {
        str << "," << endl;
        str << TAB << "\"arguments\" : {" << endl;

        INC_TAB;

        for (size_t i = 0; i < vParamDecl.size(); i++)
        {
            if (!vParamDecl[i]->isOut()) continue;

            str << TAB << "\"" << vParamDecl[i]->getTypeIdPtr()->getId() << "\" : "
                    << PROTOCOL_VAR << "." << toFunctionName(vParamDecl[i]->getTypeIdPtr(), "read")
                    << "(\"" << vParamDecl[i]->getTypeIdPtr()->getId() << "\"";

            if (!isSimple(vParamDecl[i]->getTypeIdPtr()->getTypePtr()) && !isBinBuffer(vParamDecl[i]->getTypeIdPtr()->getTypePtr()))
            {
                str << ", " << getDataType(vParamDecl[i]->getTypeIdPtr()->getTypePtr());
            }

            str << ")";

            if (i == vParamDecl.size() - 1)
            {
                str << endl;
            }
            else
            {
                str << "," << endl;
            }

        }

        DEL_TAB;
        str << TAB << "}";
    }

    str << endl;
    DEL_TAB;
    str << TAB << "}" << endl;
    DEL_TAB;
    str << TAB << "};" << endl;
    DEL_TAB;
    str << TAB << "} catch (e) {" << endl;
    INC_TAB;
    str << TAB << "throw _makeError(data, e.message, " << IDL_NAMESPACE_STR << "Error.CLIENT.DECODE_ERROR);" << endl;
    DEL_TAB;
    str << TAB << "}" << endl;
    DEL_TAB;
    str << TAB << "};" << endl << endl;


    // generate error handler ($ER）
    str << TAB << "var __" << nPtr->getId() << "_" << pPtr->getId() << "$" << oPtr->getId() << "$ER = function (data) {" << endl;
    INC_TAB;
    str << TAB << "throw _makeError(data, \"Call " << pPtr->getId() << "::" << oPtr->getId() << " failed\");" << endl;
    DEL_TAB;
    str << TAB << "};" << endl << endl;


    // generate function body
    str << TAB << nPtr->getId() << "." << pPtr->getId() << "Proxy.prototype." << oPtr->getId() << " = function ("
        << sParams << ") {" << endl;

    INC_TAB;

    str << TAB << "var version = this._worker.version;" << endl;

    str << TAB << "if (version === " << PROTOCOL_SIMPLE << " || version === " << PROTOCOL_COMPLEX << ") {" << endl;
    INC_TAB;
    INVOKE_RETURN(PROTOCOL_VAR, "P", PROTOCOL_PARAMS);
    DEL_TAB;
    str << TAB << "} else {" << endl;
    INC_TAB;
    INVOKE_RETURN(IDL_NAMESPACE_STR, "I", "");
    DEL_TAB;
    str << TAB << "}" << endl;

    DEL_TAB;

    str << TAB << "};" << endl;

    // add the function into the prototype of the proxy class
    str << TAB << nPtr->getId() << "." << pPtr->getId() << "Proxy." << oPtr->getId() << " = "
            << "__" << nPtr->getId() << "_" << pPtr->getId() << "$" << oPtr->getId() << "$IF;" << endl;

    return str.str();
}

string CodeGenerator::generateJSProxy(const NamespacePtr &nPtr, const InterfacePtr &pPtr)
{
    ostringstream str;

    vector<OperationPtr> & vOperation = pPtr->getAllOperationPtr();
    sort(vOperation.begin(), vOperation.end(), SortOperation());
    for (size_t i = 0; i < vOperation.size(); i++)
    {
        str << generateJSProxy(nPtr, pPtr, vOperation[i]) << endl;
    }

    return str.str();
}

string CodeGenerator::generateJSProxy(const NamespacePtr &nPtr, bool &bNeedRpc, bool &bNeedStream)
{
    ostringstream str;

    vector<InterfacePtr> & is = nPtr->getAllInterfacePtr();
    for (size_t i = 0; i < is.size(); i++)
    {
        str << generateJSProxy(nPtr, is[i]) << endl;
    }
    if (is.size() != 0)
    {
        bNeedRpc = true;
        bNeedStream = true;
    }

    return str.str();
}

bool CodeGenerator::generateJSProxy(const ContextPtr &cPtr)
{
    vector<NamespacePtr> namespaces = cPtr->getNamespaces();

    ostringstream istr;
    set<string> setNamespace;
    for(size_t i = 0; i < namespaces.size(); i++)
    {
        if (setNamespace.count(namespaces[i]->getId()) == 0)
        {
            istr << TAB << "var " << namespaces[i]->getId() << " = " << namespaces[i]->getId() << " || {};" << endl;
            istr << TAB << "module.exports." << namespaces[i]->getId() << " = " << namespaces[i]->getId() << ";" << endl << endl;

            setNamespace.insert(namespaces[i]->getId());
        }
    }

    set<string> setInterface;
    for(size_t i = 0; i < namespaces.size(); i++) 
    {
        vector<InterfacePtr> & is = namespaces[i]->getAllInterfacePtr();
        for (size_t ii = 0; ii < is.size(); ii++)
        {
            if (setInterface.count(namespaces[i]->getId() + "::" + is[ii]->getId()) != 0)
            {
                continue;
            }
            setInterface.insert(namespaces[i]->getId() + "::" + is[ii]->getId());

            istr << TAB << namespaces[i]->getId() << "." << is[ii]->getId() << "Proxy = function () {" << endl;
            INC_TAB;
            istr << TAB << "this._name    = undefined;" << endl;
            istr << TAB << "this._worker  = undefined;" << endl;
            DEL_TAB;
            istr << TAB << "};" << endl << endl;

            istr << TAB << namespaces[i]->getId() << "." << is[ii]->getId() << "Proxy.prototype.setTimeout = function (iTimeout) {" << endl;
            INC_TAB;
            istr << TAB << "this._worker.timeout = iTimeout;" << endl;
            DEL_TAB;
            istr << TAB << "};" << endl << endl;

            istr << TAB << namespaces[i]->getId() << "." << is[ii]->getId() << "Proxy.prototype.getTimeout = function () {" << endl;
            INC_TAB;
            istr << TAB << "return this._worker.timeout;" << endl;
            DEL_TAB;
            istr << TAB << "};" << endl << endl;

            istr << TAB << namespaces[i]->getId() << "." << is[ii]->getId() << "Proxy.prototype.setVersion = function (iVersion) {" << endl;
            INC_TAB;
            istr << TAB << "this._worker.version = iVersion;" << endl;
            DEL_TAB;
            istr << TAB << "};" << endl << endl;

            istr << TAB << namespaces[i]->getId() << "." << is[ii]->getId() << "Proxy.prototype.getVersion = function () {" << endl;
            INC_TAB;
            istr << TAB << "return this._worker.version;" << endl;
            DEL_TAB;
            istr << TAB << "};" << endl;
        }
    }

    // generate proxy classes with encoders and decoders
    ostringstream estr;
    bool bNeedAssert = false;
    bool bNeedStream = false;
    bool bQuickFunc = false;
	for(size_t i = 0; i < namespaces.size(); i++)
	{
		estr << generateJS(namespaces[i], bNeedStream, bNeedAssert, bQuickFunc);
	}

    bool bNeedRpc = false;
    for(size_t i = 0; i < namespaces.size(); i++)
    {
        estr << generateJSProxy(namespaces[i], bNeedRpc, bNeedStream);
    }

    if (estr.str().empty())
    {
        return false;
    }

    // generate module imports
    ostringstream ostr;
    for (map<string, ImportFile>::iterator it = _mapFiles.begin(); it != _mapFiles.end(); it++)
    {
        if (it->second.sModule.empty()) continue;

        if (estr.str().find(it->second.sModule + ".") == string::npos) continue;

        ostr << "var " << it->second.sModule << " = require(\"" << it->second.sFile << "\");" << endl;
    }

    // concat generated code    
    ostringstream sstr;
    sstr << printHeaderRemark("Client");
    sstr << DISABLE_ESLINT << endl;
    sstr << endl;
    sstr << "\"use strict\";" << endl << endl;
    if (bNeedAssert)
    {
        sstr << "var assert    = require(\"assert\");" << endl;
    }
    if (bNeedStream)
    {
        sstr << "var " << IDL_NAMESPACE_STR << "Stream = require(\"" << _sStreamPath << "\");" << endl;
    }
    if (bNeedRpc)
    {
        sstr << "var " << IDL_NAMESPACE_STR << "Error  = require(\"" << _sRpcPath << "\").error;" << endl;
    }

    sstr << ostr.str() << endl;

    // generate helper functions
    if (bQuickFunc)
    {
        sstr << "var _hasOwnProperty = Object.prototype.hasOwnProperty;" << endl;
    }
    if (bNeedRpc)
    {
        sstr << TAB << "var _makeError = function (data, message, type) {" << endl;
        INC_TAB;
        sstr << TAB << "var error = new Error(message || \"\");" << endl;
        sstr << TAB << "error.request = data.request;" << endl;
        sstr << TAB << "error.response = {" << endl;
        INC_TAB;
        sstr << TAB << "\"costtime\" : data.request.costtime" << endl;
        DEL_TAB;
        sstr << TAB << "};" << endl;
        sstr << TAB << "if (type === " << IDL_NAMESPACE_STR << "Error.CLIENT.DECODE_ERROR) {" << endl;
        INC_TAB;
        sstr << TAB << "error.name = \"DECODE_ERROR\";" << endl;
        sstr << TAB << "error.response.error = {" << endl;
        INC_TAB;
        sstr << TAB << "\"code\" : type," << endl;
        sstr << TAB << "\"message\" : message" << endl;
        DEL_TAB;
        sstr << TAB << "};" << endl;
        DEL_TAB;
        sstr << TAB << "} else {" << endl;
        INC_TAB;
        sstr << TAB << "error.name = \"RPC_ERROR\";" << endl;
        sstr << TAB << "error.response.error = data.error;" << endl;
        DEL_TAB;
        sstr << TAB << "}" << endl;
        sstr << TAB << "return error;" << endl;
        DEL_TAB;
        sstr << TAB << "};" << endl;
    }
    if (bQuickFunc || bNeedRpc)
    {
        sstr << endl;
    }

    sstr << istr.str() << endl;
    sstr << estr.str() << endl;

    string sFileName = TC_File::excludeFileExt(_sToPath + TC_File::extractFileName(cPtr->getFileName())) + "Proxy.js";

    TC_File::makeDirRecursive(_sToPath);
    makeUTF8File(sFileName, sstr.str());

    return true;
}

#undef INVOKE_RETURN
#undef PROTOCOL_PARAMS