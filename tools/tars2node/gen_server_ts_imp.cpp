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

void CodeGenerator::generateTSServerImp(const ContextPtr &cPtr)
{
    string sFileName = TC_File::excludeFileExt(_sToPath + TC_File::extractFileName(cPtr->getFileName())) + "Imp.ts";
    if (TC_File::isFileExist(sFileName))
    {
        return ;
    }

    ostringstream str;
    str << printHeaderRemark("Imp");

    vector<NamespacePtr> namespaces = cPtr->getNamespaces();

    // generate the server implementation class
    ostringstream estr;
    set<string> setInterface;
    for(size_t i = 0; i < namespaces.size(); i++)
    {
        estr << "export namespace " << namespaces[i]->getId() << " {" << endl;
        INC_TAB;

        vector<InterfacePtr> & is = namespaces[i]->getAllInterfacePtr();
        for (size_t ii = 0; ii < is.size(); ii++)
        {
            if (setInterface.count(namespaces[i]->getId() + "::" + is[ii]->getId()) != 0)
            {
                continue;
            }
            setInterface.insert(namespaces[i]->getId() + "::" + is[ii]->getId());

            estr << TAB << "export class " << is[ii]->getId() << "Imp extends base." << namespaces[i]->getId() << "." << is[ii]->getId() << "Imp { " << endl;
            INC_TAB;

            estr << TAB << "initialize() {" << endl;
            INC_TAB;
            estr << TAB << "// TODO: implement initialize" << endl;
            DEL_TAB;
            estr << TAB << "}" << endl << endl;

            vector<OperationPtr> & vOperation = is[ii]->getAllOperationPtr();
            for (size_t iii = 0; iii < vOperation.size(); iii++)
            {
                const OperationPtr &oPtr = vOperation[iii];

                // generate function entries
                estr << TAB << oPtr->getId() << "(current: base." << namespaces[i]->getId() << "." << is[ii]->getId() << "Imp." << oPtr->getId() << "Current";

                vector<ParamDeclPtr> &vParamDecl = oPtr->getAllParamDeclPtr();
                for (size_t j = 0; j < vParamDecl.size(); j++)
                {
                    estr << ", " << vParamDecl[j]->getTypeIdPtr()->getId() << ": " << getTsType(vParamDecl[j]->getTypeIdPtr()->getTypePtr(), true, true);
                }
                estr << ") { " << endl;

                INC_TAB;
                estr << TAB << "// TODO: implement " << oPtr->getId() << "" << endl;

                DEL_TAB;
                estr << TAB << "}" << endl;
                if (iii != vOperation.size() - 1) estr << endl;
            }

            DEL_TAB;
            estr << TAB << "}" << endl;
            if (ii != is.size() - 1) estr << endl;
        }

        DEL_TAB;
        estr << "}" << endl;
        if (i != namespaces.size() - 1) estr << endl;
    }

    // generate module imports
    str << "import * as " << IDL_NAMESPACE_STR << "Stream from \"" << _sStreamPath << "\";" << endl;
    str << "import * as base from \"./"
        << TC_File::excludeFileExt(TC_File::extractFileName(cPtr->getFileName())) << "\";" << endl;

    for (map<string, ImportFile>::iterator it = _mapFiles.begin(); it != _mapFiles.end(); it++)
    {
        if (it->second.sModule.empty()) continue;

        if (estr.str().find(it->second.sModule + ".") == string::npos) continue;

        str << "import * as " << it->second.sModule << " from \"" << TC_File::excludeFileExt(it->second.sFile) << "\";" << endl;
    }

    str << endl;
    str << estr.str();

    TC_File::makeDirRecursive(_sToPath);
    makeUTF8File(sFileName, str.str());
}
