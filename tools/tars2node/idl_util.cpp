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

string CodeGenerator::toFunctionName(const TypeIdPtr& pPtr, const string& sAction)
{
    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr->getTypePtr());
    if (bPtr)
    {
        switch (bPtr->kind())
        {
            case Builtin::KindBool   : return sAction + "Boolean";
            case Builtin::KindString : return sAction + "String";
            case Builtin::KindByte   : return sAction + "Int8";
            case Builtin::KindShort  : return sAction + (bPtr->isUnsigned() ? "UInt8"  : "Int16");
            case Builtin::KindInt    : return sAction + (bPtr->isUnsigned() ? "UInt16" : "Int32");
            case Builtin::KindLong   : return sAction + (bPtr->isUnsigned() ? "UInt32" : "Int64");
            case Builtin::KindFloat  : return sAction + "Float";
            case Builtin::KindDouble : return sAction + "Double";
            default                  : assert(false);
        }
    }

    VectorPtr vPtr = VectorPtr::dynamicCast(pPtr->getTypePtr());
    if (vPtr)
    {
        BuiltinPtr vbPtr = BuiltinPtr::dynamicCast(vPtr->getTypePtr());
        if (vbPtr && vbPtr->kind() == Builtin::KindByte)
        {
            return sAction + "Bytes";
        }

        return sAction + "List";
    }

    StructPtr sPtr = StructPtr::dynamicCast(pPtr->getTypePtr());
    if (sPtr)
    {
        return sAction + "Struct";
    }

    EnumPtr ePtr = EnumPtr::dynamicCast(pPtr->getTypePtr());
    if (ePtr)
    {
        return sAction + "Int32";
    }

    MapPtr mPtr = MapPtr::dynamicCast(pPtr->getTypePtr());
    if (mPtr)
    {
        return sAction + "Map";
    }

    return "";
}

string CodeGenerator::toObjectString(const TypeIdPtr &pPtr)
{
    string nameId = pPtr->getId();
    VectorPtr vPtr = VectorPtr::dynamicCast(pPtr->getTypePtr());
    if (vPtr)
    {
        return nameId + ".toObject()";
    }

    StructPtr sPtr = StructPtr::dynamicCast(pPtr->getTypePtr());
    if (sPtr)
    {
        return nameId + ".toObject()";
    }

    MapPtr mPtr = MapPtr::dynamicCast(pPtr->getTypePtr());
    if (mPtr)
    {
        return nameId + ".toObject()";
    }

    return nameId;
}

string CodeGenerator::representArgument(const TypePtr& pPtr) const
{
    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr);
    if (bPtr)
    {
        if (bPtr->kind() == Builtin::KindLong && _iLongType != CodeGenerator::Number)
        {
            stringstream str;
            str << ", " << _iLongType;
            return str.str();
        }

        if (_bStringBinaryEncoding && bPtr->kind() == Builtin::KindString)
        {
            return ", 1";
        }
    }

    return "";
}

string CodeGenerator::getClassName(const TypePtr& pPtr)
{
    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr);
    if (bPtr)
    {
        switch (bPtr->kind())
        {
            case Builtin::KindBool   : return "bool";
            case Builtin::KindString : return "string";
            case Builtin::KindByte   : return "char";
            case Builtin::KindShort  : return "short";
            case Builtin::KindInt    : return "int32";
            case Builtin::KindLong   : return "int64";
            case Builtin::KindFloat  : return "float";
            case Builtin::KindDouble : return "double";
            default                  : assert(false);
        }
    }

    VectorPtr vPtr = VectorPtr::dynamicCast(pPtr);
    if (vPtr)
    {
        return "list(" + getClassName(vPtr->getTypePtr()) + ")";
    }

    StructPtr sPtr = StructPtr::dynamicCast(pPtr);
    if (sPtr)
    {
        vector<string> vecNames = TC_Common::sepstr<string>(sPtr->getSid(), "::");
        assert(vecNames.size() == 2);

        return vecNames[0] + "." + vecNames[1];
    }

    MapPtr mPtr = MapPtr::dynamicCast(pPtr);
    if (mPtr)
    {
        return "map(" + getClassName(mPtr->getLeftTypePtr()) + ", " + getClassName(mPtr->getRightTypePtr()) + ")";
    }

    EnumPtr ePtr = EnumPtr::dynamicCast(pPtr);
    if (ePtr)
    {
        return "int32";
    }

    return "void";
}

string CodeGenerator::getDataType(const TypePtr& pPtr, const bool &bCastEnumAsAny)
{
    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr);
    if (bPtr)
    {
        switch (bPtr->kind())
        {
            case Builtin::KindBool   : return IDL_NAMESPACE_STR + "Stream.Boolean";
            case Builtin::KindString : return IDL_NAMESPACE_STR + "Stream.String";
            case Builtin::KindByte   : return IDL_NAMESPACE_STR + "Stream.Int8";
            case Builtin::KindShort  : return IDL_NAMESPACE_STR + "Stream.Int16";
            case Builtin::KindInt    : return IDL_NAMESPACE_STR + "Stream.Int32";
            case Builtin::KindLong   : return IDL_NAMESPACE_STR + "Stream.Int64";
            case Builtin::KindFloat  : return IDL_NAMESPACE_STR + "Stream.Float";
            case Builtin::KindDouble : return IDL_NAMESPACE_STR + "Stream.Double";
            default                  : assert(false);
        }
    }

    VectorPtr vPtr = VectorPtr::dynamicCast(pPtr);
    if (vPtr)
    {
        BuiltinPtr vbPtr = BuiltinPtr::dynamicCast(vPtr->getTypePtr());
        if (vbPtr && vbPtr->kind() == Builtin::KindByte)
        {
            return IDL_NAMESPACE_STR + "Stream.BinBuffer";
        }
        return IDL_NAMESPACE_STR + "Stream.List(" + getDataType(vPtr->getTypePtr(), bCastEnumAsAny) + representArgument(vPtr->getTypePtr()) + ")";
    }

    StructPtr sPtr = StructPtr::dynamicCast(pPtr);
    if (sPtr)
    {
        vector<string> vecNames = TC_Common::sepstr<string>(sPtr->getSid(), "::");
        assert(vecNames.size() == 2);

        return findName(vecNames[0], vecNames[1]);
    }

    MapPtr mPtr = MapPtr::dynamicCast(pPtr);
    if (mPtr)
    {
        string sLeft = representArgument(mPtr->getLeftTypePtr());
        string sRight = representArgument(mPtr->getRightTypePtr());

        if (sLeft.empty() && !sRight.empty())
        {
            sLeft = ", 0";
        }

        return IDL_NAMESPACE_STR + "Stream.Map(" + getDataType(mPtr->getLeftTypePtr(), bCastEnumAsAny) + ", " +
                getDataType(mPtr->getRightTypePtr(), bCastEnumAsAny) + sLeft + sRight + ")";
    }

    EnumPtr ePtr = EnumPtr::dynamicCast(pPtr);
    if (ePtr)
    {
        vector<string> vecNames = TC_Common::sepstr<string>(ePtr->getSid(), "::");
        assert(vecNames.size() == 2);
        string suffix = bCastEnumAsAny ? " as any" : "";

        return findName(vecNames[0], vecNames[1]) + suffix;
    }

    assert(false);
    return "";
}

string CodeGenerator::getTsType(const TypePtr &pPtr, const bool bStream, const bool bBase)
{
    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr);
    if (bPtr)
    {
        switch (bPtr->kind())
        {
            case Builtin::KindBool   : return "boolean";
            case Builtin::KindString : return _bStringBinaryEncoding ? "Buffer" : "string";
            case Builtin::KindByte   : return "number";
            case Builtin::KindShort  : return "number";
            case Builtin::KindInt    : return "number";
            case Builtin::KindFloat  : return "number";
            case Builtin::KindDouble : return "number";
            case Builtin::KindLong   :
            {
                switch (_iLongType)
                {
                    case CodeGenerator::Number : return "number";
                    case CodeGenerator::String : return "string";
                    case CodeGenerator::BigInt : return "bigint";
                    default                    : assert(false);
                }
            }
            default                  : assert(false);
        }
    }

    VectorPtr vPtr = VectorPtr::dynamicCast(pPtr);
    if (vPtr)
    {
        BuiltinPtr vbPtr = BuiltinPtr::dynamicCast(vPtr->getTypePtr());
        if (vbPtr && vbPtr->kind() == Builtin::KindByte)
        {
            return bStream ? (IDL_NAMESPACE_STR + "Stream.BinBuffer") : "Buffer";
        }
        return (bStream ? (IDL_NAMESPACE_STR + "Stream.List") : "Array") + string("<") + getTsType(vPtr->getTypePtr(), bStream, bBase) + string(">");
    }

    StructPtr sPtr = StructPtr::dynamicCast(pPtr);
    if (sPtr)
    {
        vector<string> vecNames = TC_Common::sepstr<string>(sPtr->getSid(), "::");
        assert(vecNames.size() == 2);

        return findName(vecNames[0], vecNames[1], bBase) + (bStream ? "" : ".Object");
    }

    MapPtr mPtr = MapPtr::dynamicCast(pPtr);
    if (mPtr)
    {
        if (bStream)
        {
            // In current version (20190311) of the streaming library, 
            // TypeScript cannot infer enum type over conditional type correctly.
            // So use `HeroMap` instead of `Map` to solve this problem.
            EnumPtr keyTypePtr = EnumPtr::dynamicCast(mPtr->getLeftTypePtr());
            string mapName = keyTypePtr ? "HeroMap" : "Map";

            return IDL_NAMESPACE_STR + "Stream." + mapName + "<" + getTsType(mPtr->getLeftTypePtr(), bStream) + ", " + getTsType(mPtr->getRightTypePtr(), bStream, bBase) + ">";
        }
        else
        {
            const string& sLeftType = getTsType(mPtr->getLeftTypePtr(), bStream, bBase);
            const string& sRightType = getTsType(mPtr->getRightTypePtr(), bStream, bBase);
            if (isSimple(mPtr->getLeftTypePtr()))
            {
                const string& recordKeyType = sLeftType == "number" ? "number" : "string";
                return "Record<" + recordKeyType + ", " + sRightType + ">";
            }
            else
            {
                return "";
            }
        }
    }

    EnumPtr ePtr = EnumPtr::dynamicCast(pPtr);
    if (ePtr)
    {
        vector<string> vecNames = TC_Common::sepstr<string>(ePtr->getSid(), "::");
        assert(vecNames.size() == 2);

        return findName(vecNames[0], vecNames[1], bBase);
    }

    assert(false);
    return "";
}

bool CodeGenerator::isSimple(const TypePtr & pPtr) const
{
    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr);
    if (bPtr)
    {
        return true;
    }

    EnumPtr ePtr = EnumPtr::dynamicCast(pPtr);
    if (ePtr)
    {
        return true;
    }

    return false;
}

bool CodeGenerator::isBinBuffer(const TypePtr & pPtr) const
{
    VectorPtr vPtr = VectorPtr::dynamicCast(pPtr);
    if (vPtr)
    {
        BuiltinPtr vbPtr = BuiltinPtr::dynamicCast(vPtr->getTypePtr());
        if (vbPtr && vbPtr->kind() == Builtin::KindByte)
        {
            return true;
        }

    }

    return false;
}

string CodeGenerator::getDefault(const TypeIdPtr & pPtr, const string &sDefault, const string& sNamespace, const bool &bGlobal, const bool &bCastEnumAsAny)
{
    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr->getTypePtr());
    if (bPtr)
    {
        switch (bPtr->kind())
        {
            case Builtin::KindBool    :
                return sDefault.empty() ? "false" : sDefault;
            case Builtin::KindString  :
            {
                if (_bStringBinaryEncoding)
                {
                    return IDL_NAMESPACE_STR + "Stream.BinBuffer.from(\"" + TC_Common::replace(sDefault, "\"", "\\\"") +  "\").toNodeBufferUnSafe()";
                }
                else
                {
                    return "\"" + TC_Common::replace(sDefault, "\"", "\\\"") +  "\"";
                }
            }
            case Builtin::KindByte    : // [[fallthrough]]
            case Builtin::KindShort   : // [[fallthrough]]
            case Builtin::KindInt     : // [[fallthrough]]
            case Builtin::KindLong    :
            {
                string sTemp = TC_Common::trim(sDefault);
                if (sTemp.empty())
                {
                    sTemp = "0";
                }
                else
                {
                    if (TC_Common::tostr(TC_Common::strto<long>(sTemp)) != sTemp)
                    {
                        // lookup in the enum when it is a enum
                        vector<string> vecNames = TC_Common::sepstr<string>(sDefault, "::");
                        if (vecNames.size() == 2)
                        {
                            sTemp = findName(vecNames[0], vecNames[1]);
                        }
                        else
                        {
                            sTemp = findName(sNamespace, sDefault);
                        }
                    }
                }

                if (sTemp.empty())
                {
                    sTemp = "0";
                }

                if (bPtr->kind() == Builtin::KindLong)
                {
                    switch (_iLongType)
                    {
                        case CodeGenerator::String:
                        {
                            sTemp = "\"" + sTemp + "\"";
                            break;
                        }
                        case CodeGenerator::BigInt:
                        {
                            sTemp = sTemp + "n";
                            break;
                        }
                    }
                }

                return sTemp;
            }
            case Builtin::KindFloat    : // [[fallthrough]]
            case Builtin::KindDouble   :
                return sDefault.empty()?"0.0":sDefault;
            default                    :
                assert(false);
        }
    }

    EnumPtr ePtr = EnumPtr::dynamicCast(pPtr->getTypePtr());
    if (ePtr)
    {
        if (sDefault.empty())
        {
            vector<TypeIdPtr>& eMember = ePtr->getAllMemberPtr();
            if (eMember.size() > 0)
            {
                vector<string> vecNames = TC_Common::sepstr<string>(ePtr->getSid(), "::");

                string sModule = findName(vecNames[0], eMember[0]->getId());

                return sModule;
            }
        }

        if (!TC_Common::isdigit(sDefault))
        {
            string s1 = sNamespace;
            string s2 = sDefault;

            string::size_type index = sDefault.find("::");
            if (index != string::npos)
            {
                s1 = sDefault.substr(0, index);
                s2 = sDefault.substr(index + 2);
            }

            string sModule = findName(s1, s2);

            assert(!sModule.empty());

            return sModule;
        }

        return sDefault;
    }

    if (bGlobal)
    {
        return "new " + getDataType(pPtr->getTypePtr(), bCastEnumAsAny);
    }

    return sDefault;
}