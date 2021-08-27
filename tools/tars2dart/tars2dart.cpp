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
#include "tars2dart.h"
#include "interface_analysis.h"
#include "tars_filter.h"
#include "util/tc_md5.h"
#include "util/tc_file.h"
#include "util/tc_common.h"
#include <string>

#define TAB     g_parse->getTab()
#define INC_TAB g_parse->incTab()
#define DEL_TAB g_parse->delTab()

static string g_default_package = "tars_idl";
static string g_default_tars_package = "/tars/codec/";

//////////////////////////////////////////////////////////////////////////////////
//
Tars2Dart::Tars2Dart()
{
    _bWithCompact = false;
    s_TARS_PACKAGE       = g_default_package + TARS_PACKAGE;
    s_PROXY_PACKAGE     = g_default_package + PROXY_PACKAGE;
    s_WUP_PACKAGE       = g_default_package + WUP_PACKAGE;
    _tarsPackage        = g_default_tars_package;
}

string Tars2Dart::writeTo(const TypeIdPtr& pPtr) const
{
    ostringstream s;
    if (EnumPtr::dynamicCast(pPtr->getTypePtr()))
    {
        s << TAB << "_os.write(" << pPtr->getId() << ", " << pPtr->getTag() << ");" << endl;
    }
    else
    {
        s << TAB << "_os.write(" << pPtr->getId() << ", " << pPtr->getTag() << ");" << endl;
    }

    return s.str();
}

string Tars2Dart::readFrom(const TypeIdPtr& pPtr) const
{
    ostringstream s;
    if (EnumPtr::dynamicCast(pPtr->getTypePtr()))
    {
        s << TAB << "_is.read((tars::Int32&)" << pPtr->getId();
    }
    else
    {
        s << TAB << "_is.read(" << pPtr->getId();
    }

    s << ", " << pPtr->getTag() << ", " << (pPtr->isRequire() ? "true" : "false") << ");" << endl;

    return s.str();
}

string Tars2Dart::display(const TypeIdPtr& pPtr) const
{
    ostringstream s;
    if (EnumPtr::dynamicCast(pPtr->getTypePtr()))
    {
        s << TAB << "_ds.display((tars::Int32)" << pPtr->getId() << ",\"" << pPtr->getId() << "\");" << endl;;
    }
    else
    {
        s << TAB << "_ds.display(" << pPtr->getId() << ",\"" << pPtr->getId() << "\");" << endl;;
    }

    return s.str();
}

string Tars2Dart::toTypeInit(const TypePtr& pPtr) const
{
    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr);
    if (bPtr)
    {
        switch (bPtr->kind())
        {
        case Builtin::KindBool:
            return "false";
        case Builtin::KindByte:
            return "0";
        case Builtin::KindShort:
            return "0";
        case Builtin::KindInt:
            return "0";
        case Builtin::KindLong:
            return "0";
        case Builtin::KindFloat:
            return "0.0f";
        case Builtin::KindDouble:
            return "0.0";
        case Builtin::KindString:
            return "\"\"";
        default:
            return "";
        }
    }

    VectorPtr vPtr = VectorPtr::dynamicCast(pPtr);
    if (vPtr)
    {
        // BuiltinPtr bPtr = BuiltinPtr::dynamicCast(vPtr->getTypePtr());
        // if (bPtr && bPtr->kind() == Builtin::KindByte)
        // {
        //     return "Uint8List(1);";
        // }

        // return "" + tostrVector(vPtr) + "();";
        return "null";
    }

    MapPtr mPtr = MapPtr::dynamicCast(pPtr);
    // if (mPtr) return " " + tostrMap(mPtr, true) + "();";
    if (mPtr) return "null";

    StructPtr sPtr = StructPtr::dynamicCast(pPtr);
    // if (sPtr) return " " + tostrStruct(sPtr) + "();";
    if (sPtr) return "null";

    EnumPtr ePtr = EnumPtr::dynamicCast(pPtr);
    if (ePtr) return "0";

    return "";
}


string Tars2Dart::toObjStr(const TypePtr& pPtr) const
{
    string sType = tostr(pPtr);

    if (sType == "boolean") return "bool";
    if (sType == "byte")    return "Uint8List";
    if (sType == "short")   return "int";
    if (sType == "int") return "int";
    if (sType == "long")    return "int";
    if (sType == "float")   return "double";
    if (sType == "double")  return "double";

    return sType;
}

bool Tars2Dart::isObjType(const TypePtr& pPtr) const
{
    EnumPtr ePtr = EnumPtr::dynamicCast(pPtr);
    if (ePtr)
    {
        return false;
    }

    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr);
    if (bPtr)
    {
        switch (bPtr->kind())
        {
        case Builtin::KindBool:
            return false;
        case Builtin::KindByte:
            return false;
        case Builtin::KindShort:
            return false;
        case Builtin::KindInt:
            return false;
        case Builtin::KindLong:
            return false;
        case Builtin::KindFloat:
            return false;
        case Builtin::KindDouble:
            return false;
        default:
            return true;
        }
    }

    return true;
}

string Tars2Dart::tostr(const TypePtr& pPtr) const
{
    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr);
    if (bPtr) return tostrBuiltin(bPtr);

    VectorPtr vPtr = VectorPtr::dynamicCast(pPtr);
    if (vPtr) return tostrVector(vPtr);

    MapPtr mPtr = MapPtr::dynamicCast(pPtr);
    if (mPtr) return tostrMap(mPtr);

    StructPtr sPtr = StructPtr::dynamicCast(pPtr);
    if (sPtr) return tostrStruct(sPtr);

    EnumPtr ePtr = EnumPtr::dynamicCast(pPtr);
    if (ePtr) return tostrEnum(ePtr);

    if (!pPtr) return "void";

    assert(false);
    return "";
}

vector<string> Tars2Dart::toImportStrs(const TypePtr& pPtr) const{
    vector<string> result;
    itrToImportStrs(pPtr, result);
    return result;
}

void Tars2Dart::itrToImportStrs(const TypePtr& pPtr, vector<string>& result) const{
    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr);
    if (bPtr) return;

    VectorPtr vPtr = VectorPtr::dynamicCast(pPtr);
    if (vPtr){
        itrToImportStrs(vPtr->getTypePtr(), result);
        return;
    }

    MapPtr mPtr = MapPtr::dynamicCast(pPtr);
    if (mPtr) {
        itrToImportStrs(mPtr->getLeftTypePtr(), result);
        itrToImportStrs(mPtr->getRightTypePtr(), result);
        return;
    }

    StructPtr sPtr = StructPtr::dynamicCast(pPtr);
    if (sPtr) { result.push_back(sPtr->getId()); return; } 

    EnumPtr ePtr = EnumPtr::dynamicCast(pPtr);
    if (ePtr){ result.push_back(ePtr->getId()); return; }

    return;
}


/*******************************BuiltinPtr********************************/
string Tars2Dart::tostrBuiltin(const BuiltinPtr& pPtr) const
{
    string s;

    switch (pPtr->kind())
    {
    case Builtin::KindBool:
        s = "bool";
        break;
    case Builtin::KindByte:
        s = "int";
        break;
    case Builtin::KindShort:
        s = "int";
        break;
    case Builtin::KindInt:
        s = "int";
        break;
    case Builtin::KindLong:
        s = "int";
        break;
    case Builtin::KindFloat:
        s = "double";
        break;
    case Builtin::KindDouble:
        s = "double";
        break;
    case Builtin::KindString:
        s = "String";
        break;
    case Builtin::KindVector:
        s = "List";
        break;
    case Builtin::KindMap:
        s = "Map";
        break;
    default:
        assert(false);
        break;
    }

    return s;
}
/*******************************VectorPtr********************************/
string Tars2Dart::tostrVector(const VectorPtr& pPtr) const
{
    string s;
    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr->getTypePtr());
    if (bPtr && bPtr->kind() == Builtin::KindByte)
    {
        s = "Uint8List";
    }
    else
    {
        s = "List" + string("<") + toObjStr(pPtr->getTypePtr()) + ">";
    }
    return s;
}

/*******************************MapPtr********************************/
string Tars2Dart::tostrMap(const MapPtr& pPtr, bool bNew) const
{
    string s;
    s = "Map";
    s += string("<") + toObjStr(pPtr->getLeftTypePtr()) + ", " + toObjStr(pPtr->getRightTypePtr()) + ">";

    return s;
}

string Tars2Dart::tostrStruct(const StructPtr& pPtr) const
{
    // return _packagePrefix + tars::TC_Common::replace(pPtr->getSid(), "::", ".");
    return pPtr->getId();
}

string Tars2Dart::tostrEnum(const EnumPtr& pPtr) const
{
    return "int";
}

/////////////////////////////////////////////////////////////////////

string Tars2Dart::MD5(const StructPtr& pPtr) const
{
    string s;
    vector<TypeIdPtr>& member = pPtr->getAllMemberPtr();
    for (size_t j = 0; j < member.size(); j++)
    {
        s += "_" + tostr(member[j]->getTypePtr());
    }

    return "\"" + tars::TC_MD5::md5str(s) + "\"";
}

string Tars2Dart::generateDart(const ParamDeclPtr& pPtr) const
{
    ostringstream s;

    TypePtr typePtr = pPtr->getTypeIdPtr()->getTypePtr();
    if (!pPtr->isOut())
    {
        s << tostr(typePtr);
    }

    s << " " << pPtr->getTypeIdPtr()->getId();

    return s.str();
}

struct SortOperation                                                                                                                          
{
    bool operator ()(const OperationPtr& o1, const OperationPtr& o2)
    {
        return o1->getId() < o2->getId();
    }
};

string Tars2Dart::generateDart(const InterfacePtr& pPtr, const NamespacePtr& nPtr) const
{
    ostringstream s;
    //generateAndroidStub(pPtr, nPtr);
    return s.str();
}

string Tars2Dart::generateDefautElem(const TypePtr& pPtr, const string& sElemName) const
{
    static int iCount = 0;
    ostringstream s;
    return s.str();
}

string Tars2Dart::getDefaultValue(const TypeIdPtr& pPtr, const string sp) const{
    string sDefalut;
    if (pPtr -> hasDefault()){
        BuiltinPtr bPtr  = BuiltinPtr::dynamicCast(pPtr->getTypePtr());
        EnumPtr    ePtr  = EnumPtr::dynamicCast(pPtr->getTypePtr());
        //string转义
        if (bPtr && bPtr->kind() == Builtin::KindString)
        {
            sDefalut = tars::TC_Common::replace(pPtr->def(), "\"", "\\\"");
            //sDefalut = " = (" + tostr(member[i]->getTypePtr()) + ")\"" + sDefalut + "\"";
            sDefalut =  " " + sp +" \"" + sDefalut + "\"";
        }
        else if (ePtr)
        {
            std::string sdef = pPtr->def();
            std::string::size_type pos = sdef.rfind("::");
            if (pos != std::string::npos && pos + 2 < sdef.size())
            {
                sdef = sdef.substr(pos + 2);
            }

            if (_bEnumCompact)
            {
                sDefalut =  " " + sp + " " + _packagePrefix + tars::TC_Common::replace(ePtr->getSid(), "::", ".") + "._" + sdef;
            }
            else
            {
                sDefalut = " " + sp + " " + _packagePrefix + tars::TC_Common::replace(ePtr->getSid(), "::", ".") + "." + sdef + ".value()";
            }
        }
        else
        {
            //sDefalut = " = (" + tostr(member[i]->getTypePtr()) + ")" + member[i]->def();
            sDefalut = " " + sp + " " + pPtr->def();
        }
    }else{
        string typeDefault = toTypeInit(pPtr->getTypePtr());
        if(typeDefault == "" || typeDefault == "null"){
            sDefalut = "";
        }else{
            sDefalut = " " + sp + " " + toTypeInit(pPtr->getTypePtr());
        }
    }
    return sDefalut;
}

string Tars2Dart::generateNewElem(const TypePtr& pPtr) const
{
    ostringstream s;

    VectorPtr vPtr = VectorPtr::dynamicCast(pPtr);
    if (vPtr)
    {
        BuiltinPtr bPtr = BuiltinPtr::dynamicCast(vPtr->getTypePtr());
        if (bPtr && bPtr->kind() == Builtin::KindByte)
        {
            return "[0x0]";
        }

        return "["+ generateNewElem(vPtr->getTypePtr()) +"]";
    }

    MapPtr mPtr = MapPtr::dynamicCast(pPtr);
    if (mPtr)
    {
        s <<  " " << tostrMap(mPtr, true) << "()" << endl;
        return s.str();
    }

    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr);
    if (bPtr)
    {
        switch (bPtr->kind())
        {
        case Builtin::KindBool:
            return "false";
        case Builtin::KindByte:
            return "0";
        case Builtin::KindShort:
            return "0";
        case Builtin::KindInt:
            return "0";
        case Builtin::KindLong:
            return "0";
        case Builtin::KindFloat:
            return "0.0f";
        case Builtin::KindDouble:
            return "0.0";
        case Builtin::KindString:
            return "\"\"";
        default:
            return "";
        }
    }

    StructPtr sPtr = StructPtr::dynamicCast(pPtr);
    if (sPtr) return " " + tostrStruct(sPtr) + "()";

    EnumPtr ePtr = EnumPtr::dynamicCast(pPtr);
    if (ePtr) return "0";

    return "";
}

string Tars2Dart::tostrCache(const TypeIdPtr& pPtr) const
{
    string prefix = "cache";
    string cacheName =  prefix +"_" + pPtr->getId();

    static int iCount = 0;
    string cacheCodeStr = "";

    VectorPtr vPtr = VectorPtr::dynamicCast(pPtr->getTypePtr());
    if (vPtr)
    {
        int iId = iCount;
        BuiltinPtr bPtr = BuiltinPtr::dynamicCast(vPtr->getTypePtr());
        if (bPtr && bPtr->kind() == Builtin::KindByte)
        {
            cacheCodeStr = "static Uint8List " + cacheName+ " = Uint8List.fromList([0x0])";
        }else{
            cacheCodeStr = "static List<" + tostr(vPtr->getTypePtr()) + "> " + cacheName + " = ["+ generateNewElem(vPtr->getTypePtr()) +"]";
        }
    }

    MapPtr mPtr = MapPtr::dynamicCast(pPtr->getTypePtr());
    if (mPtr)
    {
        string leftType = tostr(mPtr->getLeftTypePtr());
        string rightType = tostr(mPtr->getRightTypePtr());

        cacheCodeStr = "static Map<"+ leftType +", "+ rightType +"> " + cacheName
        + " = {"+ generateNewElem(mPtr->getLeftTypePtr()) +":"+ generateNewElem(mPtr->getRightTypePtr()) + "}";
    }
  
    StructPtr sPtr = StructPtr::dynamicCast(pPtr->getTypePtr());
    if (sPtr)
    {
        cacheCodeStr = "static "+ toObjStr(sPtr) +" " + cacheName+ " = "+ generateNewElem(sPtr) ;
    }

    EnumPtr ePtr = EnumPtr::dynamicCast(pPtr->getTypePtr());
    if (ePtr)
    {
        cacheCodeStr = "static "+ toObjStr(ePtr) +" " + cacheName+ " = "+ generateNewElem(ePtr) ;
    }

    return cacheCodeStr;
}

/******************************StructPtr***************************************/
string Tars2Dart::generateDart(const StructPtr& pPtr, const NamespacePtr& nPtr) const
{
    ostringstream s;
    s << g_parse->printHeaderRemark();

    vector<string> key = pPtr->getKey();
    vector<TypeIdPtr>& member = pPtr->getAllMemberPtr();

    // s << TAB << "package " << _packagePrefix << nPtr->getId() << ";" << endl;
    // s << endl;

    s << TAB << "import 'dart:core';"<< endl;
    s << TAB << "import 'dart:typed_data';"<< endl;
    s << TAB << "import '" << _tarsPackage << "tars_input_stream.dart';"<< endl;
    s << TAB << "import '" << _tarsPackage << "tars_output_stream.dart';"<< endl;
    s << TAB << "import '" << _tarsPackage << "tars_struct.dart';"<< endl;
    s << TAB << "import '" << _tarsPackage << "tars_displayer.dart';"<< endl;
    s << TAB << "import '" << _tarsPackage << "tars_deep_copyable.dart';"<< endl;
    s << endl;
    

    //导入tars定义的结构体
    map<string , bool> mapImport;
    for (size_t i = 0; i < member.size(); i++)
    {
         vector<string> packages = toImportStrs(member[i]->getTypePtr());
         if(!packages.empty()){
            for (size_t j = 0; j < packages.size(); j++){
                if(mapImport.count(packages[j])){
                    continue;
                }
                mapImport.insert(pair<string, bool>(packages[j] , true));  
                s << TAB << "import '" << packages[j] << ".dart';"<< endl;
            }
         }
    }
  
    s << endl;

    bool bHasImpPrefix = false;

    //class定义部分
    s << TAB << "class " << pPtr->getId() << " extends " <<  "TarsStruct";

    s << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    if (!_bWithCompact)
    {
        //生成结构名称
        s << TAB << "String className()" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB << "return " << "\"" << pPtr->getId() << "\"" << ";" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;
        s << endl;

    }


    //定义成员变量
    for (size_t i = 0; i < member.size(); i++)
    {
        string sDefalut;
        if (member[i]->hasDefault())
        {
            BuiltinPtr bPtr  = BuiltinPtr::dynamicCast(member[i]->getTypePtr());
            EnumPtr    ePtr  = EnumPtr::dynamicCast(member[i]->getTypePtr());
            s << TAB << tostr(member[i]->getTypePtr()) << ((bPtr && bPtr->isSimple())? " " :"? ") << member[i]->getId() << getDefaultValue(member[i],"=") << ";" << endl;
        }
        else
        {
            //s << TAB << "public " << tostr(member[i]->getTypePtr()) << " "<< member[i]->getId() << sDefalut << endl;
            BuiltinPtr bPtr  = BuiltinPtr::dynamicCast(member[i]->getTypePtr());
            EnumPtr ePtr = EnumPtr::dynamicCast(member[i]->getTypePtr());
            if ((!bPtr && !ePtr) || (bPtr && bPtr->kind() == Builtin::KindString))
            {
                s << TAB  << tostr(member[i]->getTypePtr()) << ((bPtr && bPtr->isSimple())? " " :"? ") << member[i]->getId() << ";" << endl;
            }
            else
            {
                s << TAB  << tostr(member[i]->getTypePtr()) << ((bPtr && bPtr->isSimple())? " " :"? ") << member[i]->getId() << getDefaultValue(member[i],"=")  << ";" << endl;
            }
        }
        s << endl;
    }
 

    //带参数的构造函数，格式为： TestData({int id : 0, String? code : ""}){...}
    //(constructor)(...)
    s << TAB << "" << pPtr->getId() << "({";
    for (size_t i = 0; i < member.size(); i++)
    {
        BuiltinPtr bPtr  = BuiltinPtr::dynamicCast(member[i]->getTypePtr());
        s << tostr(member[i]->getTypePtr()) << ((bPtr && bPtr->isSimple())? " " :"? ") << member[i]->getId()
            << getDefaultValue(member[i],":")
            << ((i < member.size() - 1) ? ", " : "");
    }
    s << "})" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    for (size_t i = 0; i < member.size(); i++)
    {
        s << TAB << "this." << member[i]->getId() << " = " << member[i]->getId() << ";" << endl;
    }
    DEL_TAB;
    s << TAB << "}" << endl;
    s << endl;
 

    //writeTo()
    s << TAB << "@override"  << endl;
    s << TAB << "void writeTo(" << "TarsOutputStream _os)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    for (size_t i = 0; i < member.size(); i++)
    {
        BuiltinPtr bPtr = BuiltinPtr::dynamicCast(member[i]->getTypePtr());
        EnumPtr ePtr    = EnumPtr::dynamicCast(member[i]->getTypePtr());
        if (!member[i]->isRequire() && ((!bPtr && !ePtr) || (bPtr && bPtr->kind() == Builtin::KindString)))
        {
            s << TAB << "if (null != " << member[i]->getId() << ")" << endl;
            s << TAB << "{" << endl;
            INC_TAB;
            s << TAB << "_os.write(" << member[i]->getId() << ", " << member[i]->getTag() << ");" << endl;
            DEL_TAB;
            s << TAB << "}" << endl;
        }
        else if (EnumPtr::dynamicCast(member[i]->getTypePtr()))
        {
            s << TAB << "_os.write(" << member[i]->getId() << ", " << member[i]->getTag() << ");" << endl;
        }
        else
        {
            s << TAB << "_os.write(" << member[i]->getId() << ", " << member[i]->getTag() << ");" << endl;
        }
    }
    DEL_TAB;
    s << TAB << "}" << endl;
    s << endl;


    //生成cache
    for (size_t i = 0; i < member.size(); i++)
    {
        BuiltinPtr bPtr  = BuiltinPtr::dynamicCast(member[i]->getTypePtr());
        if (!bPtr)
        {
            s << TAB << tostrCache(member[i]) << ";" << endl;
        }
    }
    s << endl;



    //readFrom()
    s << TAB << "@override"  << endl;
    s << TAB << "void readFrom(" <<  "TarsInputStream _is)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    for (size_t i = 0; i < member.size(); i++)
    {
        string prefix = "";
        TypePtr& tPtr = member[i]->getTypePtr();
        BuiltinPtr bPtr  = BuiltinPtr::dynamicCast(tPtr);
        EnumPtr ePtr = EnumPtr::dynamicCast(tPtr);
        VectorPtr vPtr = VectorPtr::dynamicCast(tPtr);
        MapPtr mPtr = MapPtr::dynamicCast(tPtr);
        StructPtr sPtr = StructPtr::dynamicCast(tPtr);

        if (!bPtr && !ePtr)
        {
            prefix = "cache_";
        }

        //基础类型
        if (bPtr){
            s << TAB << "this." << member[i]->getId() << " = " 
                << " _is.read<" << toObjStr(member[i]->getTypePtr()) << ">(" << prefix + member[i]->getId()
                << ", " << member[i]->getTag() << ", " << (member[i]->isRequire() ? "true" : "false") << ")" << ";" << endl;
        }

        //数组
        if (vPtr)
        {
            s << TAB << "this." << member[i]->getId() << " = "
                << " _is.read<" << toObjStr(vPtr->getTypePtr()) << ">(" << prefix + member[i]->getId()
                    << ", "  << member[i]->getTag() << ", " << (member[i]->isRequire() ? "true" : "false") << ");" << endl;
        }

        //map
        if (mPtr)
        {
            VectorPtr svPtr = VectorPtr::dynamicCast(mPtr->getRightTypePtr());
            MapPtr smPtr = MapPtr::dynamicCast(mPtr->getRightTypePtr());
            if (svPtr)
            {
                //Map<K,List<V>> 结构
                s << TAB << "this." << member[i]->getId() << " = "
                << " _is.readMapList<" << toObjStr(mPtr->getLeftTypePtr()) << "," << toObjStr(svPtr->getTypePtr()) << ">(" << prefix + member[i]->getId()
                    << ", " << member[i]->getTag() << ", " << (member[i]->isRequire() ? "true" : "false") << ");" << endl;
            }else if (smPtr)
            {   
                //Map<K,Map<K2,V2>> 结构
                s << TAB << "this." << member[i]->getId() << " = "
                << " _is.readMapMap<" << toObjStr(mPtr->getLeftTypePtr()) << "," << toObjStr(smPtr->getLeftTypePtr()) << "," << toObjStr(smPtr->getRightTypePtr()) << ">(" << prefix + member[i]->getId()
                    << ", " << member[i]->getTag() << ", " << (member[i]->isRequire() ? "true" : "false") << ");" << endl;
            }else{
                //普通 Map<K,V> 结构
                s << TAB << "this." << member[i]->getId() << " = "
                << " _is.readMap<" << toObjStr(mPtr->getLeftTypePtr()) << "," << toObjStr(mPtr->getRightTypePtr()) << ">(" << prefix + member[i]->getId()
                    << ", " << member[i]->getTag() << ", " << (member[i]->isRequire() ? "true" : "false") << ");" << endl;
            }
        }

        //自定义结构体
        if (sPtr)
        {
            s << TAB << "this." << member[i]->getId() << " = "
                << " _is.read<" << toObjStr(member[i]->getTypePtr()) << ">(" << prefix + member[i]->getId()
                    << ", " << member[i]->getTag() << ", " << (member[i]->isRequire() ? "true" : "false") << ");" << endl;
        }

        
    }

    DEL_TAB;
    s << TAB << "}" << endl;
    s << endl;
 
    if (!_bWithCompact)
    {
        //display()
        s << TAB << "@override"  << endl;
        s << TAB << "void displayAsString(StringBuffer _os, int _level)" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB  << "TarsDisplayer _ds = "  << "TarsDisplayer(_os,  level : _level);" << endl;
        for (size_t i = 0; i < member.size(); i++)
        {
            s << TAB << "_ds.display(" << member[i]->getId()
                << ", \"" << member[i]->getId() << "\");" << endl;
        }
        DEL_TAB;
        s << TAB << "}" << endl;
        s << endl;
    }

    //deepCopy()
    s << TAB << "@override"  << endl;
    s << TAB << "Object deepCopy()" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB  << "var o = " <<  pPtr->getId() << "();" << endl;
    for (size_t i = 0; i < member.size(); i++)
    {

        string prefix = "";
        TypePtr& tPtr = member[i]->getTypePtr();
        BuiltinPtr bPtr  = BuiltinPtr::dynamicCast(tPtr);
        EnumPtr ePtr = EnumPtr::dynamicCast(tPtr);
        VectorPtr vPtr = VectorPtr::dynamicCast(tPtr);
        MapPtr mPtr = MapPtr::dynamicCast(tPtr);
        StructPtr sPtr = StructPtr::dynamicCast(tPtr);

        if (!bPtr && !ePtr)
        {
            prefix = "cache_";
        }

        //基础类型
        if (bPtr || ePtr){
            s << TAB << "o." << member[i]->getId() << " = "  << "this." << member[i]->getId()  << ";" << endl;
        }

        
        //数组
        if (vPtr)
        {
            BuiltinPtr bPtr = BuiltinPtr::dynamicCast(vPtr->getTypePtr());
            if (bPtr && bPtr->kind() == Builtin::KindByte)
            {
                s << TAB << "o." << member[i]->getId() << " = "  << "this." << member[i]->getId()  << ";" << endl;
            } else {
                s << TAB << "if (null != " << member[i]->getId() << ")" << endl;
                s << TAB << "{" << endl;
                INC_TAB;
                s << TAB << "o." << member[i]->getId() << " = "
                    << " listDeepCopy<" << toObjStr(vPtr->getTypePtr()) << ">(this." << member[i]->getId() << "!);" << endl;
                DEL_TAB;
                s << TAB << "}" << endl;
            }
        }

        //map
        if (mPtr)
        {
            s << TAB << "if (null != " << member[i]->getId() << ")" << endl;
            s << TAB << "{" << endl;
            INC_TAB;
            VectorPtr svPtr = VectorPtr::dynamicCast(mPtr->getRightTypePtr());
            MapPtr smPtr = MapPtr::dynamicCast(mPtr->getRightTypePtr());
            if (svPtr)
            {
                //Map<K,List<V>> 结构
                s << TAB << "o." << member[i]->getId() << " = "
                << " mapListDeepCopy<" << toObjStr(mPtr->getLeftTypePtr()) << "," << toObjStr(svPtr->getTypePtr()) << ">(this." << member[i]->getId() << "!);" << endl;

            }else if (smPtr)
            {   
                //Map<K,Map<K2,V2>> 结构
                s << TAB << "o." << member[i]->getId() << " = "
                << " mapMapDeepCopy<" << toObjStr(mPtr->getLeftTypePtr()) << "," << toObjStr(smPtr->getLeftTypePtr()) << "," << toObjStr(smPtr->getRightTypePtr()) << "!);" << endl;

            }else{
                //普通 Map<K,V> 结构
                s << TAB << "o." << member[i]->getId() << " = "
                << " mapDeepCopy<" << toObjStr(mPtr->getLeftTypePtr()) << "," << toObjStr(mPtr->getRightTypePtr()) << ">(this." << member[i]->getId() << "!);" << endl;
            }
            DEL_TAB;
            s << TAB << "}" << endl;
        }

        //自定义结构体
        if (sPtr)
        {
            s << TAB << "o." << member[i]->getId() << " = this." << member[i]->getId() << "?.deepCopy() as " << toObjStr(member[i]->getTypePtr()) << "?;" << endl;
        }

    }
    s << TAB  << "return o;" << endl;

    DEL_TAB;
    s << TAB << "}" << endl;
    s << endl;


    DEL_TAB;
    s << TAB << "}" << endl;
    s << endl;


    string fileJava  = getFilePath(nPtr->getId()) + pPtr->getId() + ".dart";
    tars::TC_File::makeDirRecursive(getFilePath(nPtr->getId()));
    tars::TC_File::save2file(fileJava, s.str());

    return s.str();
}

/******************************EnumPtr***************************************/
string Tars2Dart::generateDart(const EnumPtr& pPtr, const NamespacePtr& nPtr) const
{
    ostringstream s;
    s << g_parse->printHeaderRemark();

    s << TAB << "import 'dart:core';"<< endl;
    s << endl;

    s << TAB << "class " << pPtr->getId() << "" << endl;
    s << TAB << "{" << endl;
    INC_TAB;


    //成员变量
    vector<TypeIdPtr>& member = pPtr->getAllMemberPtr();
    bool bFlag = false;
    string sValue;
    for (size_t i = 0; i < member.size(); i++)
    {
        s << TAB << "static const int " << member[i]->getId() << " = ";
        if (member[i]->hasDefault())
        {
            bFlag = true;
            sValue = member[i]->def();
            s << sValue;
        }
        else
        {
            if (bFlag == true)
            {
                assert(i > 0);
                string stem = tars::TC_Common::tostr(tars::TC_Common::strto<int>(sValue) + 1);
                sValue = stem;
                s << stem;
            }
            else
            {
                s << tars::TC_Common::tostr(i);
            }
        }
        s << ";" << endl;
    }
    
    DEL_TAB;
    s << TAB << "}" << endl;

    string fileJava  = getFilePath(nPtr->getId()) + pPtr->getId() + ".dart";
    tars::TC_File::makeDirRecursive(getFilePath(nPtr->getId()));
    tars::TC_File::save2file(fileJava, s.str());


    return s.str();
}

/******************************ConstPtr***************************************/
void Tars2Dart::generateDart(const ConstPtr& pPtr, const NamespacePtr& nPtr) const
{
    return;
}

/******************************Tars2Dart***************************************/

void Tars2Dart::generateDart(const ContextPtr& pPtr) const
{
    ostringstream s;

    vector<NamespacePtr> namespaces = pPtr->getNamespaces();

    for (size_t i = 0; i < namespaces.size(); i++)
    {
        generateDart(namespaces[i]);
    }

}

void Tars2Dart::createFile(const string& file)
{
    std::vector<ContextPtr> contexts = g_parse->getContexts();
    for (size_t i = 0; i < contexts.size(); i++)
    {
        if (file == contexts[i]->getFileName())
        {
            generateDart(contexts[i]);
        }
    }
}

void Tars2Dart::setBaseDir(const string& dir)
{
    _baseDir = dir;
}


void Tars2Dart::setBasePackage(const string& prefix)
{
    _packagePrefix = prefix;
    if (_packagePrefix.length() != 0
        && _packagePrefix.substr(_packagePrefix.length() - 1, 1) != ".")
    {
        _packagePrefix += ".";
    }
}


string Tars2Dart::getFilePath(const string& ns) const
{
    return _baseDir + FILE_SEP + tars::TC_Common::replace(_packagePrefix, ".", FILE_SEP) + FILE_SEP + ns + FILE_SEP;
}

// 增加的函数
static string firstCharUpper(const string& castStr) {
    string resultStr = castStr;
    if (castStr.size() > 0) {
        resultStr[0] = toupper(castStr[0]);
    }
    return resultStr;
}

void Tars2Dart::generateDart(const NamespacePtr &pPtr) const
{
    vector<InterfacePtr>    &is    = pPtr->getAllInterfacePtr();
    vector<StructPtr>       &ss    = pPtr->getAllStructPtr();
    vector<EnumPtr>         &es    = pPtr->getAllEnumPtr();
    vector<ConstPtr>        &cs    = pPtr->getAllConstPtr();

    InterfaceAnalysis interfaceAnalysis;

    if (_bWithGenerateInterfaceDependencies) {
        interfaceAnalysis.analysis(is);
    }

    for (size_t i = 0; i < is.size(); i++)
    {
        generateDart(is[i], pPtr);
    }

    TarsFilter romTarsFilter;
    std::map<std::string, StructPtr> needGenerateStructMap;
    if (_bWithGenerateInterfaceDependencies) {
        if (_bWithFilterRomTars) {
            romTarsFilter.filterStructs(
                    interfaceAnalysis.getAllStructs(), needGenerateStructMap);
        } else {
            needGenerateStructMap = interfaceAnalysis.getAllStructs();
        }
    }

    for (size_t i = 0; i < ss.size(); i++)
    {
        generateDart(ss[i], pPtr);
        needGenerateStructMap.erase(ss[i]->getSid());
    }
    for (std::map<std::string, StructPtr>::const_iterator
            it = needGenerateStructMap.begin(); it != needGenerateStructMap.end(); ++it) {
        generateDart(it->second, pPtr);
    }

    std::map<std::string, EnumPtr> needGenerateEnumMap;
    if (_bWithGenerateInterfaceDependencies) {
        if (_bWithFilterRomTars) {
            romTarsFilter.filterEnums(
                            interfaceAnalysis.getAllEnums(), needGenerateEnumMap);
        } else {
            needGenerateEnumMap = interfaceAnalysis.getAllEnums();
        }
    }

    for (size_t i = 0; i < es.size(); i++)
    {
        generateDart(es[i], pPtr);
        needGenerateEnumMap.erase(es[i]->getSid());
    }
    for (std::map<std::string, EnumPtr>::const_iterator
                it = needGenerateEnumMap.begin(); it != needGenerateEnumMap.end(); ++it) {
        generateDart(it->second, pPtr);
    }

    for (size_t i = 0; i < cs.size(); i++)
    {
        generateDart(cs[i], pPtr);
    }

    return;
}

// 增加的实现
string Tars2Dart::generateConstructor(const TypePtr& vType) const {
    stringstream s;

    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(vType);
    if (bPtr) {
        switch (bPtr->kind()) {
        case Builtin::KindBool: {
            s << "Boolean.valueOf(false)";
        } break;
        case Builtin::KindByte: {
            s << "Byte.valueOf((byte)0)";
        } break;
        case Builtin::KindShort: {
            s << "Short.valueOf((short)0)";
        } break;
        case Builtin::KindInt: {
            s << "Integer.valueOf(0)";
        } break;
        case Builtin::KindLong: {
            s << "Long.valueOf(0)";
        } break;
        case Builtin::KindFloat : {
            s << "Float.valueOf((float)0)";
        } break;
        case Builtin::KindDouble : {
            s << "Double.valueOf((double)0)";
        } break;
        default:{
            s << "new " << toObjStr(vType) << "()";
        } break;
        }
    } else {
        s << "new " << toObjStr(vType) << "()";
    }

    return s.str();
}

string Tars2Dart::generateAndroidJavaParams(
        const vector<ParamDeclPtr>& vParamDecl
        , bool needParamType
        , bool needOutParam) const {
    ostringstream s;

    bool needDot = false;
    for (size_t index = 0; index < vParamDecl.size(); ++index) {
        ParamDeclPtr paramDeclPtr = vParamDecl[index];

        if (!paramDeclPtr->isOut()) {
            if (!needDot) {
                needDot = true;
            } else {
                s << ", ";
            }
            if (needParamType) {
                s << tostr(paramDeclPtr->getTypeIdPtr()->getTypePtr()) << " ";
            }
            s << "in" << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId());
        } else if (needOutParam){
            if (!needDot) {
                needDot = true;
            } else {
                s << ", ";
            }
            if (needParamType) {
                s << "WupOutWrapper<" << toObjStr(paramDeclPtr->getTypeIdPtr()->getTypePtr()) << "> ";
            }
            s<< "out" << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId());
        }
    }
    return s.str();
}



