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

#include "tars2case.h"
#include "util/tc_md5.h"
#include "util/tc_file.h"
#include "util/tc_common.h"
#include <string>

//////////////////////////////////////////////////////////////////////////////////
//
Tars2Case::Tars2Case() : _jsonCase(false), _webCase(false)
{
}

string Tars2Case::toDescStr(const TypePtr &pPtr) const
{
	BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr);
	if (bPtr)
	{
		switch (bPtr->kind())
		{
			case Builtin::KindByte:   return bPtr->isUnsigned() ? "byte_u" : "byte"; break;
			case Builtin::KindShort:  return bPtr->isUnsigned() ? "short_u" : "short"; break;
			case Builtin::KindInt:    return bPtr->isUnsigned() ? "int_u" : "int"; break;
			case Builtin::KindLong:   return "long";  break;
			case Builtin::KindBool:   return "bool"; break;
			case Builtin::KindFloat:  return "float"; break;
			case Builtin::KindDouble: return "double"; break;
			case Builtin::KindString: return "string"; break;
			default: assert(false); break;
		}
	}

	VectorPtr vPtr = VectorPtr::dynamicCast(pPtr);
	if (vPtr) return "vector";

	MapPtr mPtr = MapPtr::dynamicCast(pPtr);
	if (mPtr) return "map";

	StructPtr sPtr = StructPtr::dynamicCast(pPtr);
	if (sPtr) return "struct";

	EnumPtr ePtr = EnumPtr::dynamicCast(pPtr);
	if (ePtr) return "int";

	return "void";
}


string Tars2Case::toStr(const TypePtr &pPtr) const
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

string Tars2Case::toCase(const TypePtr &pPtr, const string& varName) const
{
	BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr);
	if (bPtr) return tocaseBuiltin(bPtr, varName);

	VectorPtr vPtr = VectorPtr::dynamicCast(pPtr);
	if (vPtr) return tocaseVector(vPtr, varName);

	MapPtr mPtr = MapPtr::dynamicCast(pPtr);
	if (mPtr) return tocaseMap(mPtr, varName);

	StructPtr sPtr = StructPtr::dynamicCast(pPtr);
	if (sPtr) return tocaseStruct(sPtr, varName);

	EnumPtr ePtr = EnumPtr::dynamicCast(pPtr);
	if (ePtr) return tocaseEnum(ePtr, varName);

	if (!pPtr) return "void";

	assert(false);
	return "";
}


/*******************************BuiltinPtr********************************/
string Tars2Case::tostrBuiltin(const BuiltinPtr &pPtr) const
{
	string s;

	switch (pPtr->kind())
	{
	case Builtin::KindBool:
		s = "bool";
		break;
	case Builtin::KindByte:
        s = pPtr->isUnsigned() ? "short" : "byte";
        break;
    case Builtin::KindShort:
        s = pPtr->isUnsigned() ? "int" : "short";
        break;
    case Builtin::KindInt:
        s = pPtr->isUnsigned() ? "long" : "int";
        break;
	case Builtin::KindLong:
		s = "long";
		break;
	case Builtin::KindFloat:
		s = "float";
		break;
	case Builtin::KindDouble:
		s = "double";
		break;
	case Builtin::KindString:
		s = "string";
		break;
	case Builtin::KindVector:
		s = "vector";
		break;
	case Builtin::KindMap:
		s = "map";
		break;
	default:
		assert(false);
		break;
	}

	return s;
}

string Tars2Case::tocaseBuiltin(const BuiltinPtr &pPtr, const string& varName) const
{
	string s;

	switch (pPtr->kind())
	{
	case Builtin::KindBool:
		s = "true";
		break;
	case Builtin::KindByte:
		s = "0";
		break;
	case Builtin::KindShort:
		s = "0";
		break;
	case Builtin::KindInt:
		s = "0";
		break;
	case Builtin::KindLong:
		s = "0";
		break;
	case Builtin::KindFloat:
		s = "0.0";
		break;
	case Builtin::KindDouble:
		s = "0.00";
		break;
	case Builtin::KindString:
		s = "key";
		break;
/*
	case Builtin::KindVector:
		s = "vector";
		break;
	case Builtin::KindMap:
		s = "map";
		break;
*/
	default:
		assert(false);
		break;
	}

	return s;
}
/*******************************VectorPtr********************************/
string Tars2Case::tostrVector(const VectorPtr &pPtr) const
{
	string s;
	BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr->getTypePtr());
	s = "vector" + string("<") + toStr(pPtr->getTypePtr()) + ">";
	return s;
}

string Tars2Case::tocaseVector(const VectorPtr &pPtr, const string& varName) const
{
	string s;
	BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr->getTypePtr());
	s = string("<");
	for(int i = 0; i < 2; i++) {
		if(i != 0) {
			s += ",";
		}
		s += toCase(pPtr->getTypePtr(), varName);
	}
	s += ">";
	return s;
}

string Tars2Case::tostrMap(const MapPtr &pPtr, bool bNew) const
{
	string s = "map";
	s += string("<") + toStr(pPtr->getLeftTypePtr()) + "," + toStr(pPtr->getRightTypePtr()) + ">";

	return s;
}

string Tars2Case::tocaseMap(const MapPtr &pPtr, const string& varName) const
{
	string s = string("[");
	for(int i = 0; i < 2; i++) {
		if(i != 0) {
			s += ",";
		}
		s +=  toCase(pPtr->getLeftTypePtr(), varName) + "=" + toCase(pPtr->getRightTypePtr(), varName);
	}
	s += "]";

	return s;
}


string Tars2Case::tostrStruct(const StructPtr &pPtr) const
{
	vector<TypeIdPtr>& members = pPtr->getAllMemberPtr();
	string s;

	//是否生成tag和require等信息
	bool genTag = false;
	for(size_t i = 0; i < members.size(); i++) {
		TypeIdPtr typeId = members[i];
		if(!s.empty()) {
			s += ",";
		}

		if(i == 0) {
			if(typeId->getTag() == 0) {
				genTag = false;
			}
			else {
				genTag = true;
			}
		}
		else {
			//检查tag是否连续
			if(typeId->getTag() == members[i-1]->getTag() + 1) {
				genTag = false;
			}
			else {
				genTag = true;
			}
		}

		if(genTag) {
			s+=genratePrefix(typeId) + " ";
		}


		s += toStr(typeId->getTypePtr());
	}
	return "struct<" + s + ">";
}

string Tars2Case::tocaseStruct(const StructPtr &pPtr, const string& varName) const
{
	vector<TypeIdPtr>& members = pPtr->getAllMemberPtr();
	string s = string("<");

	for(size_t i = 0; i < members.size(); i++) {
		if(i != 0) {
			s += ",";
		}
		s += toCase(members[i]->getTypePtr(), varName);
	}
	s += ">";
	return s;
}

string Tars2Case::genratePrefix(const TypeIdPtr &ptr) const
{
	string s = TC_Common::tostr(ptr->getTag()) + " ";
	if(ptr->isRequire()) {
		s += "require";
	}
	else {
		s += "optional";
	}
	return s;
}

string Tars2Case::tostrEnum(const EnumPtr &pPtr) const
{
	return "int";
}

string Tars2Case::tocaseEnum(const EnumPtr &pPtr, const string& varName) const
{
	return "19";
}


string Tars2Case::generateTest(const ParamDeclPtr &pPtr) const
{
	ostringstream s;

	TypePtr typePtr = pPtr->getTypeIdPtr()->getTypePtr();
	s << toStr(typePtr);
	return s.str();
}

string Tars2Case::generateCase(const ParamDeclPtr &pPtr) const
{
	ostringstream s;

	TypePtr typePtr = pPtr->getTypeIdPtr()->getTypePtr();
	string varName = pPtr->getTypeIdPtr()->getId();
	s << toCase(typePtr, varName);
	return s.str();
}

struct SortOperation
{
	bool operator()(const OperationPtr &o1, const OperationPtr &o2)
	{
		return o1->getId() < o2->getId();
	}
};


string Tars2Case::generateTest(const InterfacePtr &pPtr, const string& outfile) const
{
	ostringstream s;
	vector<OperationPtr>& vOperation = pPtr->getAllOperationPtr();
	std::sort(vOperation.begin(), vOperation.end(), SortOperation());

	//生成客户端接口的声明
	TC_File::makeDirRecursive(_baseDir);
	for (size_t i = 0; i < vOperation.size(); i++)
	{
		string sCase = "";
		string sInParams = "";
		string sOutParams = "";

		vector<ParamDeclPtr>& vParamDecl = vOperation[i]->getAllParamDeclPtr();
		for (size_t j = 0; j < vParamDecl.size(); j++)
		{
			if(vParamDecl[j]->isOut()) {
				sOutParams += (sOutParams.empty() ? "" : "|") + generateTest(vParamDecl[j]);
			}
			else {
				sCase += (sCase.empty() ? "" : "\r\n") + generateCase(vParamDecl[j]);
				sInParams += (sInParams.empty() ? "" : "|") + generateTest(vParamDecl[j]);
			}
		}

		ostringstream sc;
		sc << sInParams << endl << "#### 隔离符 ####" << endl << sCase << endl;
		string filecase  = _baseDir + "/" + vOperation[i]->getId() + ".case";
		TC_File::save2file(filecase, sc.str());
	}
	return s.str();
}

JsonValuePtr Tars2Case::toJsonBuiltin(const BuiltinPtr &pPtr) const
{
    JsonValuePtr p;
	switch (pPtr->kind())
	{
	case Builtin::KindBool:
		p = JsonOutput::writeJson(true);
		break;
	case Builtin::KindByte:
		p = JsonOutput::writeJson(8);
		break;
	case Builtin::KindShort:
		p = JsonOutput::writeJson(16);
		break;
	case Builtin::KindInt:
		p = JsonOutput::writeJson(32);
		break;
	case Builtin::KindLong:
        p = JsonOutput::writeJson(64);
		break;
	case Builtin::KindFloat:
		p = JsonOutput::writeJson(1.01);
		break;
	case Builtin::KindDouble:
		p = JsonOutput::writeJson((double)2.0000005);
		break;
	case Builtin::KindString:
		p = JsonOutput::writeJson(string("string"));
		break;
	default:
		assert(false);
		break;
	}
	return p;
}

JsonValuePtr Tars2Case::toJsonEnum(const EnumPtr &pPtr) const
{
    return JsonOutput::writeJson(string("int32"));
}

JsonValueArrayPtr Tars2Case::toJsonVector(const VectorPtr &pPtr) const
{
    JsonValueArrayPtr p = new JsonValueArray();

    JsonValuePtr p1 = generateJson(pPtr->getTypePtr());
    p->push_back(p1);
    p->push_back(p1);
	return p;
}

JsonValueObjPtr Tars2Case::toJsonMap(const MapPtr &pPtr) const
{
    JsonValueObjPtr p = new JsonValueObj();
    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr->getLeftTypePtr());
	if (bPtr)
    {
        p->value["key"] = generateJson(pPtr->getRightTypePtr());
        // p->value[tocaseBuiltin(bPtr, "")] = generateJson(pPtr->getRightTypePtr());
    }
	return p;
}

JsonValueObjPtr Tars2Case::toJsonStruct(const StructPtr &pPtr) const
{
    JsonValueObjPtr p = new JsonValueObj();
	vector<TypeIdPtr>& members = pPtr->getAllMemberPtr();

	//是否生成tag和require等信息
	for(size_t i = 0; i < members.size(); i++)
    {
        TypeIdPtr typeId = members[i];
        string keyName = typeId->getId();
        p->value[keyName] = generateJson(typeId->getTypePtr());
	}
	return p;
}


JsonValuePtr Tars2Case::generateJson(const TypePtr &pPtr) const
{
	BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr);
	if (bPtr) return toJsonBuiltin(bPtr);

	VectorPtr vPtr = VectorPtr::dynamicCast(pPtr);
	if (vPtr) return toJsonVector(vPtr);

	MapPtr mPtr = MapPtr::dynamicCast(pPtr);
	if (mPtr) return toJsonMap(mPtr);

	StructPtr sPtr = StructPtr::dynamicCast(pPtr);
	if (sPtr) return toJsonStruct(sPtr);

	EnumPtr ePtr = EnumPtr::dynamicCast(pPtr);
	if (ePtr) return toJsonEnum(ePtr);

    assert(false);
	return NULL;
}

JsonValuePtr Tars2Case::toJsonDescEnum(const EnumPtr &pPtr) const
{
    return JsonOutput::writeJson(string("0"));
}

JsonValuePtr Tars2Case::toJsonDescVector(const VectorPtr &pPtr) const
{
    JsonValueObjPtr p = new JsonValueObj();
	string name = "0_vector_" + toDescStr(pPtr->getTypePtr());
	p->value[name] = generateJsonDesc(pPtr->getTypePtr());
	return p;
}

JsonValuePtr Tars2Case::toJsonDescMap(const MapPtr &pPtr) const
{
    string name;
    JsonValueObjPtr p = new JsonValueObj();
	name = "0_map_" + toDescStr(pPtr->getLeftTypePtr());
	p->value[name] = generateJsonDesc(pPtr->getLeftTypePtr());
	name = "1_map_" + toDescStr(pPtr->getRightTypePtr());
	p->value[name] = generateJsonDesc(pPtr->getRightTypePtr());
	return p;
}

JsonValuePtr Tars2Case::toJsonDescStruct(const StructPtr &pPtr) const
{
    JsonValueObjPtr p = new JsonValueObj();
	vector<TypeIdPtr>& members = pPtr->getAllMemberPtr();

	//是否生成tag和require等信息
	for(size_t i = 0; i < members.size(); i++)
    {
		ostringstream osd;
        osd << members[i]->getTag() << "_" << members[i]->getId() << "_" << toDescStr(members[i]->getTypePtr());
        p->value[osd.str()] = generateJsonDesc(members[i]);
	}
	return p;
}

JsonValuePtr Tars2Case::generateJsonDesc(const TypePtr &pPtr) const
{
	BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr);
	if (bPtr) return JsonOutput::writeJson(bPtr->def());

	VectorPtr vPtr = VectorPtr::dynamicCast(pPtr);
	if (vPtr) return toJsonDescVector(vPtr);

	MapPtr mPtr = MapPtr::dynamicCast(pPtr);
	if (mPtr) return toJsonDescMap(mPtr);

	StructPtr sPtr = StructPtr::dynamicCast(pPtr);
	if (sPtr) return toJsonDescStruct(sPtr);

	EnumPtr ePtr = EnumPtr::dynamicCast(pPtr);
	if (ePtr) return toJsonDescEnum(ePtr);

    assert(false);
	return NULL;
}

JsonValuePtr Tars2Case::generateJsonDesc(const TypeIdPtr &pPtr) const
{
	BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr->getTypePtr());
	if (bPtr) return JsonOutput::writeJson(pPtr->hasDefault() ? pPtr->def() : bPtr->def());
	return generateJsonDesc(pPtr->getTypePtr());
}

string Tars2Case::generateDesc(const InterfacePtr &pPtr, const string& outfile) const
{
	ostringstream s;
	vector<OperationPtr>& vOperation = pPtr->getAllOperationPtr();
	std::sort(vOperation.begin(), vOperation.end(), SortOperation());

	//生成客户端接口的声明
	TC_File::makeDirRecursive(_baseDir);
	for (size_t i = 0; i < vOperation.size(); i++)
	{
		JsonValueObjPtr inDesc  = new JsonValueObj();
		JsonValueObjPtr inParam = new JsonValueObj();

		vector<ParamDeclPtr>& vParamDecl = vOperation[i]->getAllParamDeclPtr();
		for (size_t j = 0; j < vParamDecl.size(); j++)
		{
			ostringstream osd;
			TypeIdPtr& p = vParamDecl[j]->getTypeIdPtr();
			osd << p->getTag() << "_" << p->getId() << "_" << toDescStr(p->getTypePtr());
			if(!vParamDecl[j]->isOut())
            {
				inDesc->value[osd.str()]   = generateJsonDesc(p);
                inParam->value[p->getId()] = generateJson(vParamDecl[j]->getTypeIdPtr()->getTypePtr());
			}
		}

		string filedesc  = _baseDir + "/" + vOperation[i]->getId() + ".desc";
		string filecase  = _baseDir + "/" + vOperation[i]->getId() + ".case";
		TC_File::save2file(filedesc, TC_Json::writeValue(inDesc));
		TC_File::save2file(filecase, TC_Json::writeValue(inParam));
	}
	return s.str();
}

string Tars2Case::generateJson(const InterfacePtr &pPtr, const string& outfile) const
{
	ostringstream s;

	vector<OperationPtr>& vOperation = pPtr->getAllOperationPtr();
	std::sort(vOperation.begin(), vOperation.end(), SortOperation());

	// 生成客户端接口的声明
	JsonValueObjPtr v = new JsonValueObj();
	// JsonValueArrayPtr v = new JsonValueArray();
	for (size_t i = 0; i < vOperation.size(); i++)
	{
		string intCaseValues = "";
		string intCaseParams = "";
		string outCaseParams = "";

		JsonValueObjPtr inDesc  = new JsonValueObj();
		JsonValueObjPtr outDesc = new JsonValueObj();
		JsonValueObjPtr inParam  = new JsonValueObj();
		JsonValueObjPtr outParam = new JsonValueObj();

		vector<ParamDeclPtr>& vParamDecl = vOperation[i]->getAllParamDeclPtr();
		for (size_t j = 0; j < vParamDecl.size(); j++)
		{
			ostringstream osd;
			TypeIdPtr& p = vParamDecl[j]->getTypeIdPtr();
			osd << p->getTag() << "_" << p->getId() << "_" << toDescStr(p->getTypePtr());

			if(vParamDecl[j]->isOut())
            {
				outCaseParams += (outCaseParams.empty() ? "" : "|") + generateTest(vParamDecl[j]);
				outDesc->value[osd.str()]   = generateJsonDesc(p->getTypePtr());
                outParam->value[p->getId()] = generateJson(p->getTypePtr());
			}
			else
            {
				intCaseValues += (intCaseValues.empty() ? "" : "<br>") + generateCase(vParamDecl[j]);
				intCaseParams += (intCaseParams.empty() ? "" : "|") + generateTest(vParamDecl[j]);
				inDesc->value[osd.str()]   = generateJsonDesc(p->getTypePtr());
                inParam->value[p->getId()] = generateJson(vParamDecl[j]->getTypeIdPtr()->getTypePtr());
			}
		}

		JsonValueObjPtr p = new JsonValueObj();
		p->value["funinput"]   = inParam;
		p->value["funoutput"]  = outParam;
		p->value["descinput"]  = inDesc;
		p->value["descoutput"] = outDesc;
		p->value["rettype"]    = JsonOutput::writeJson(toStr(vOperation[i]->getReturnPtr()->getTypePtr()));
		p->value["funintpara"] = JsonOutput::writeJson(intCaseParams);
		p->value["funintvals"] = JsonOutput::writeJson(intCaseValues);
		p->value["funoutpara"] = JsonOutput::writeJson(outCaseParams);
		v->value[vOperation[i]->getId()] = JsonValuePtr::dynamicCast(p);
	}

	if (_baseDir.empty())
	{
		cout << TC_Json::writeValue(v) << endl;
	}
	else
	{
		string filetest  = _baseDir + "/" + (outfile.empty() ? pPtr->getId() : outfile) + ".json";
		TC_File::makeDirRecursive(_baseDir);
		TC_File::save2file(filetest, TC_Json::writeValue(v));
	}
	return TC_Json::writeValue(v);
}

/******************************Tars2Case***************************************/
void Tars2Case::generateFile(const ContextPtr &pPtr, const string& outfile) const
{
	ostringstream s;

	vector<NamespacePtr> namespaces = pPtr->getNamespaces();
	for (size_t i = 0; i < namespaces.size(); i++)
	{
		vector<InterfacePtr>  &is  = namespaces[i]->getAllInterfacePtr();
		for (size_t j = 0; j < is.size(); j++)
		{
			if (_webCase)
			{
				// 生成TarsWeb需要的用例
            	generateJson(is[j], outfile);
			}
			else if (_jsonCase)
			{
				generateDesc(is[j], outfile);
			}
			else
			{
				// 生成Tars压测用例的文件
				generateTest(is[j], outfile);
			}
		}
	}
}

void Tars2Case::createFile(const string &tarsfile, const string& outfile)
{
	std::vector<ContextPtr> contexts = g_parse->getContexts();
	for (size_t i = 0; i < contexts.size(); i++)
	{
		if (tarsfile == contexts[i]->getFileName())
		{
			generateFile(contexts[i], outfile);
		}
	}
}

void Tars2Case::setBaseDir(const string &dir)
{
	_baseDir = dir;
}

void Tars2Case::setJsonCase(bool jsonCase)
{
	_jsonCase = jsonCase;
}

void Tars2Case::setWebSupport(bool webCase)
{
	_webCase = webCase;
}

string Tars2Case::getFilePath(const string &ns) const
{
	return _baseDir + "/" + ns + "/";
}
