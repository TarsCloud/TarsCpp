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

string CodeGenerator::generateTSServerAsync(const NamespacePtr &nPtr, const InterfacePtr &pPtr, const OperationPtr &oPtr)
{
    ostringstream str;

    string sParams = "";
    int rspNum = 0;
    if (oPtr->getReturnPtr()->getTypePtr())
    {
        sParams += "_ret: " + getTsType(oPtr->getReturnPtr()->getTypePtr());

        // push the symbol into dependent list
        getDataType(oPtr->getReturnPtr()->getTypePtr());
        ++rspNum;
    }

    vector<ParamDeclPtr> &vParamDecl = oPtr->getAllParamDeclPtr();
    for (size_t i = 0; i < vParamDecl.size(); i++)
    {
        if (!vParamDecl[i]->isOut())
            continue;

        sParams += (sParams.empty() ? "" : ", ") + vParamDecl[i]->getTypeIdPtr()->getId();
        sParams += ": " + getTsType(vParamDecl[i]->getTypeIdPtr()->getTypePtr());
        ++rspNum;
    }

    str << TAB << "protected static __" << oPtr->getId() << "_responser(this: " << IDL_NAMESPACE_STR << "Rpc.TarsCurrent, " << sParams << ") {" << endl;

    INC_TAB;

    if (sParams.empty())
    {
        str << TAB << "this.doResponse(new " << IDL_NAMESPACE_STR << "Stream.BinBuffer());" << endl;
        DEL_TAB;
        str << TAB << "}" << endl;

        return str.str();
    }

    str << TAB << "if (this.getRequestVersion() === " << PROTOCOL_SIMPLE << " || this.getRequestVersion() === " << PROTOCOL_COMPLEX << ") {" << endl;
    INC_TAB;
    str << TAB << "const " << PROTOCOL_VAR << " = new " << IDL_NAMESPACE_STR << "Stream.UniAttribute();" << endl;
    str << TAB << PROTOCOL_VAR << "." << PROTOCOL_VAR << "Version = this.getRequestVersion();" << endl;
    if (oPtr->getReturnPtr()->getTypePtr())
    {
        str << TAB << PROTOCOL_VAR << "." << toFunctionName(oPtr->getReturnPtr(), "write") << "(\"\", _ret"
            << representArgument(oPtr->getReturnPtr()->getTypePtr()) << ");" << endl;
    }
    for (size_t i = 0; i < vParamDecl.size(); i++)
    {
        if (!vParamDecl[i]->isOut())
            continue;

        str << TAB << PROTOCOL_VAR << "." << toFunctionName(vParamDecl[i]->getTypeIdPtr(), "write") << "(\""
            << vParamDecl[i]->getTypeIdPtr()->getId() << "\", " << vParamDecl[i]->getTypeIdPtr()->getId()
            << representArgument(vParamDecl[i]->getTypeIdPtr()->getTypePtr()) << ");" << endl;
    }
    str << endl;
    str << TAB << "this.doResponse(" << PROTOCOL_VAR << ".encode());" << endl;
    DEL_TAB;

    //// ========= 增加对 JSON_VERSION 支持
    str << TAB << "} else if (this.getRequestVersion() === " << PROTOCOL_JSON << ") {" << endl;

    INC_TAB;
    str << TAB << "const _data_ = {" << endl;
    INC_TAB;
    if (oPtr->getReturnPtr()->getTypePtr())
    {
        str << TAB << "\"tars_ret\": _ret";
        --rspNum;
        if (rspNum > 0)
        {
            str << ", " << endl;
        }
    }
    for (size_t i = 0; i < vParamDecl.size(); i++)
    {
        if (!vParamDecl[i]->isOut())
            continue;

        str << TAB << "\"" << vParamDecl[i]->getTypeIdPtr()->getId() << "\": " << toObjectString(vParamDecl[i]->getTypeIdPtr());
        --rspNum;
        if (rspNum > 0)
        {
            str << ", " << endl;
        }
    }
    DEL_TAB;
    str << endl;
    str << TAB << "};" << endl;
    str << endl;
    str << TAB << "this.doResponse(new TarsStream.BinBuffer(Buffer.from(JSON.stringify(_data_))));" << endl;
    DEL_TAB;
    //// ========= 
    
    str << TAB << "} else {" << endl;

    INC_TAB;
    str << TAB << "const os = new " << IDL_NAMESPACE_STR << "Stream." << IDL_TYPE << "OutputStream();" << endl;
    if (oPtr->getReturnPtr()->getTypePtr())
    {
        str << TAB << "os." << toFunctionName(oPtr->getReturnPtr(), "write") << "(0, _ret"
            << representArgument(oPtr->getReturnPtr()->getTypePtr()) << ");" << endl;
    }
    for (size_t i = 0; i < vParamDecl.size(); i++)
    {
        if (!vParamDecl[i]->isOut())
            continue;

        str << TAB << "os." << toFunctionName(vParamDecl[i]->getTypeIdPtr(), "write") << "("
            << (i + 1) << ", " << vParamDecl[i]->getTypeIdPtr()->getId()
            << representArgument(vParamDecl[i]->getTypeIdPtr()->getTypePtr()) << ");" << endl;
    }
    str << endl;
    str << TAB << "this.doResponse(os.getBinBuffer());" << endl;
    DEL_TAB;
    str << TAB << "}" << endl;

    DEL_TAB;

    str << TAB << "}" << endl;

    return str.str();
}

string CodeGenerator::generateTSServerDispatch(const NamespacePtr &nPtr, const InterfacePtr &pPtr, const OperationPtr &oPtr)
{
    ostringstream str;
    vector<ParamDeclPtr> & vParamDecl = oPtr->getAllParamDeclPtr();

    str << TAB << "protected __" << oPtr->getId() << "(current: " << IDL_NAMESPACE_STR << "Rpc.TarsCurrent";
    if (vParamDecl.size() != 0) str << ", binBuffer: " << IDL_NAMESPACE_STR << "Stream.BinBuffer";
    str << ") {" << endl;

    INC_TAB;

    ostringstream dstr;

    for (size_t i = 0; i < vParamDecl.size(); i++)
    {
        dstr << TAB << "let " << vParamDecl[i]->getTypeIdPtr()->getId() << ": " << getTsType(vParamDecl[i]->getTypeIdPtr()->getTypePtr()) << ";" << endl;
    }
    if (vParamDecl.size() != 0)
    {
        dstr << endl;
    }

    dstr << TAB << "if (current.getRequestVersion() === " << PROTOCOL_SIMPLE << " || current.getRequestVersion() === " << PROTOCOL_COMPLEX << ") {" << endl;
    INC_TAB;
    dstr << TAB << "const " << PROTOCOL_VAR << " = new " << IDL_NAMESPACE_STR << "Stream.UniAttribute();" << endl;
    dstr << TAB << PROTOCOL_VAR << "." << PROTOCOL_VAR << "Version = current.getRequestVersion();" << endl;
    dstr << TAB << PROTOCOL_VAR << ".decode(binBuffer);" << endl;

    for (size_t i = 0; i < vParamDecl.size(); i++)
    {
        dstr << TAB << vParamDecl[i]->getTypeIdPtr()->getId()
                << " = " << PROTOCOL_VAR << "." << toFunctionName(vParamDecl[i]->getTypeIdPtr(), "read")
                << "(\"" << vParamDecl[i]->getTypeIdPtr()->getId() << "\"";

        if (!isSimple(vParamDecl[i]->getTypeIdPtr()->getTypePtr()) && !isBinBuffer(vParamDecl[i]->getTypeIdPtr()->getTypePtr()))
        {
            dstr << ", " << getDataType(vParamDecl[i]->getTypeIdPtr()->getTypePtr(), true);
        }

        if (vParamDecl[i]->isOut())
        {
            dstr << ", " << getDefault(vParamDecl[i]->getTypeIdPtr(), "", nPtr->getId(), true, true)
                    << representArgument(vParamDecl[i]->getTypeIdPtr()->getTypePtr());
        }

        dstr << ");" << endl;
    }
    DEL_TAB;

    //// ========= 增加对 JSON_VERSION 支持
    dstr << TAB << "} else if (current.getRequestVersion() === " << PROTOCOL_JSON << ") {" << endl;
    INC_TAB;
    dstr << TAB << "const _data_ = JSON.parse(binBuffer.toNodeBuffer().toString());" << endl;

    for (size_t i = 0; i < vParamDecl.size(); i++)
    {
        if (vParamDecl[i]->isOut())
        {
            dstr << TAB << vParamDecl[i]->getTypeIdPtr()->getId()
                 << " = _data_." << vParamDecl[i]->getTypeIdPtr()->getId() << " || " << getDefault(vParamDecl[i]->getTypeIdPtr(), "", nPtr->getId(), true)
                 << ";" << endl;
        }
        else
        {
            dstr << TAB << vParamDecl[i]->getTypeIdPtr()->getId()
                 << " = _data_." << vParamDecl[i]->getTypeIdPtr()->getId()
                 << ";" << endl;
        }
    }
    DEL_TAB;
    //// =========

    dstr << TAB << "} else {" << endl;

    INC_TAB;
    dstr << TAB << "const is = new " << IDL_NAMESPACE_STR << "Stream." << IDL_TYPE << "InputStream(binBuffer);" << endl;
    string sParams = "";
    for (size_t i = 0; i < vParamDecl.size(); i++)
    {
        sParams += ", " + vParamDecl[i]->getTypeIdPtr()->getId();

        dstr << TAB << vParamDecl[i]->getTypeIdPtr()->getId()
                << " = is." << toFunctionName(vParamDecl[i]->getTypeIdPtr(), "read") << "("
                << (i + 1) << ", " << (vParamDecl[i]->isOut() ? "false" : "true") << ", ";

        if (isSimple(vParamDecl[i]->getTypeIdPtr()->getTypePtr()))
        {
            dstr << getDefault(vParamDecl[i]->getTypeIdPtr(), vParamDecl[i]->getTypeIdPtr()->def(), nPtr->getId(), true, true)
                    << representArgument(vParamDecl[i]->getTypeIdPtr()->getTypePtr());
        }
        else
        {
            dstr << getDataType(vParamDecl[i]->getTypeIdPtr()->getTypePtr(), true);
        }

        dstr << ");" << endl;
    }
    DEL_TAB;
    dstr << TAB << "}" << endl << endl;

    if (!sParams.empty())
    {
        str << dstr.str();
    }

    str << TAB << "current.sendResponse = " << pPtr->getId() << "Imp.__" << oPtr->getId() << "_responser;" << endl << endl;

    str << TAB << "this." << oPtr->getId() << "(current" << sParams << ");" << endl << endl;

    str << TAB << "return " << IDL_NAMESPACE_STR << "Rpc.error.SUCCESS;" << endl;

    DEL_TAB;

    str << TAB << "}" << endl;

    return str.str();
}

string CodeGenerator::generateTSServer(const NamespacePtr &pPtr, bool &bNeedStream, bool &bNeedRpc, bool &bNeedAssert)
{
	ostringstream str;

    vector<InterfacePtr> & is = pPtr->getAllInterfacePtr();
    for (size_t i = 0; i < is.size(); i++)
    {
        str << generateTSServer(is[i], pPtr) << endl;
    }
    if (is.size() != 0)
    {
        bNeedRpc = true;
        bNeedStream = true;
        bNeedAssert = true;
    }

	return str.str();
}

string CodeGenerator::generateTSServer(const InterfacePtr &pPtr, const NamespacePtr &nPtr)
{
    ostringstream str;
    vector<OperationPtr> & vOperation = pPtr->getAllOperationPtr();

    // generate the implementation class
    str << TAB << "export abstract class " << pPtr->getId() << "Imp { " << endl;
    INC_TAB;
    str << TAB << "_name!: string" << endl;
    str << TAB << "_worker!: any" << endl << endl;

    // generate the initialize function
    str << TAB << "initialize(): PromiseLike<any> | void {}" << endl << endl;

    // generate the dispatch function
    str << TAB << "onDispatch(current: " << IDL_NAMESPACE_STR << "Rpc.TarsCurrent, funcName: string, binBuffer: " << IDL_NAMESPACE_STR << "Stream.BinBuffer) { " << endl;
    INC_TAB;
    str << TAB << "if (\"__\" + funcName in this) {" << endl;
    INC_TAB;
    str << TAB << "return (this as any)[\"__\" + funcName](current, binBuffer);" << endl;
    DEL_TAB;
    str << TAB << "} else {" << endl;
    INC_TAB;
    str << TAB << "return " << IDL_NAMESPACE_STR << "Rpc.error.SERVER.FUNC_NOT_FOUND;" << endl;
    DEL_TAB;
    str << TAB << "}" << endl;
    DEL_TAB;
    str << TAB << "}" << endl << endl;

    // generate the ping function
    str << TAB << "__" << TC_Common::lower(IDL_NAMESPACE_STR) << "_ping(current: " << IDL_NAMESPACE_STR << "Rpc.TarsCurrent) { " << endl;
    INC_TAB;
    str << TAB << "const _ret = 0;" << endl;
    str << TAB << "if (current.getRequestVersion() === " << PROTOCOL_SIMPLE << " || current.getRequestVersion() === " << PROTOCOL_COMPLEX << ") {" << endl;
    INC_TAB;
    str << TAB << "const " << PROTOCOL_VAR << " = new " << IDL_NAMESPACE_STR << "Stream.UniAttribute();" << endl;
    str << TAB << PROTOCOL_VAR << "." << PROTOCOL_VAR << "Version = current.getRequestVersion();" << endl;
    str << TAB << PROTOCOL_VAR << ".writeInt32(\"\", _ret);" << endl << endl;
    str << TAB << "current.doResponse(" << PROTOCOL_VAR << ".encode());" << endl;
    DEL_TAB;
    str << TAB << "} else {" << endl;
    INC_TAB;
    str << TAB << "const os = new " << IDL_NAMESPACE_STR << "Stream." << IDL_TYPE << "OutputStream();" << endl;
    str << TAB << "os.writeInt32(0, _ret);" << endl << endl;
    str << TAB << "current.doResponse(os.getBinBuffer());" << endl;
    DEL_TAB;
    str << TAB << "}" << endl << endl;
    str << TAB << "return " << IDL_NAMESPACE_STR << "Rpc.error.SUCCESS;" << endl;
    DEL_TAB;
    str << TAB << "}" << endl << endl;

    // generate functions
    for (size_t i = 0; i < vOperation.size(); i++)
    {
        const OperationPtr &oPtr = vOperation[i];

        // generate function definition
        str << TAB << oPtr->getId() << "(current: " << pPtr->getId() << "Imp." << oPtr->getId() << "Current";

        vector<ParamDeclPtr> &vParamDecl = oPtr->getAllParamDeclPtr();
        for (size_t j = 0; j < vParamDecl.size(); j++)
        {
            str << ", " << vParamDecl[j]->getTypeIdPtr()->getId() << ": " << getTsType(vParamDecl[j]->getTypeIdPtr()->getTypePtr());
        }
        str << "): any { " << endl;

        INC_TAB;
        str << TAB << "assert.fail(\"" << oPtr->getId() << " function not implemented\");" << endl;
        DEL_TAB;
        str << TAB << "}" << endl << endl;

        // generate encoder and decoder
        str << generateTSServerAsync(nPtr, pPtr, vOperation[i]) << endl;
        str << generateTSServerDispatch(nPtr, pPtr, vOperation[i]) << endl;
    }

    DEL_TAB;
    str << TAB << "}" << endl << endl;    // end of class

    // generate additional namespaces
    str << TAB << "export namespace " << pPtr->getId() << "Imp {" << endl;
    INC_TAB;

    for (size_t i = 0; i < vOperation.size(); i++)
    {
        OperationPtr &oPtr = vOperation[i];

        str << TAB << "export interface " << oPtr->getId() << "Current extends " << IDL_NAMESPACE_STR << "Rpc." << IDL_TYPE << "Current {" <<endl;
        INC_TAB;
        str << TAB;
        if (oPtr->getReturnPtr()->getTypePtr())
        {
            str << "sendResponse(ret: " << getTsType(oPtr->getReturnPtr()->getTypePtr());

            vector<ParamDeclPtr> &vParamDecl = oPtr->getAllParamDeclPtr();
            for (size_t j = 0; j < vParamDecl.size(); j++)
            {
                if(!vParamDecl[j]->isOut()) {
                    continue;
                }
                str << ", " << vParamDecl[j]->getTypeIdPtr()->getId() << ": " << getTsType(vParamDecl[j]->getTypeIdPtr()->getTypePtr()) ;
            }
            str << "): void;" << endl;
        }
        else
        {
            str << "sendResponse(): void;" << endl;
        }

        DEL_TAB;
        str << TAB << "}" << endl;
    }

    DEL_TAB;
    str << TAB << "}" << endl;

    return str.str();
}

bool CodeGenerator::generateTSServer(const ContextPtr &pPtr)
{
    vector<NamespacePtr> namespaces = pPtr->getNamespaces();

    // generate server classes with encoders and decoders
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
        kstr << generateTSServer(namespaces[i], bNeedStream, bNeedRpc, bNeedAssert);
        DEL_TAB;

        estr << generateTS(namespaces[i], kstr.str());
    }
    if (estr.str().empty())
    {
        return false;
    }

    ostringstream str;

    // generate the source file
    str << printHeaderRemark("Server");
    str << DISABLE_TSLINT << endl;
    str << DISABLE_ESLINT << endl;
    str << endl;
    str << "/// <reference types=\"node\" />" << endl;
    if (bNeedAssert)
    {
        str << TAB << "import assert = require(\"assert\");" << endl;
    }
    if (bNeedStream)
    {
        str << "import * as " << IDL_NAMESPACE_STR << "Stream from \"" << _sStreamPath << "\";" << endl;
    }
    if (bNeedRpc)
    {
        str << "import * as " << IDL_NAMESPACE_STR << "Rpc from \"" << _sRpcPath << "\";" << endl;
    }

    for (map<string, ImportFile>::iterator it = _mapFiles.begin(); it != _mapFiles.end(); it++)
    {
        if (it->second.sModule.empty()) continue;

        if (estr.str().find(it->second.sModule + ".") == string::npos) continue;

        str << "import * as " << it->second.sModule << " from \"" << TC_File::excludeFileExt(it->second.sFile) << "\";" << endl;
    }
    if (bQuickFunc)
    {
        str << endl;
        str << TAB << "const _hasOwnProperty = Object.prototype.hasOwnProperty;" << endl;
    }

    str << endl << estr.str() << endl;

    string sFileName = TC_File::excludeFileExt(_sToPath + TC_File::extractFileName(pPtr->getFileName())) + ".ts";

    TC_File::makeDirRecursive(_sToPath);
    makeUTF8File(sFileName, str.str());

    return true;
}
