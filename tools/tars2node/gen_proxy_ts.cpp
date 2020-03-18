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
    str << prefix << "." << TC_Common::lower(protocol) << "Encoder"; \
    str << "(" << sParams << params << "), options, " << prefix << ").then("; \
    str << prefix << "." << TC_Common::lower(protocol) << "Decoder, "; \
    str << prefix << ".errorResponser);" << endl;
#define PROTOCOL_PARAMS (sParams.empty() ? "" : ", ") << "version"

struct SortOperation
{
    bool operator()(const OperationPtr &o1, const OperationPtr &o2)
    {
        return o1->getId() < o2->getId();
    }
};

string CodeGenerator::generateTSProxy(const NamespacePtr &nPtr, const InterfacePtr &pPtr, const OperationPtr &oPtr)
{
    ostringstream str;

    vector<ParamDeclPtr> & vParamDecl = oPtr->getAllParamDeclPtr();
    bool bHasParamOut = false;
    string sParams = "";
    string sParamsWithType = "";

    for (size_t i = 0; i < vParamDecl.size(); i++)
    {
        if (vParamDecl[i]->isOut())
        {
            continue;
        }

        sParams += (sParams.empty() ? "" : ", ")
            + vParamDecl[i]->getTypeIdPtr()->getId();

        sParamsWithType += (sParamsWithType.empty() ? "" : ", ")
            + vParamDecl[i]->getTypeIdPtr()->getId()
            + ": "
            + getTsType(vParamDecl[i]->getTypeIdPtr()->getTypePtr());
    }

    // generate function metadata (SharedFunctionInfo)
    str << TAB << "static " << oPtr->getId() << " = _castFunctionInfo({" << endl;
    INC_TAB;

    str << TAB << "name: \"" << oPtr->getId() << "\"," << endl;
    str << TAB << "return: \"" << getClassName(oPtr->getReturnPtr()->getTypePtr()) << "\"," << endl;
    str << TAB << "arguments: [";
    for (size_t i = 0; i < vParamDecl.size(); i++)
    {
        str << (i > 0 ? ", {" : "{") << endl;
        INC_TAB;

        str << TAB << "name: \"" << vParamDecl[i]->getTypeIdPtr()->getId() << "\"," << endl;
        str << TAB << "class: \"" << getClassName(vParamDecl[i]->getTypeIdPtr()->getTypePtr()) << "\"," << endl;

        if (vParamDecl[i]->isOut())
        {
            bHasParamOut = true;
            str << TAB << "direction: \"out\"" << endl;
        }
        else
        {
            str << TAB << "direction: \"in\"" << endl;
        }

        DEL_TAB;
        str << TAB << "}";
    }
    str << "]," << endl;

    // generate IDL Encoder ($IE)
    str << TAB << TC_Common::lower(IDL_NAMESPACE_STR) << "Encoder(" << sParamsWithType << ") {" << endl;
    INC_TAB;

    str << TAB << "const os = new " << IDL_NAMESPACE_STR << "Stream." << IDL_TYPE << "OutputStream();" << endl;

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
    str << TAB << "}," << endl;

    // generate IDL Decoder ($ID)
    str << TAB << TC_Common::lower(IDL_NAMESPACE_STR) << "Decoder(data: " << IDL_NAMESPACE_STR << "Rpc.RpcResponse) {" << endl;
    INC_TAB;

    str << TAB << "try {" << endl;
    INC_TAB;

    if (oPtr->getReturnPtr()->getTypePtr() || bHasParamOut)
    {
        str << TAB << "var is = new " << IDL_NAMESPACE_STR << "Stream." << IDL_TYPE << "InputStream(data.response.sBuffer);" << endl;
    }

    str << TAB << "return {" << endl;
    INC_TAB;
    str << TAB << "request: data.request," << endl;
    str << TAB << "response: {" << endl;
    INC_TAB;
    str << TAB << "costtime: data.request.costtime," << endl;

    str << TAB << "return: ";
    if (oPtr->getReturnPtr()->getTypePtr())
    {
        str << "is." << toFunctionName(oPtr->getReturnPtr(), "read") << "(0, true, ";

        if (isSimple(oPtr->getReturnPtr()->getTypePtr()))
        {
            str << getDefault(oPtr->getReturnPtr(), oPtr->getReturnPtr()->def(), nPtr->getId(), true, true)
                << representArgument(oPtr->getReturnPtr()->getTypePtr());
        }
        else
        {
            str << getDataType(oPtr->getReturnPtr()->getTypePtr(), true);
        }

        str << ")," << endl;
    }
    else 
    {
        str << "undefined as undefined," << endl;
    }

    str << TAB << "arguments: ";
    if (bHasParamOut)
    {
        str << "{" << endl;
        INC_TAB;

        for (size_t i = 0; i < vParamDecl.size(); i++)
        {
            if (!vParamDecl[i]->isOut()) continue;

            str << TAB << vParamDecl[i]->getTypeIdPtr()->getId()
                << ": is." << toFunctionName(vParamDecl[i]->getTypeIdPtr(), "read") << "(" << (i + 1) << ", true, ";

            if (isSimple(vParamDecl[i]->getTypeIdPtr()->getTypePtr()))
            {
                str << getDefault(vParamDecl[i]->getTypeIdPtr(), vParamDecl[i]->getTypeIdPtr()->def(), nPtr->getId(), true, true)
                    << representArgument(vParamDecl[i]->getTypeIdPtr()->getTypePtr());
            }
            else
            {
                str << getDataType(vParamDecl[i]->getTypeIdPtr()->getTypePtr(), true);
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
        str << TAB << "}" << endl;
    }
    else
    {
        str << "undefined as undefined" << endl;
    }

    DEL_TAB;
    str << TAB << "}" << endl;
    DEL_TAB;
    str << TAB << "};" << endl;
    DEL_TAB;
    str << TAB << "} catch (e) {" << endl;
    INC_TAB;
    str << TAB << "throw _makeError(data, e.message, " << IDL_NAMESPACE_STR << "Rpc.error.CLIENT.DECODE_ERROR);" << endl;
    DEL_TAB;
    str << TAB << "}" << endl;
    DEL_TAB;
    str << TAB << "}," << endl;

    // generate Protocol Encoder ($PE)
    str << TAB << TC_Common::lower(PROTOCOL_VAR) << "Encoder(" << sParamsWithType << (sParamsWithType.empty() ? "" : ", ") << "__$PROTOCOL$VERSION: number) {" << endl;
    INC_TAB;

    str << TAB << "const " << PROTOCOL_VAR << " = new " << IDL_NAMESPACE_STR << "Stream.UniAttribute();" << endl;

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
    str << TAB << "}," << endl;

    // generate Protocol Decoder ($PD)
    str << TAB << TC_Common::lower(PROTOCOL_VAR) << "Decoder(data: " << IDL_NAMESPACE_STR << "Rpc.RpcResponse) {" << endl;
    INC_TAB;

    str << TAB << "try {" << endl;
    INC_TAB;
    if (oPtr->getReturnPtr()->getTypePtr() || bHasParamOut) {
        str << TAB << "const " << PROTOCOL_VAR << ": " << IDL_NAMESPACE_STR << "Stream.UniAttribute = (data.response as any)." << PROTOCOL_VAR << ";" << endl;
    }
    str << TAB << "return {" << endl;
    INC_TAB;
    str << TAB << "request: data.request," << endl;
    str << TAB << "response: {" << endl;
    INC_TAB;
    str << TAB << "costtime: data.request.costtime," << endl;

    str << TAB << "return: ";
    if (oPtr->getReturnPtr()->getTypePtr())
    {
        str << PROTOCOL_VAR << "." << toFunctionName(oPtr->getReturnPtr(), "read") << "(\"\"";

        if (!isSimple(oPtr->getReturnPtr()->getTypePtr()) && !isBinBuffer(oPtr->getReturnPtr()->getTypePtr()))
        {
            str << ", " << getDataType(oPtr->getReturnPtr()->getTypePtr(), true);
        }

        str << ", " << getDefault(oPtr->getReturnPtr(), "", nPtr->getId(), true, true)
                << representArgument(oPtr->getReturnPtr()->getTypePtr());

        str << ")," << endl;
    }
    else 
    {
        str << "undefined as undefined," << endl;
    }

    str << TAB << "arguments: ";
    if (bHasParamOut)
    {
        str << "{" << endl;
        INC_TAB;

        for (size_t i = 0; i < vParamDecl.size(); i++)
        {
            if (!vParamDecl[i]->isOut()) continue;

            str << TAB << vParamDecl[i]->getTypeIdPtr()->getId() << ": "
                    << PROTOCOL_VAR << "." << toFunctionName(vParamDecl[i]->getTypeIdPtr(), "read")
                    << "(\"" << vParamDecl[i]->getTypeIdPtr()->getId() << "\"";

            if (!isSimple(vParamDecl[i]->getTypeIdPtr()->getTypePtr()) && !isBinBuffer(vParamDecl[i]->getTypeIdPtr()->getTypePtr()))
            {
                str << ", " << getDataType(vParamDecl[i]->getTypeIdPtr()->getTypePtr(), true);
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
        str << TAB << "}" << endl;
    }
    else
    {
        str << "undefined as undefined" << endl;
    }

    DEL_TAB;
    str << TAB << "}" << endl;
    DEL_TAB;
    str << TAB << "};" << endl;
    DEL_TAB;
    str << TAB << "} catch (e) {" << endl;
    INC_TAB;
    str << TAB << "throw _makeError(data, e.message, " << IDL_NAMESPACE_STR << "Rpc.error.CLIENT.DECODE_ERROR);" << endl;
    DEL_TAB;
    str << TAB << "}" << endl;
    DEL_TAB;
    str << TAB << "}," << endl;

    // generate error handler ($ER）
    str << TAB << "errorResponser(data: " << IDL_NAMESPACE_STR << "Rpc.RpcResponse) {" << endl;
    INC_TAB;
    str << TAB << "throw _makeError(data, \"Call " << pPtr->getId() << "::" << oPtr->getId() << " failed\");" << endl;
    DEL_TAB;
    str << TAB << "}" << endl;

    DEL_TAB;
    str << TAB << "})" << endl << endl;     // end of metadata

    // generate function body
    str << TAB << oPtr->getId() << "(" << sParamsWithType << (sParamsWithType.empty() ? "" : ", ") << "options?: " << IDL_NAMESPACE_STR << "Rpc.InvokeProperty) {" << endl;
    INC_TAB;

    string sFuncFullName = pPtr->getId() + "Proxy." + oPtr->getId();
    str << TAB << "const version = this._worker.version;" << endl;

    str << TAB << "if (version === " << PROTOCOL_SIMPLE << " || version === " << PROTOCOL_COMPLEX << ") {" << endl;
    INC_TAB;
    INVOKE_RETURN(PROTOCOL_VAR, sFuncFullName, PROTOCOL_PARAMS);
    DEL_TAB;
    str << TAB << "} else {" << endl;
    INC_TAB;
    INVOKE_RETURN(IDL_NAMESPACE_STR, sFuncFullName, "");
    DEL_TAB;
    str << TAB << "}" << endl;

    DEL_TAB;
    str << TAB << "}" << endl << endl;

    return str.str();
}

string CodeGenerator::generateTSProxy(const NamespacePtr &nPtr, const InterfacePtr &pPtr)
{
    ostringstream str;

    vector<OperationPtr> & vOperation = pPtr->getAllOperationPtr();
    sort(vOperation.begin(), vOperation.end(), SortOperation());
    for (size_t i = 0; i < vOperation.size(); i++)
    {
        str << generateTSProxy(nPtr, pPtr, vOperation[i]);
    }

    return str.str();
}

string CodeGenerator::generateTSProxy(const NamespacePtr &nPtr, bool &bNeedStream, bool &bNeedRpc)
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
        str << TAB << "export class " << is[i]->getId() << "Proxy {" << endl;
        INC_TAB;

        str << TAB << "protected _name!: string" << endl;
        str << TAB << "protected _worker!: " << IDL_NAMESPACE_STR << "Rpc.ObjectProxy" << endl << endl;

        str << TAB << "setTimeout (iTimeout: number) { this._worker.timeout = iTimeout; }" << endl;
        str << TAB << "getTimeout () { return this._worker.timeout; }" << endl;
        str << TAB << "setVersion (iVersion: number) { this._worker.version = iVersion; }" << endl;
        str << TAB << "getVersion () { return this._worker.version; }" << endl << endl;

        str << generateTSProxy(nPtr, is[i]) << endl;

        DEL_TAB;
        str << TAB << "}" << endl << endl;
    }

    return str.str();
}

bool CodeGenerator::generateTSProxy(const ContextPtr &cPtr)
{
    vector<NamespacePtr> namespaces = cPtr->getNamespaces();

    // generate proxy classes with encoders and decoders
    ostringstream estr;
    bool bNeedStream = false;
    bool bNeedRpc = false;
    bool bNeedAssert = false;
    bool bQuickFunc = false;
    for(size_t i = 0; i < namespaces.size(); i++)
    {
        ostringstream kstr;

        kstr << generateTS(namespaces[i], bNeedStream, bNeedAssert, bQuickFunc);

        INC_TAB;
        kstr << generateTSProxy(namespaces[i], bNeedStream, bNeedRpc);
        DEL_TAB;

        estr << generateTS(namespaces[i], kstr.str());
    }
    if (estr.str().empty())
    {
        return false;
    }

    // generate module imports
    ostringstream sstr;
    sstr << printHeaderRemark("Client");
    sstr << DISABLE_TSLINT << endl;
    sstr << DISABLE_ESLINT << endl;
    sstr << endl;
    sstr << "/// <reference types=\"node\" />" << endl;
    if (bNeedAssert)
    {
        sstr << TAB << "import assert = require(\"assert\");" << endl;
    }
    if (bNeedStream)
    {
        sstr << "import * as " << IDL_NAMESPACE_STR << "Stream from \"" << _sStreamPath << "\";" << endl;
    }
    if (bNeedRpc)
    {
        sstr << "import * as " << IDL_NAMESPACE_STR << "Rpc from \"" << _sRpcPath << "\";" << endl;
    }

    for (map<string, ImportFile>::iterator it = _mapFiles.begin(); it != _mapFiles.end(); it++)
    {
        if (it->second.sModule.empty()) continue;

        if (estr.str().find(it->second.sModule + ".") == string::npos) continue;

        sstr << "import * as " << it->second.sModule << " from \"" << TC_File::excludeFileExt(it->second.sFile) << "\";" << endl;
    }

    // generate helper functions
    if (bQuickFunc || bNeedRpc)
    {
        sstr << endl;
    }
    if (bQuickFunc)
    {
        sstr << "const _hasOwnProperty = Object.prototype.hasOwnProperty;" << endl;
    }
    if (bNeedRpc)
    {
        sstr << TAB << "function _castFunctionInfo<Ret, Arg>(data: SharedFunctionInfo<Ret, Arg>) { return data; }" << endl;
        sstr << TAB << "function _makeError(data: " << IDL_NAMESPACE_STR << "Rpc.RpcResponse, message: string, type?: number): " << IDL_NAMESPACE_STR << "Rpc.RpcError {" << endl;
        INC_TAB;
        sstr << TAB << "var error: any = new Error(message || \"\");" << endl;
        sstr << TAB << "error.request = data.request;" << endl;
        sstr << TAB << "error.response = {" << endl;
        INC_TAB;
        sstr << TAB << "costtime: data.request.costtime" << endl;
        DEL_TAB;
        sstr << TAB << "};" << endl;
        sstr << TAB << "if (type === " << IDL_NAMESPACE_STR << "Rpc.error.CLIENT.DECODE_ERROR) {" << endl;
        INC_TAB;
        sstr << TAB << "error.name = \"DECODE_ERROR\";" << endl;
        sstr << TAB << "error.response.error = {" << endl;
        INC_TAB;
        sstr << TAB << "code: type," << endl;
        sstr << TAB << "message: message" << endl;
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
        sstr << TAB << "}" << endl << endl;

        sstr << "export interface SharedFunctionInfo<Ret = any, Arg = any> extends " << IDL_NAMESPACE_STR << "Rpc.SharedFunctionInfo {" << endl;
        INC_TAB;
        sstr << TAB << TC_Common::lower(IDL_NAMESPACE_STR) << "Encoder (...args: any[]): " << IDL_NAMESPACE_STR << "Stream.BinBuffer," << endl;
        sstr << TAB << TC_Common::lower(IDL_NAMESPACE_STR) << "Decoder (data: " << IDL_NAMESPACE_STR << "Rpc.RpcResponse): " << IDL_NAMESPACE_STR << "Rpc.ProxyResponse<Ret, Arg>," << endl;
        sstr << TAB << TC_Common::lower(PROTOCOL_VAR) << "Encoder (...args: any[]): " << IDL_NAMESPACE_STR << "Stream.UniAttribute," << endl;
        sstr << TAB << TC_Common::lower(PROTOCOL_VAR) << "Decoder (data: " << IDL_NAMESPACE_STR << "Rpc.RpcResponse): " << IDL_NAMESPACE_STR << "Rpc.ProxyResponse<Ret, Arg>," << endl;
        sstr << TAB << "errorResponser (data: " << IDL_NAMESPACE_STR << "Rpc.RpcResponse): never" << endl;
        DEL_TAB;
        sstr << TAB << "}" << endl << endl;
    }

    sstr << estr.str() << endl;

    string sFileName = TC_File::excludeFileExt(_sToPath + TC_File::extractFileName(cPtr->getFileName())) + "Proxy.ts";
    TC_File::makeDirRecursive(_sToPath);
    makeUTF8File(sFileName, sstr.str());

    return true;
}
