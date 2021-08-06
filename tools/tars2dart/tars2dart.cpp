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

static string g_default_package = "com.tars";
//////////////////////////////////////////////////////////////////////////////////
//
Tars2Dart::Tars2Dart()
{
    _bForceArray = false;
    _bCheckDefault = false;
    _bWithJbr = false;
    _bWithCompact = false;
    s_TARS_PACKAGE       = g_default_package + TARS_PACKAGE;
    s_PROXY_PACKAGE     = g_default_package + PROXY_PACKAGE;
    s_WUP_PACKAGE       = g_default_package + WUP_PACKAGE;
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
    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr);
    if (bPtr) return result;

    VectorPtr vPtr = VectorPtr::dynamicCast(pPtr);
    if (vPtr){
        StructPtr vsPtr = StructPtr::dynamicCast(vPtr->getTypePtr());
        if (vsPtr){ result.push_back(vsPtr->getId()); return result; }
    }

    MapPtr mPtr = MapPtr::dynamicCast(pPtr);
    if (mPtr) {
        StructPtr mlPtr = StructPtr::dynamicCast(mPtr->getLeftTypePtr());
        StructPtr mrPtr = StructPtr::dynamicCast(mPtr->getRightTypePtr());
        if(mlPtr && mrPtr){ result.push_back(mlPtr->getId());result.push_back(mrPtr->getId());   return result; } 
        if (mlPtr) { result.push_back(mlPtr->getId()); return result; }
        if (mrPtr) { result.push_back(mrPtr->getId()); return result; }
    }

    StructPtr sPtr = StructPtr::dynamicCast(pPtr);
    if (sPtr) { result.push_back(sPtr->getId()); return result; } 

    EnumPtr ePtr = EnumPtr::dynamicCast(pPtr);
    if (ePtr){ result.push_back(ePtr->getId()); return result; }

    return result;
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
        s = "Uint8List";
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
    generateAndroidStub(pPtr, nPtr);
    return s.str();
}

string Tars2Dart::generateDefautElem(const TypePtr& pPtr, const string& sElemName) const
{
    static int iCount = 0;
    ostringstream s;

    if (sElemName.empty())
    {
        iCount++;
    }

    VectorPtr vPtr = VectorPtr::dynamicCast(pPtr);
    if (vPtr)
    {
        int iId = iCount;
        string sVar = sElemName.empty() ? "__var_" + tars::TC_Common::tostr(iId) : sElemName;
        if (sElemName.empty())
        {
            s << TAB << toObjStr(vPtr) << " " << sVar
                << " = " << toTypeInit(vPtr) << endl;
        }

        BuiltinPtr bPtr = BuiltinPtr::dynamicCast(vPtr->getTypePtr());
        if (bPtr && bPtr->kind() == Builtin::KindByte)
        {
            iCount++;
            s << TAB << "byte __var_" << tars::TC_Common::tostr(iCount)
                << " = 0;"  << endl;

            s << TAB << "((" << tostr(vPtr->getTypePtr()) << "[])" << sVar
                << ")[0] = __var_" << tars::TC_Common::tostr(iId + 1) << ";" << endl;
        }
        else if (_bForceArray)
        {
            s << generateDefautElem(vPtr->getTypePtr(), "");
            s << TAB << "(List<" << tostr(vPtr->getTypePtr()) << ">)" << sVar
                << ")[0] = __var_" << tars::TC_Common::tostr(iId + 1) << ";" << endl;
        }
        else
        {
            s << generateDefautElem(vPtr->getTypePtr(), "");
            s <<  TAB << "((" << tostr(pPtr) << ")" << sVar << ").add(__var_" << tars::TC_Common::tostr(iId + 1) << ");" << endl;
        }

        return s.str();
    }

    MapPtr mPtr = MapPtr::dynamicCast(pPtr);
    if (mPtr)
    {
        int iId = iCount;
        string sVar = sElemName.empty() ? "__var_" + tars::TC_Common::tostr(iId) : sElemName;

        if (sElemName.empty())
        {
            s << TAB << toObjStr(mPtr) << " " << sVar
                << " = " << toTypeInit(mPtr) << endl;
        }

        s << generateDefautElem(mPtr->getLeftTypePtr(), "");
        int iIdRight = iCount + 1;
        s << generateDefautElem(mPtr->getRightTypePtr(), "");

        s << TAB << sVar << ".put(__var_" << tars::TC_Common::tostr(iId + 1) << ", __var_"
            << tars::TC_Common::tostr(iIdRight) << ");" << endl;

        return s.str();
    }

    //首个非map、vector调用，不用生成默认元素
    if (!sElemName.empty())
    {
        return s.str();
    }

    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr);
    if (bPtr)
    {
        s << TAB << toObjStr(bPtr) << " __var_" << tars::TC_Common::tostr(iCount)
            << " = " << toTypeInit(bPtr) << endl;
        return s.str();
    }

    StructPtr sPtr = StructPtr::dynamicCast(pPtr);
    if (sPtr)
    {
        s << TAB << toObjStr(sPtr) << " __var_" << tars::TC_Common::tostr(iCount)
            << " = " << toTypeInit(sPtr) << endl;
        return s.str();
    }

    EnumPtr ePtr = EnumPtr::dynamicCast(pPtr);
    if (ePtr)
    {
        s << TAB << toObjStr(ePtr) << " __var_" << tars::TC_Common::tostr(iCount)
            << " = " << toTypeInit(ePtr) << endl;
        return s.str();
    }

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
            return "Uint8List.fromList(List.filled(1,0))";
        }

        return "List<"+tostr(vPtr->getTypePtr())+">.filled(1,"+ generateNewElem(vPtr->getTypePtr()) +")";
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
            cacheCodeStr = "static Uint8List " + cacheName+ " = Uint8List.fromList(List.filled(1,0))";
        }else{
            cacheCodeStr = "static List<" + tostr(vPtr->getTypePtr()) + "> " + cacheName + " = List.filled(1,"+ generateNewElem(vPtr->getTypePtr()) +")";
        }
    }

    MapPtr mPtr = MapPtr::dynamicCast(pPtr->getTypePtr());
    if (mPtr)
    {
        string leftType = tostr(mPtr->getLeftTypePtr());
        string rightType = tostr(mPtr->getRightTypePtr());

        cacheCodeStr = "static Map<"+ leftType +", "+ rightType +"> " + cacheName
        + " = Map.fromEntries(List<MapEntry<"+ leftType +", "+ rightType 
        +">>.filled(1,MapEntry<"+ leftType +", "+ rightType +">("+ generateNewElem(mPtr->getLeftTypePtr()) +","+ generateNewElem(mPtr->getRightTypePtr()) + ")))";
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
    // s << TAB << "import '" << _tarsPackage << "tars_util.dart';"<< endl;
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
//  if (_bWithWsp)
//  {
//      s << " implements WspStruct";
//      bHasImpPrefix = true;
//  }

    // if (key.size() > 0)
    // {
    //     if (bHasImpPrefix)
    //     {
    //         s << ", Comparable<" << pPtr->getId() << ">";
    //     }
    //     else
    //     {
    //         s << " implements Comparable<" << pPtr->getId() << ">";
    //         bHasImpPrefix = true;
    //     }
    // }

    // if (!_bWithCompact)
    // {
    //     if (bHasImpPrefix)
    //     {
    //         s << ", java.lang.Cloneable";
    //     }
    //     else
    //     {
    //         s << " implements java.lang.Cloneable";
    //     }
    // }
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


        // 生成带包前缀的结构名
        // s << TAB << "public String fullClassName()" << endl;
        // s << TAB << "{" << endl;
        // INC_TAB;
        // s << TAB << "return " << "\"" << _packagePrefix << nPtr->getId() << "." << pPtr->getId() << "\"" << ";" << endl;
        // DEL_TAB;
        // s << TAB << "}" << endl;
        // s << endl;
    }

//  if (_bWithWsp)
//  {
//      s << TAB <<"private HashMap<String,Object> arr = new HashMap<String,Object>();"<<endl;
//  }

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

    // set 和 get 不再需要
//     if (!_bWithCompact)
//     {
//         //成员变量get/set for java bean
//         for (size_t i = 0; i < member.size(); i++)
//         {
//             string sName = "";
//             //支持javabean规范,并且第二个字符是大写
//             if (_bWithJbr && member[i]->getId()[1] >= 'A' && member[i]->getId()[1] <= 'Z')
//             {
//                 sName = member[i]->getId();
//             }
//             else
//             {
//                 sName = tars::TC_Common::upper(member[i]->getId().substr(0, 1)) + member[i]->getId().substr(1);
//             }
//             s << TAB << "public " << tostr(member[i]->getTypePtr()) << " get" << sName
//                 << "()" << endl;
//             s << TAB << "{" << endl;
//             INC_TAB;
//             s << TAB << "return " <<  member[i]->getId() << ";" << endl;
//             DEL_TAB;
//             s << TAB << "}" << endl;
//             s << endl;

//             s << TAB << "public void " << " set" << sName << "(" << tostr(member[i]->getTypePtr()) << " " << member[i]->getId() << ")" << endl;
//             s << TAB << "{" << endl;
//             INC_TAB;
//             s << TAB << "this." <<  member[i]->getId() << " = " <<  member[i]->getId() << ";" << endl;
// //          if (_bWithWsp)
// //          {
// //              s << TAB <<"arr.put(\""<<member[i]->getId()<<"\","<< member[i]->getId()<<");"<<endl;
// //          }
//             DEL_TAB;
//             s << TAB << "}" << endl;
//             s << endl;
//         }
//     }

    //(constructor)()
    // s << TAB << "" << pPtr->getId() << "();" << endl;

//     s << TAB << "{" << endl;
//     INC_TAB;
// //  if (_bWithWsp)
// //  {
// //      s << TAB << "buildIndex();" << endl;
// //  }
// //  else
//     {
// /*
//  * 无参的够造函数，不用调用成员设置方法
//         for (size_t i = 0; i < member.size(); i++)
//         {
//             string sName = "";
//             //支持javabean规范,并且第二个字符是大写
//             if (_bWithJbr && member[i]->getId()[1] >= 'A' && member[i]->getId()[1] <= 'Z' ) 
//             {
//                 sName = member[i]->getId();
//             }
//             else
//             {
//                 sName = tars::TC_Common::upper(member[i]->getId().substr(0, 1)) + member[i]->getId().substr(1);
//             }
//             s << TAB <<"set" << sName  << "(" << member[i]->getId() << ");" << endl;
//         }
// */
//     }

//     DEL_TAB;
//     s << TAB << "}" << endl;
//     s << endl;

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

    //compareTo()
    // if (key.size() > 0)
    // {
    //     s << TAB << "int compareTo(" << pPtr->getId() << " o)" << endl;
    //     s << TAB << "{" << endl;
    //     INC_TAB;

    //     s << TAB << "int[] r = " << endl;
    //     s << TAB << "{" << endl;
    //     INC_TAB;

    //     for (size_t i = 0; i < key.size(); i++)
    //     {
    //         s << TAB << s_TARS_PACKAGE << ".TarsUtil.compareTo(" << key[i] << ", o."
    //             << key[i] << ")" << ((i < key.size() - 1) ? ", " : "") << endl;
    //     }
    //     DEL_TAB;
    //     s << TAB << "};" << endl;

    //     s << TAB << "for(int i = 0; i < r.length; ++i)" << endl;
    //     s << TAB << "{" << endl;
    //     INC_TAB;
    //     s << TAB << "if(r[i] != 0) return r[i];" << endl;
    //     DEL_TAB;
    //     s << TAB << "}" << endl;
    //     s << TAB << "return 0;" << endl;
    //     DEL_TAB;
    //     s << TAB << "}" << endl;
    //     s << endl;
    // }

    // if (!_bWithCompact)
    // {
    //     //equals()
    //     s << TAB << "boolean equals(Object o)" << endl;
    //     s << TAB << "{" << endl;
    //     INC_TAB;

    //     s << TAB << "if(o == null)" << endl;
    //     s << TAB << "{" << endl;
    //     INC_TAB;
    //     s << TAB << "return false;" << endl;
    //     DEL_TAB;
    //     s << TAB << "}" << endl;
    //     s << endl;

    //     s << TAB << pPtr->getId() << " t = (" << pPtr->getId() << ") o;" << endl;
    //     s << TAB << "return (" << endl;

    //     INC_TAB;

    //     //定义了key
    //     if (key.size() > 0)
    //     {
    //         for (size_t i = 0; i < key.size(); i++)
    //         {
    //             s << TAB << s_TARS_PACKAGE << ".TarsUtil.equals(" << key[i] << ", t." << key[i] << ")"
    //                 << ((i < key.size() - 1) ? " && " : " );") << endl;
    //         }
    //     }
    //     else
    //     {
    //         //使用所有元素比较
    //         for (size_t i = 0; i < member.size(); i++)
    //         {
    //             s << TAB << s_TARS_PACKAGE << ".TarsUtil.equals(" << member[i]->getId() << ", t." << member[i]->getId() << ")"
    //                 << ((i < member.size() - 1) ? " && " : " );") << endl;
    //         }
    //     }
    //     DEL_TAB;

    //     DEL_TAB;
    //     s << TAB << "}" << endl;
    //     s << endl;
    // }

    // if (!_bWithCompact)
    // {
    //     //hashCode()
    //     if (key.size() > 0)
    //     {
    //         s << TAB << "int hashCode()" << endl;
    //         s << TAB << "{" << endl;
    //         INC_TAB;

    //         s << TAB << "int [] hc = { " << endl;
    //         INC_TAB;
    //         for (size_t i = 0; i < key.size(); i++)
    //         {
    //             s << TAB << s_TARS_PACKAGE << ".TarsUtil.hashCode(" << key[i]
    //                 << ")" << ((i < key.size() - 1) ? ", " : "") << endl;
    //         }
    //         DEL_TAB;
    //         s << TAB << "};" << endl;
    //         s << TAB << "return java.util.Arrays.hashCode(hc);" << endl;

    //         DEL_TAB;
    //         s << TAB << "}" << endl;
    //         s << endl;
    //     }
    //     else //生成异常代码
    //     {
    //         s << TAB << "int hashCode()" << endl;
    //         s << TAB << "{" << endl;
    //         INC_TAB;
    //         s << TAB << "try" << endl;
    //         s << TAB << "{" << endl;
    //         INC_TAB;
    //         s << TAB << "throw new Exception(\"Need define key first!\");" << endl;
    //         DEL_TAB;
    //         s << TAB << "}" << endl;
    //         s << TAB << "catch(Exception ex)" << endl;
    //         s << TAB << "{" << endl;
    //         INC_TAB;
    //         s << TAB << "ex.printStackTrace();" << endl;
    //         DEL_TAB;
    //         s << TAB << "}" << endl;
    //         s << TAB << "return 0;" << endl;
    //         DEL_TAB;
    //         s << TAB << "}" << endl;
    //     }
    // }

    // if (!_bWithCompact)
    // {
    //     //clone()
    //     s << TAB << "java.lang.Object clone()" << endl;
    //     s << TAB << "{" << endl;
    //     INC_TAB;
    //     s << TAB << "java.lang.Object o = null;" << endl;
    //     s << TAB << "try" << endl;
    //     s << TAB << "{" << endl;
    //     INC_TAB;
    //     s << TAB << "o = super.clone();" << endl;
    //     DEL_TAB;
    //     s << TAB << "}" << endl;
    //     s << TAB << "catch(CloneNotSupportedException ex)" << endl;
    //     s << TAB << "{" << endl;
    //     INC_TAB;
    //     s << TAB << "assert false; // impossible" << endl;
    //     DEL_TAB;
    //     s << TAB << "}" << endl;
    //     s << TAB << "return o;" << endl;
    //     DEL_TAB;
    //     s << TAB << "}" << endl;
    //     s << endl;
    // }

    //writeTo()
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
            //s << TAB << "_os.write(" << pPtr->getId() << ", " << pPtr->getTag() << ");" << endl;
            MapPtr mPtr = MapPtr::dynamicCast(member[i]->getTypePtr());
            VectorPtr vPtr = VectorPtr::dynamicCast(member[i]->getTypePtr());

            if (!_bCheckDefault || member[i]->isRequire() || (!member[i]->hasDefault() && !mPtr && !vPtr))
            {
                s << TAB << "_os.write(" << member[i]->getId() << ", " << member[i]->getTag() << ");" << endl;
            }
            else
            {
                string sDefault = member[i]->def();
                BuiltinPtr bPtr = BuiltinPtr::dynamicCast(member[i]->getTypePtr());

                if (bPtr && bPtr->kind() == Builtin::KindString)
                {
                    sDefault = "\"" + tars::TC_Common::replace(member[i]->def(), "\"", "\\\"") + "\"";
                }

                if (mPtr)
                {
                    s << TAB << "if (" << member[i]->getId() << ".size() > 0)" << endl;
                }
                else if (vPtr)
                {
                    BuiltinPtr newPtr = BuiltinPtr::dynamicCast(vPtr->getTypePtr());
                    if (newPtr && newPtr->kind() == Builtin::KindByte)
                    {
                        s << TAB << "if (" << member[i]->getId() << ".length > 0)" << endl;
                    }
                    else if (_bForceArray)
                    {
                        s << TAB << "if (" << member[i]->getId() << ".length > 0)" << endl;
                    }
                    else
                    {
                        s << TAB << "if (" << member[i]->getId() << ".size() > 0)" << endl;
                    }
                }
                else if (bPtr && bPtr->kind() == Builtin::KindString)
                {
                    s << TAB << "if (!(" << sDefault << ".equals(" << member[i]->getId() << ")))" << endl;
                }
                else
                {
                    s << TAB << "if (" << member[i]->getId() << " != " << sDefault << ")" << endl;
                }
                s << TAB << "{" << endl;
                INC_TAB;
                s << TAB << "_os.write(" << member[i]->getId() << ", " << member[i]->getTag() << ");" << endl;
                DEL_TAB;
                s << TAB << "}" << endl;
            }
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

            // string prefix = "cache_";
            // s << TAB << "static " <<  tostr(member[i]->getTypePtr()) << " cache_" << member[i]->getId() << ";" << endl;
            // s << TAB << "static {" << endl;
            // INC_TAB;
            // s << TAB << prefix << member[i]->getId() << " = " << toTypeInit(member[i]->getTypePtr()) << endl;
            // s << generateDefautElem(member[i]->getTypePtr(), prefix + member[i]->getId());
            // DEL_TAB;
            // s << TAB << "}" << endl;
        }
    }
    s << endl;



    //readFrom()
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

//  if (_bJson)
//  {
//      //writeToJsonString
//      s << TAB << "public String writeToJsonString() throws JSONException" << endl;
//      s << TAB << "{" << endl;
//      INC_TAB;
//      s << TAB << "return JSON.toJSONString(this);" << endl;
//      DEL_TAB;
//      s << TAB << "}" << endl;
//      s << endl;
//
//      //readFromJsonString
//      s << TAB << "public void readFromJsonString(String text) throws JSONException" << endl;
//      s << TAB << "{" << endl;
//      INC_TAB;
//      s << TAB << pPtr->getId() << " temp = " << "JSON.parseObject(text, " << pPtr->getId() << ".class);" << endl;
//      for (size_t i = 0; i < member.size(); i++)
//      {
//          s << TAB << "this." << member[i]->getId() << " = temp." << member[i]->getId() << ";" << endl;
//      }
//      DEL_TAB;
//      s << TAB << "}" << endl;
//      s << endl;
//  }

    if (!_bWithCompact)
    {
        //display()
        s << TAB << "void display(StringBuffer _os, int _level)" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB  << "TarsDisplayer _ds = new "  << "TarsDisplayer(_os,  level : _level);" << endl;
        for (size_t i = 0; i < member.size(); i++)
        {
            s << TAB << "_ds.display(" << member[i]->getId()
                << ", \"" << member[i]->getId() << "\");" << endl;
        }
        DEL_TAB;
        s << TAB << "}" << endl;
        s << endl;

        //displaySimple()
//      s << TAB << "public void displaySimple(java.lang.StringBuilder _os, int _level)" << endl;
//      s << TAB << "{" << endl;
//      INC_TAB;
//      s << TAB << s_TARS_PACKAGE << ".TarsDisplayer _ds = new " << s_TARS_PACKAGE << ".TarsDisplayer(_os, _level);" << endl;
//      for (size_t i = 0; i < member.size(); i++)
//      {
//          if(i != member.size() - 1)
//          {
//              s << TAB << "_ds.displaySimple(" << member[i]->getId() << ", true);" << endl;
//          }
//          else
//          {
//              s << TAB << "_ds.displaySimple(" << member[i]->getId() << ", false);" << endl;
//          }
//      }
//      DEL_TAB;
//      s << TAB << "}" << endl;
//      s << endl;
    }


//    if (_bWithWsp)
//    {
//        s << TAB << "public Object getFieldByName(String name)" << endl;
//        s << TAB << "{" << endl;
//        INC_TAB;
//        s << TAB <<"return arr.get(name);"<<endl;
//        DEL_TAB;
//        s << TAB << "}" << endl;
//        s <<endl;
//
////        s << TAB <<"@Override"<<endl;
//        s << TAB << "public boolean containField(String name)" << endl;
//        s << TAB << "{" << endl;
//        INC_TAB;
//        s << TAB <<"return arr.containsKey(name);"<<endl;
//        DEL_TAB;
//        s << TAB << "}" << endl;
//        s <<endl;
//
////        s << TAB <<"@Override"<<endl;
//        s << TAB << "public void setFieldByName(String name, Object value)" << endl;
//        s << TAB << "{" << endl;
//        INC_TAB;
//        s << TAB <<"throw new RuntimeException(this+\" can not support setFieldByName now\");"<<endl;
//        DEL_TAB;
//        s << TAB << "}" << endl;
//        s <<endl;
//
////        s << TAB <<"@Override"<<endl;
//        s << TAB << "public void buildIndex()" << endl;
//        s << TAB << "{" << endl;
//        INC_TAB;
//        for (size_t i = 0; i < member.size(); i++)
//        {
//            string sName = "";
//            //支持javabean规范,并且第二个字符是大写
//            if (_bWithJbr && member[i]->getId()[1] >= 'A' && member[i]->getId()[1] <= 'Z' )
//            {
//                sName = member[i]->getId();
//            }
//            else
//            {
//                sName = tars::TC_Common::upper(member[i]->getId().substr(0, 1)) + member[i]->getId().substr(1);
//            }
//            s << TAB <<"set" << sName << "(" << member[i]->getId() << ");" << endl;
//        }
//        DEL_TAB;
//        s << TAB << "}" << endl;
//    }

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

    s << TAB << "package " << _packagePrefix << nPtr->getId() << ";" << endl;
    s << endl;

    s << TAB << "public final class " << pPtr->getId() << " implements java.io.Serializable" << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    if (_bEnumCompact)
    {
        //成员变量
        vector<TypeIdPtr>& member = pPtr->getAllMemberPtr();
        bool bFlag = false;
        string sValue;
        for (size_t i = 0; i < member.size(); i++)
        {
            s << TAB << "public static final int _" << member[i]->getId() << " = ";
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
    }
    else
    {
        //成员变量
        vector<TypeIdPtr>& member = pPtr->getAllMemberPtr();
        s << TAB << "private static " << pPtr->getId()
            << "[] __values = new " << pPtr->getId()
            << "[" << tars::TC_Common::tostr(member.size()) << "];" << endl;
        s << TAB << "private int __value;" << endl;

        s << TAB << "private String __T = new String();" << endl;
        s << endl;

        bool bFlag = false;
        string sValue;
        for (size_t i = 0; i < member.size(); i++)
        {
            s << TAB << "public static final int _" << member[i]->getId() << " = ";
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
            s << TAB << "public static final " << pPtr->getId() << " "
                << member[i]->getId() << " = new " << pPtr->getId() << "(" << i << ",_"
                << member[i]->getId() << ",\"" << member[i]->getId() << "\");" << endl;
        }
        s << endl;


        //convert(int)
        s << TAB << "public static " << pPtr->getId() << " convert(int val)" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB << "for(int __i = 0; __i < __values.length; ++__i)" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB << "if(__values[__i].value() == val)" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB << "return __values[__i];" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;
        s << TAB << "assert false;" << endl;
        s << TAB << "return null;" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;
        s << endl;

        //convert(String)
        s << TAB << "public static " << pPtr->getId() << " convert(String val)" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB << "for(int __i = 0; __i < __values.length; ++__i)" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB << "if(__values[__i].toString().equals(val))" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB << "return __values[__i];" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;
        s << TAB << "assert false;" << endl;
        s << TAB << "return null;" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;
        s << endl;

        //value()
        s << TAB << "public int value()" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB << "return __value;" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;
        s << endl;

        //toString()
        s << TAB << "public String toString()" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB << "return __T;" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;
        s << endl;

        //(constructor)(int)
        s << TAB << "private " << pPtr->getId() << "(int index, int val, String s)" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB << "__T = s;" << endl;
        s << TAB << "__value = val;" << endl;
        s << TAB << "__values[index] = this;" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;
        s << endl;
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
    ostringstream s;
    s << g_parse->printHeaderRemark();

    //const类型的包单独放到一个const目录下面
    s << TAB << "package " << _packagePrefix << nPtr->getId() << ".cnst" << ";" << endl;
    s << endl;

    s << TAB << "public interface " << pPtr->getTypeIdPtr()->getId() << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    //TODO: String转义 / long 识别 / byte 范围
    if (pPtr->getConstGrammarPtr()->t == ConstGrammar::STRING)
    {
        string tmp = tars::TC_Common::replace(pPtr->getConstGrammarPtr()->v, "\"", "\\\"");
        s << TAB << tostr(pPtr->getTypeIdPtr()->getTypePtr()) << " value = \""
            << tmp << "\";" << endl;
    }
    else if (tostr(pPtr->getTypeIdPtr()->getTypePtr()) == "long")
    {
        s << TAB << tostr(pPtr->getTypeIdPtr()->getTypePtr()) << " value = "
            << pPtr->getConstGrammarPtr()->v << "L;" << endl;
    }
    else
    {
        s << TAB << tostr(pPtr->getTypeIdPtr()->getTypePtr()) << " value = "
            << pPtr->getConstGrammarPtr()->v << ";" << endl;
    }

    DEL_TAB;
    s << TAB << "}" << endl;

    string fileJava  = getFilePath(nPtr->getId()) + "/cnst/" + pPtr->getTypeIdPtr()->getId() + ".dart";
    tars::TC_File::makeDirRecursive(getFilePath(nPtr->getId() + "/cnst/"));
    tars::TC_File::save2file(fileJava, s.str());

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

string Tars2Dart::generateAndroidStub(const InterfacePtr &pPtr, const NamespacePtr &nPtr) const {
    ostringstream s;

    s << g_parse->printHeaderRemark();
    s << TAB << "package " << _packagePrefix << nPtr->getId() << ";" << endl;

    s << endl;
    s << TAB << "import java.util.concurrent.Semaphore;" << endl;
    s << TAB << "import com.tars.wupagent.api.WupConstants.WUP_ERROR_CODE;" << endl;
    s << TAB << "import com.tars.wupagent.api.*;" << endl;
    
    s << TAB << "import com.tars.wup.UniPacket;" << endl;
    s << endl;

    s << TAB << "public class " << firstCharUpper(pPtr->getId()) << "Agent extends WupAgent {" << endl << endl;
    INC_TAB;

    s << TAB << "public " << firstCharUpper(pPtr->getId()) << "Agent(String servantName) {" << endl;
    INC_TAB;
    s << TAB << "super(servantName);" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;
    s << endl;

    string sParams;
    vector<OperationPtr>& vOperation = pPtr->getAllOperationPtr();
    std::sort(vOperation.begin(), vOperation.end(), SortOperation());

    for (size_t i = 0; i < vOperation.size(); i++)
    {
        vector<ParamDeclPtr>& vParamDecl = vOperation[i]->getAllParamDeclPtr();

        TypeIdPtr& returnType = vOperation[i]->getReturnPtr();
        bool isReturnTypeVoid = false;
        if (!returnType->getTypePtr()) {
            isReturnTypeVoid = true;
        }


        // generate Result
        s << TAB << "public static class " << firstCharUpper(vOperation[i]->getId()) <<  "Result extends WupBaseResult " << "{" << endl;

        INC_TAB;

        s << TAB << "public " << firstCharUpper(vOperation[i]->getId()) <<  "Result() {" << endl;
        s << TAB << "}" << endl << endl;

        s << TAB << "public " << firstCharUpper(vOperation[i]->getId()) <<  "Result(int errorCode, String errorMsg) {" << endl;
        INC_TAB;
        s << TAB << "super(errorCode, errorMsg);" << endl;
        DEL_TAB;
        s << TAB << "}" << endl << endl;

        for (size_t paramIndex = 0; paramIndex < vParamDecl.size(); paramIndex++) {

            ParamDeclPtr paramDeclPtr = vParamDecl[paramIndex];

            string inOrOutPrefix = "in";
            if (paramDeclPtr->isOut()) {
                inOrOutPrefix = "out";
            }

            s << TAB << tostr(paramDeclPtr->getTypeIdPtr()->getTypePtr()) << " ";
            s << inOrOutPrefix << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId()) << ";" << endl;

            s << TAB << "public " << tostr(paramDeclPtr->getTypeIdPtr()->getTypePtr())
                     <<  " get" << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId())
                     << "() {" << endl;
            INC_TAB;

            s << TAB << "return ";
            s << inOrOutPrefix << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId()) << ";" << endl;

            DEL_TAB;

            s << TAB << "}" << endl;


            s << TAB << "public " << firstCharUpper(vOperation[i]->getId()) <<  "Result"
                     << " set" << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId())
                     << "(" << tostr(paramDeclPtr->getTypeIdPtr()->getTypePtr())
                     << " " << paramDeclPtr->getTypeIdPtr()->getId()
                     << ") {" << endl;
            INC_TAB;
            s << TAB << "this." << inOrOutPrefix << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId())
                     << " = " << paramDeclPtr->getTypeIdPtr()->getId() << ";" << endl;
            s << TAB << "return this;" << endl;
            DEL_TAB;

            s << TAB << "}" << endl << endl;

        }

        if (!isReturnTypeVoid) {
            s << TAB << tostr(returnType->getTypePtr()) << " ret;" << endl;
            s << TAB << "public " << tostr(returnType->getTypePtr())
                     << " getRet() {" << endl;
            INC_TAB;
            s << TAB << " return ret;" << endl;
            DEL_TAB;
            s << TAB << "}" << endl;
            s << TAB << "public " << firstCharUpper(vOperation[i]->getId()) <<  "Result"
                     << " setRet(" <<  tostr(returnType->getTypePtr()) << " ret) {" << endl;
            INC_TAB;
            s << TAB << "this.ret = ret;" << endl;
            s << TAB << "return this;" << endl;
            DEL_TAB;
            s << TAB << "}" << endl << endl;
        }

        DEL_TAB;

        s << TAB << "}" << endl << endl;

        s << TAB << "public static interface I" << firstCharUpper(vOperation[i]->getId()) << "Callback {" << endl;
        INC_TAB;
        s << TAB << "public void on" << firstCharUpper(vOperation[i]->getId()) << "Callback("
                 <<  firstCharUpper(vOperation[i]->getId()) << "Result result);" << endl;
        DEL_TAB;
        s << TAB << "}" << endl << endl;

        // generate BaseMethod

        s << TAB << "private static abstract class "
                 << firstCharUpper(vOperation[i]->getId()) << "BaseMethod extends AsyncWupMethod {" << endl;
        INC_TAB;

        s << TAB << "public " << firstCharUpper(vOperation[i]->getId())
                 << "BaseMethod(String servantName, WupOption wupOption) {" << endl;
        INC_TAB;
        s << TAB << "super(servantName, \"" << vOperation[i]->getId() << "\", wupOption);" << endl;
        DEL_TAB;
        s << TAB << "}" << endl << endl;

        for (size_t paramIndex = 0; paramIndex < vParamDecl.size(); paramIndex++) {
            ParamDeclPtr paramDeclPtr = vParamDecl[paramIndex];

            string inOrOutPrefix = "in";
            bool generateSet = true;
            if (paramDeclPtr->isOut()) {
                inOrOutPrefix = "out";
                generateSet = false;
            }

            s << TAB << tostr(paramDeclPtr->getTypeIdPtr()->getTypePtr()) << " ";
            s << inOrOutPrefix << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId()) << ";" << endl;

            s << TAB << "public " << tostr(paramDeclPtr->getTypeIdPtr()->getTypePtr())
                     <<  " get" << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId())
                     << "() {" << endl;
            INC_TAB;

            s << TAB << "return ";
            s << inOrOutPrefix << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId()) << ";" << endl;

            DEL_TAB;
            s << TAB << "}" << endl;

            if (generateSet) {

                s << TAB << "public void set" << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId())
                         << "(" << tostr(paramDeclPtr->getTypeIdPtr()->getTypePtr())
                         << " " << paramDeclPtr->getTypeIdPtr()->getId()
                         << ") {" << endl;
                INC_TAB;
                s << TAB << "this." << inOrOutPrefix << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId())
                         << " = " << paramDeclPtr->getTypeIdPtr()->getId() << ";" << endl;
                DEL_TAB;
                s << TAB << "}" << endl;
            }

            s << endl;
        }

        if (!isReturnTypeVoid) {
            s << TAB << tostr(returnType->getTypePtr()) << " ret;" << endl;
            s << TAB << "public " << tostr(returnType->getTypePtr())
                     << " getRet() {" << endl;
            INC_TAB;
            s << TAB << " return ret;" << endl;
            DEL_TAB;
            s << TAB << "}" << endl << endl;
        }

        s << TAB << "@Override" << endl;
        s << TAB << "protected void fillReqUniPacket(UniPacket reqPacket) {" << endl;
        INC_TAB;
        for (size_t paramIndex = 0; paramIndex < vParamDecl.size(); paramIndex++) {
            ParamDeclPtr paramDeclPtr = vParamDecl[paramIndex];

            if (!paramDeclPtr->isOut()) {
                s << TAB << "reqPacket.put(\"" << paramDeclPtr->getTypeIdPtr()->getId()
                         << "\", in" << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId()) << ");" << endl;
            }
        }
        DEL_TAB;
        s << TAB << "}" << endl <<endl;


        s << TAB << "@Override" << endl;
        s << TAB << "protected void fillFieldsByUniPacket(UniPacket respPacket) {" << endl;
        INC_TAB;

        if (!isReturnTypeVoid) {
            s << TAB << "ret = respPacket.get(\"\", "
                     << generateConstructor(returnType->getTypePtr()) << ");" << endl;
        }

        for (size_t paramIndex = 0; paramIndex < vParamDecl.size(); paramIndex++) {
            ParamDeclPtr paramDeclPtr = vParamDecl[paramIndex];
            if (paramDeclPtr->isOut()) {
                s << TAB << "out" << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId())
                         << " = respPacket.get(\"" << paramDeclPtr->getTypeIdPtr()->getId()
                         << "\", " << generateConstructor(paramDeclPtr->getTypeIdPtr()->getTypePtr()) << ");" << endl;
            }
        }

        DEL_TAB;
        s << TAB << "}" << endl <<endl;

        DEL_TAB;
        s << TAB << "}" << endl << endl;

        // generate AsyncMethod

        s << TAB << "private static class Async" << firstCharUpper(vOperation[i]->getId())
                 << "Method extends " << firstCharUpper(vOperation[i]->getId()) << "BaseMethod {" << endl;
        INC_TAB;
        s << TAB << "private I" <<  firstCharUpper(vOperation[i]->getId())
                 << "Callback m" << firstCharUpper(vOperation[i]->getId()) << "Callback;" << endl;

        s << TAB << "public Async" << firstCharUpper(vOperation[i]->getId()) << "Method(String servantName"
                 << ", WupOption wupOption"
                 << ", I" << firstCharUpper(vOperation[i]->getId())
                 << "Callback " << vOperation[i]->getId() << "Callback) {" << endl;
        INC_TAB;
        s << TAB << "super(servantName, wupOption);" << endl;
        s << TAB << "this.m" <<  firstCharUpper(vOperation[i]->getId()) << "Callback = "
                 << vOperation[i]->getId() << "Callback;" << endl;
        DEL_TAB;
        s << TAB << "}" << endl << endl;

        s << TAB << "@Override" << endl;
        s << TAB << "protected void onError(int errorCode, String errorMsg) {" << endl;
        INC_TAB;
        s << TAB << firstCharUpper(vOperation[i]->getId())
                 << "Result result = new " <<  firstCharUpper(vOperation[i]->getId()) << "Result(errorCode, errorMsg);" << endl;
        s << TAB << "result.setRequestId(getRequestId());" << endl;
        s << TAB << "m" << firstCharUpper(vOperation[i]->getId())
                 << "Callback.on" << firstCharUpper(vOperation[i]->getId()) << "Callback(result);" << endl;
        DEL_TAB;
        s << TAB << "}" << endl << endl;

        s << TAB << "@Override" << endl;
        s << TAB << "protected void onFinished() {" << endl;
        INC_TAB;
        s << TAB << firstCharUpper(vOperation[i]->getId())
                 << "Result result = new " <<  firstCharUpper(vOperation[i]->getId()) << "Result();" << endl;
        s << TAB << "result.setRequestId(getRequestId());" << endl;
        for (size_t paramIndex = 0; paramIndex < vParamDecl.size(); paramIndex++) {
            ParamDeclPtr paramDeclPtr = vParamDecl[paramIndex];

            s << TAB << "result.set" << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId())
                     << "(get" << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId()) << "());" << endl;
        }
        if (!isReturnTypeVoid) {
            s << TAB << "result.setRet(getRet());" << endl;
        }
        s << TAB << "m" << firstCharUpper(vOperation[i]->getId())
                 << "Callback.on" << firstCharUpper(vOperation[i]->getId()) << "Callback(result);" << endl;
        DEL_TAB;
        s << TAB << "}" << endl << endl;

        DEL_TAB;
        s << TAB << "}" << endl << endl;

        // generate AsyncMethod end

        // generate SyncMethod
        s << TAB << "private static class Sync" << firstCharUpper(vOperation[i]->getId())
                 << "Method extends " << firstCharUpper(vOperation[i]->getId()) << "BaseMethod {" << endl;
        INC_TAB;
        s << TAB << "private Semaphore mSemaphore;" << endl;
        s << TAB << "private WupException mWupException;" << endl;

        s << TAB << "public Sync" << firstCharUpper(vOperation[i]->getId())
                 << "Method(String servantName, WupOption wupOption) {" << endl;
        INC_TAB;
        s << TAB << "super(servantName, wupOption);" << endl;
        s << TAB << "mSemaphore = new Semaphore(0);" << endl;
        DEL_TAB;
        s << TAB  << "}" << endl << endl;

        s << TAB << "public void waitResponse() throws InterruptedException {" << endl;
        INC_TAB;
        s << TAB << "mSemaphore.acquire();" << endl;
        DEL_TAB;
        s << TAB << "}" << endl << endl;

        s << TAB << "@Override" << endl;
        s << TAB << "protected void handleFinished() {" << endl;
        INC_TAB;
        s << TAB << "mSemaphore.release();" << endl;
        DEL_TAB;
        s << TAB << "}" << endl << endl;

        s << TAB << "@Override" << endl;
        s << TAB << "protected void handleError(final int errorCode, final String errorMsg) {" << endl;
        INC_TAB;
        s << TAB << "mWupException = new WupException(errorCode, errorMsg);" << endl;
        s << TAB << "mSemaphore.release();" << endl;
        DEL_TAB;
        s << TAB << "}" << endl << endl;

        s << TAB << "public WupException getWupException() {" << endl;
        INC_TAB;
        s << TAB << "return mWupException;" << endl;
        DEL_TAB;
        s << TAB << "}" << endl << endl;

        DEL_TAB;
        s << TAB << "}" << endl << endl;

        // generate SyncMethod end

        // async api
        s << TAB << "public WupHandle async_" << vOperation[i]->getId() << "(";
        if (vParamDecl.size())
        {
            s << generateAndroidJavaParams(vParamDecl, true, false) << ", ";
        }
        s << "I" << firstCharUpper(vOperation[i]->getId()) << "Callback "
                 << vOperation[i]->getId() << "Callback) throws WupException { " << endl;
        INC_TAB;
        s << TAB << "if (" << vOperation[i]->getId() << "Callback == null) {" << endl;
        INC_TAB;
        s << TAB << "throw new IllegalArgumentException(\"" << vOperation[i]->getId()
                 << "Callback should not be null\");" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;

        s << TAB << "WupOption asyncWupOption = new WupOption(wupOption);" << endl;

        for (size_t index = 0; index < vParamDecl.size(); ++index) {
            ParamDeclPtr paramDeclPtr = vParamDecl[index];

            if (!paramDeclPtr->isOut() && isObjType(paramDeclPtr->getTypeIdPtr()->getTypePtr())) {
                s << TAB <<  "if (in" << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId()) << " == null) {" << endl;
                INC_TAB;
                s << TAB << "throw new IllegalArgumentException(\"in"
                         << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId()) << " should not be null\");" << endl;
                DEL_TAB;
                s << TAB << "}" << endl << endl;
            }

        }

        s << TAB << "Async" <<  firstCharUpper(vOperation[i]->getId()) << "Method "
                 << vOperation[i]->getId() << "Method = new Async"
                 << firstCharUpper(vOperation[i]->getId())
                 << "Method(getServantName(), asyncWupOption, " << vOperation[i]->getId() << "Callback);" << endl;

        for (size_t index = 0; index < vParamDecl.size(); ++index) {
            ParamDeclPtr paramDeclPtr = vParamDecl[index];

            if (!paramDeclPtr->isOut()) {
                s << TAB << vOperation[i]->getId() << "Method.set"
                         << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId())
                         << "(in" << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId()) << ");" << endl;
            }

        }

        s << TAB << vOperation[i]->getId() << "Method.start();" << endl;
        s << TAB << "return new WupHandle(" <<  vOperation[i]->getId() << "Method);" << endl;

        DEL_TAB;
        s << TAB << "}" << endl << endl;

        // async api end

        // sync api
        s << TAB << "public " << tostr(returnType->getTypePtr())
                 << " " << vOperation[i]->getId() << "("
                 << generateAndroidJavaParams(vParamDecl, true, true)
                 << ") throws WupException {"  << endl;
        INC_TAB;

        for (size_t index = 0; index < vParamDecl.size(); ++index) {
            ParamDeclPtr paramDeclPtr = vParamDecl[index];

            string prefix = "in";
            if (paramDeclPtr->isOut()) {
                prefix = "out";
            }
            if (isObjType(paramDeclPtr->getTypeIdPtr()->getTypePtr())) {
                s << TAB <<  "if (" << prefix << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId()) << " == null) {" << endl;
                INC_TAB;
                s << TAB << "throw new IllegalArgumentException(\"" << prefix
                         << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId()) << " should not be null\");" << endl;
                DEL_TAB;
                s << TAB << "}" << endl << endl;
            }

        }

        s << TAB << "Sync" << firstCharUpper(vOperation[i]->getId())
                << "Method " << "sync" << firstCharUpper(vOperation[i]->getId())
                << "Method = " << "new Sync"
                << firstCharUpper(vOperation[i]->getId())
                << "Method(getServantName(), wupOption);"
                << endl;

        for (size_t index = 0; index < vParamDecl.size(); ++index) {
            ParamDeclPtr paramDeclPtr = vParamDecl[index];

            if (!paramDeclPtr->isOut()) {
                s << TAB << "sync" << firstCharUpper(vOperation[i]->getId())
                         << "Method.set"
                         << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId())
                         << "(in"
                         << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId())
                         << ");" << endl;
            }

        }

        s << TAB << "sync" << firstCharUpper(vOperation[i]->getId())
                 << "Method.start();" << endl << endl;
        s << TAB << "try {" << endl;
        INC_TAB;
        s << TAB << "sync" << firstCharUpper(vOperation[i]->getId())
                 << "Method.waitResponse();" << endl;
        DEL_TAB;
        s << TAB << "} catch (InterruptedException e) {" << endl;
        INC_TAB;
        s << TAB << "throw new WupException(WUP_ERROR_CODE.WUP_INNER_ERROR, e.getMessage());"
                 << endl;
        DEL_TAB;
        s << TAB << "}" << endl << endl;
        s << TAB << "if (sync" << firstCharUpper(vOperation[i]->getId())
                 << "Method.getWupException() != null) {" << endl;
        INC_TAB;
        s << TAB << "throw sync" << firstCharUpper(vOperation[i]->getId())
                 << "Method.getWupException();" << endl;
        DEL_TAB;
        s << TAB << "}" << endl << endl;

        for (size_t index = 0; index < vParamDecl.size(); ++index) {
            ParamDeclPtr paramDeclPtr = vParamDecl[index];

            if (paramDeclPtr->isOut()) {
                s << TAB << "out" << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId())
                         << ".setOut"
                         << "(sync" << firstCharUpper(vOperation[i]->getId()) << "Method.get"
                         << firstCharUpper(paramDeclPtr->getTypeIdPtr()->getId())
                        << "());" << endl;
            }
        }

        if (!isReturnTypeVoid) {
            s << TAB << "return sync" << firstCharUpper(vOperation[i]->getId())
                     << "Method.getRet();" << endl;
        }

        DEL_TAB;
        s << TAB << "}" << endl << endl;
        // sync api end
    }

    DEL_TAB;
    s << TAB << "}" << endl << endl;


    string fileJava = getFilePath(nPtr->getId()) + pPtr->getId() + "Agent.dart";
    tars::TC_File::makeDirRecursive(getFilePath(nPtr->getId()));
    tars::TC_File::save2file(fileJava, s.str());

    return s.str();
}


