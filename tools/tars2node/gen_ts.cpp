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

string CodeGenerator::generateTS(const EnumPtr &pPtr, const string &sNamespace)
{
    ostringstream s;

    INC_TAB;
    s << TAB << "export enum " << pPtr->getId() << " {" << endl;

    INC_TAB;
    int nenum = -1;
    bool bDependent = false;
    vector<TypeIdPtr>& member = pPtr->getAllMemberPtr();
    for (size_t i = 0; i < member.size(); i++)
    {
        bDependent |= isDependent(sNamespace, member[i]->getId());
        if (member[i]->hasDefault())
        {
            nenum = TC_Common::strto<int>(member[i]->def());
        }
        else
        {
            nenum++;
        }
        s << TAB << member[i]->getId() << " = " << TC_Common::tostr(nenum) << ((i < member.size() - 1) ? "," : "") << endl;
    }

    DEL_TAB;
    s << TAB << "}" << endl << endl;

    s << TAB << "export namespace " << pPtr->getId() << " {" << endl;
    INC_TAB;
    s << TAB << "export const _classname = \"" << sNamespace << "." << pPtr->getId() << "\";" << endl;
    s << TAB << "export function _write(os: " << IDL_NAMESPACE_STR << "Stream." << IDL_TYPE << "OutputStream, tag: number, val: number) { return os.writeInt32(tag, val); }" << endl;
    s << TAB << "export function _read(is: " << IDL_NAMESPACE_STR << "Stream." << IDL_TYPE << "InputStream, tag: number, def?: number) { return is.readInt32(tag, true, def); }" << endl;
    DEL_TAB;
    s << TAB << "}" << endl << endl;

    DEL_TAB;

    if (!_bMinimalMembers || _bEntry || bDependent || isDependent(sNamespace, pPtr->getId())) {
        return s.str();
    } else {
        return "";
    }
}

string CodeGenerator::generateTS(const ConstPtr &pPtr, const string &sNamespace, bool &bNeedStream)
{
    if (_bMinimalMembers && !_bEntry && !isDependent(sNamespace, pPtr->getTypeIdPtr()->getId()))
    {
        return "";
    }

    ostringstream s;
    if (_bStringBinaryEncoding && GET_CONST_GRAMMAR_PTR(pPtr)->t == CONST_GRAMMAR(STRING))
    {
        bNeedStream = true;
    }

    INC_TAB;
    s << TAB << "export const " <<  pPtr->getTypeIdPtr()->getId() << ": "
        << getTsType(pPtr->getTypeIdPtr()->getTypePtr()) << " = "
        << getDefault(pPtr->getTypeIdPtr(), GET_CONST_GRAMMAR_PTR(pPtr)->v, sNamespace, false, true) << ";"
        << endl;
    DEL_TAB;
    return s.str();
}

string CodeGenerator::generateTS(const StructPtr &pPtr, const string &sNamespace, bool &bNeedAssert, bool &bQuickFunc)
{
    if (_bMinimalMembers && !_bEntry && !isDependent(sNamespace, pPtr->getId()))
    {
        return "";
    }

    string sStructName = pPtr->getId() + "$OBJ";
    vector<TypeIdPtr> &member = pPtr->getAllMemberPtr();
    INC_TAB;

    // Struct
    ostringstream s;
    s << TAB << "export class " << pPtr->getId() << " {" << endl;
    INC_TAB;

    // class members
    for (size_t i = 0; i < member.size(); i++)
    {
        s << TAB << (member[i]->getId()) << ": "
            << getTsType(member[i]->getTypePtr()) << " = "
            << getDefault(member[i], member[i]->def(), sNamespace, true, true) << ";" << endl;
    }
    if (member.size() > 0)
    {
        s << endl;
    }

    // _classname, _proto_struct_name_
    s << TAB << "protected _proto_struct_name_ = \"\";" << endl;
    s << TAB << "protected _classname = \"" << sNamespace << "." << pPtr->getId() << "\";" << endl;
    s << TAB << "protected static _classname = \"" << sNamespace << "." << pPtr->getId() << "\";" << endl;

    // _write, _read
    s << TAB << "protected static _write(os: " << IDL_NAMESPACE_STR << "Stream.TarsOutputStream, tag: number, val: any) { os.writeStruct(tag, val); }" << endl;
	s << TAB << "protected static _read(is: " << IDL_NAMESPACE_STR << "Stream.TarsInputStream, tag: number, def?: any) { return is.readStruct(tag, true, def); }" << endl;

    // _readFrom
    s << TAB << "protected static _readFrom(is: " << IDL_NAMESPACE_STR << "Stream.TarsInputStream) {" << endl;
    INC_TAB;

    s << TAB << "const tmp = new " << pPtr->getId() << ";" << endl;
    for (size_t i = 0; i < member.size(); i++)
    {
        string sFuncName = toFunctionName(member[i], "read");
		s << TAB << "tmp." << member[i]->getId() << " = is." << sFuncName << "(" << member[i]->getTag()
            << ", " << (member[i]->isRequire() ? "true" : "false") << ", ";

        if (isSimple(member[i]->getTypePtr()))
        {
            s << getDefault(member[i], member[i]->def(), sNamespace, true, true)
                << representArgument(member[i]->getTypePtr());
        }
        else
        {
            s << getDataType(member[i]->getTypePtr(), true);
        }

        s << ");" << endl;
    }
    s << TAB << "return tmp;" << endl;
    DEL_TAB;        // end of _readFrom
    s << TAB << "}" << endl << endl;

    // _writeTo
    s << TAB << "protected _writeTo(os: " << IDL_NAMESPACE_STR << "Stream.TarsOutputStream) {" << endl;
    INC_TAB;
    for (size_t i = 0; i < member.size(); i++)
    {
        string sFuncName = toFunctionName(member[i], "write");

       	s << TAB << "os." << sFuncName << "(" << member[i]->getTag() << ", this." << member[i]->getId()
            << representArgument(member[i]->getTypePtr()) << ");" << endl;
    }
    DEL_TAB;        // end of _writeTo
    s << TAB << "}" << endl;

    /*
     *  Size Optimize:
     *    Remove <mutil_map> support.
     *    Remove toBinBuffer, readFromObject, toObject, new, create members.
     */
    if (_iOptimizeLevel != Os)
    {
        s << endl;

        // _equal
        vector<string> key = pPtr->getKey();

        s << TAB << "protected _equal(" << (key.size() > 0 ? "anItem: any" : "") << ")" << (key.size() > 0 ? ": boolean" : "") << " {" << endl;
        INC_TAB;

        if (key.size() > 0)
        {
            s << TAB << "return ";

            for (size_t i = 0; i < key.size(); i++)
            {
                for (size_t ii = 0; ii < member.size(); ii++)
                {
                    if (key[i] != member[ii]->getId())
                    {
                        continue;
                    }

                    if (isSimple(member[i]->getTypePtr()))
                    {
                        s << (i==0?"":TAB + TAB) << "this." << key[i] << " === " << "anItem." << key[i];
                    }
                    else
                    {
                        s << (i==0?"":TAB + TAB) << "this._equal(" << "anItem)";
                    }
                }

                if (i != key.size() - 1)
                {
                    s << " && " << endl;
                }
            }

            s << ";" << endl;
        }
        else
        {
            bNeedAssert = true;
            s << TAB << "assert.fail(\"this structure not define key operation\");" << endl;
        }

        DEL_TAB;
        s << TAB << "}" << endl << endl;

        // _genKey
        s << TAB << "protected _genKey() {" << endl;
        INC_TAB;
        s << TAB << "if (!this._proto_struct_name_) {" << endl;
        INC_TAB;
        s << TAB << "this._proto_struct_name_ = \"STRUCT\" + Math.random();" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;
        s << TAB << "return this._proto_struct_name_;" << endl;
        DEL_TAB;
        s << TAB << "}" << endl << endl;

        // toObject
        s << TAB << "toObject(): " << pPtr->getId() << ".Object { "<< endl;
        INC_TAB;
        s << TAB << "return {" << endl;

        for (size_t i = 0; i < member.size(); i++)
        {
            INC_TAB;
            if (i > 0 && i < member.size()) {
                s << "," << endl;
            }

            if (isSimple(member[i]->getTypePtr())) {
                s << TAB << member[i]->getId() << ": this." << member[i]->getId();
            }
            else {
                s << TAB << member[i]->getId() << ": this." << member[i]->getId() << ".toObject()";
            }
            DEL_TAB;
        }

        s << endl;
        s << TAB << "};" << endl;
        DEL_TAB;
        s << TAB << "}" << endl << endl;

        // readFromObject
        s << TAB << "readFromObject(json: " << pPtr->getId() << ".Object) { "<< endl;
        INC_TAB;

        for (size_t i = 0; i < member.size(); i++)
        {
            if (isSimple(member[i]->getTypePtr())) {
                s << TAB << "_hasOwnProperty.call(json, \"" << member[i]->getId() << "\") && (this." << member[i]->getId() << " = json." << member[i]->getId() << (member[i]->isRequire() ? "" : "!") << ");" << endl;
            } else {
                s << TAB << "_hasOwnProperty.call(json, \"" << member[i]->getId() << "\") && (this." << member[i]->getId() << ".readFromObject(json." << member[i]->getId() << (member[i]->isRequire() ? "" : "!") << "));" << endl;
            }
            bQuickFunc = true;
        }

        s << TAB << "return this;" << endl;

        DEL_TAB;
        s << TAB << "}" << endl << endl;

        // toBinBuffer
        s << TAB << "toBinBuffer() {" << endl;
        INC_TAB;
        s << TAB << "const os = new " << IDL_NAMESPACE_STR << "Stream." << IDL_TYPE << "OutputStream();" << endl;
        s << TAB << "this._writeTo(os);" << endl;
        s << TAB << "return os.getBinBuffer();" << endl;
        DEL_TAB;
        s << TAB << "}" << endl << endl;

        // new
        s << TAB << "static new() {" << endl;
        INC_TAB;
        s << TAB << "return new " << pPtr->getId() << "();" << endl;
        DEL_TAB;
        s << TAB << "}" << endl << endl;

        // create
        s << TAB << "static create(is: " << IDL_NAMESPACE_STR << "Stream.TarsInputStream) {" << endl;
        INC_TAB;
        s << TAB << "return " << sNamespace << "." << pPtr->getId() << "._readFrom(is);" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;
    }

    DEL_TAB;        // end of Struct
    s << TAB << "}" << endl << endl;

    // Additional namespace
    s << TAB << "export namespace " << pPtr->getId() << " {" << endl;
    INC_TAB;
    s << TAB << "export interface Object {" << endl;
    INC_TAB;
    for (size_t i = 0; i < member.size(); i++)
    {
        const string &sType = getTsType(member[i]->getTypePtr(), false);
        s << TAB << (member[i]->getId()) << (member[i]->isRequire() ? ": " : "?: ") << (!sType.empty() ? sType : "never") << ";" << endl;
    }
    DEL_TAB;
    s << TAB << "}" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;

    DEL_TAB;
    return s.str();
}

string CodeGenerator::generateTS(const NamespacePtr &pPtr, bool &bNeedStream, bool &bNeedAssert, bool &bQuickFunc)
{
    // struct
    ostringstream sstr;
    vector<StructPtr> ss(pPtr->getAllStructPtr());
    for (size_t last = 0; last != ss.size() && ss.size() != 0;)
    {
        last = ss.size();
        for (vector<StructPtr>::iterator iter=ss.begin(); iter!=ss.end();)
        {
            string str = generateTS(*iter, pPtr->getId(), bNeedAssert, bQuickFunc);
            if (!str.empty())
            {
                sstr << str << endl;
                iter = ss.erase(iter);
            }
            else
            {
                iter++;
            }
        }
    }

    // const
    ostringstream cstr;
    vector<ConstPtr> &cs = pPtr->getAllConstPtr();
    for (size_t i = 0; i < cs.size(); i++)
    {
        cstr << generateTS(cs[i], pPtr->getId(), bNeedStream);
    }

    // enum
    ostringstream estr;
    vector<EnumPtr> &es = pPtr->getAllEnumPtr();
    for (size_t i = 0; i < es.size(); i++)
    {
        estr << generateTS(es[i], pPtr->getId());
    }

    ostringstream kstr;
    if (!estr.str().empty())
    {
        bNeedStream = true;
        kstr << estr.str() << endl;
    }
    if (!cstr.str().empty()) kstr << cstr.str() << endl;
    if (!sstr.str().empty())
    {
        bNeedStream = true;
        kstr << sstr.str();
    }

    return kstr.str();
}

string CodeGenerator::generateTS(const NamespacePtr &pPtr, const string &sContent)
{
    ostringstream str;
    if (!sContent.empty())
    {
        str << "export namespace " << pPtr->getId() << " {" << endl;
        str << sContent;
        str << "}" << endl << endl;
    }
    return str.str();
}

void CodeGenerator::generateTS(const ContextPtr &pPtr)
{
    vector<NamespacePtr> namespaces = pPtr->getNamespaces();

    // generate encoders and decoders
    ostringstream estr;
    bool bNeedAssert = false;
    bool bNeedStream = false;
    bool bQuickFunc = false;
    for (size_t i = 0; i < namespaces.size(); i++)
    {
        estr << generateTS(namespaces[i], generateTS(namespaces[i], bNeedStream, bNeedAssert, bQuickFunc));
    }
    if (estr.str().empty())
    {
        return;
    }

    // generate module imports
    ostringstream ostr;
    if (bNeedAssert)
    {
        ostr << "import assert = require(\"assert\");" << endl;
    }
    if (bNeedStream)
    {
        ostr << "import * as " << IDL_NAMESPACE_STR << "Stream from \"" << _sStreamPath << "\";" << endl;
    }

    for (map<string, ImportFile>::iterator it = _mapFiles.begin(); it != _mapFiles.end(); it++)
    {
        if (it->second.sModule.empty()) continue;

        if (estr.str().find(it->second.sModule + ".") == string::npos) continue;

        ostr << "import * as " << it->second.sModule << " from \"" << TC_File::excludeFileExt(it->second.sFile) << "\";" << endl;
    }

    if (bQuickFunc)
    {
        ostr << endl;
        ostr << "const _hasOwnProperty = Object.prototype.hasOwnProperty;" << endl;
    }

    // concat generated code
    ostringstream sstr;
    sstr << printHeaderRemark("Structure");
    sstr << DISABLE_TSLINT << endl;
    sstr << DISABLE_ESLINT << endl;
    sstr << endl;
    sstr << "/// <reference types=\"node\" />" << endl;
    sstr << ostr.str() << endl;
    sstr << estr.str() << endl;

    string sFileName = TC_File::excludeFileExt(_sToPath + TC_File::extractFileName(pPtr->getFileName())) + IDL_TYPE + ".ts";

    TC_File::makeDirRecursive(_sToPath);
    makeUTF8File(sFileName, sstr.str());
}
