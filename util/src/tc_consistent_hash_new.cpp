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

#include "util/tc_consistent_hash_new.h"


namespace tars
{

int32_t TC_KetamaHashAlg::hash(const char* sKey, size_t length)
{
    vector<char> sMd5 = TC_MD5::md5bin(sKey, length);
    const char *p = (const char *) sMd5.data();

    int32_t hash = ((int32_t)(p[3] & 0xFF) << 24)
        | ((int32_t)(p[2] & 0xFF) << 16)
        | ((int32_t)(p[1] & 0xFF) << 8)
        | ((int32_t)(p[0] & 0xFF));

    return subTo32Bit(hash);
}

TC_HashAlgorithmType TC_KetamaHashAlg::getHashType()
{
    return E_TC_CONHASH_KETAMAHASH;
}

int32_t TC_DefaultHashAlg::hash(const char* sKey, size_t length)
{
	vector<char> sMd5 = TC_MD5::md5bin(sKey, length);
    const char *p = (const char *) sMd5.data();

    int32_t hash = (*(int*)(p)) ^ (*(int*)(p+4)) ^ (*(int*)(p+8)) ^ (*(int*)(p+12));

    return subTo32Bit(hash);
}

TC_HashAlgorithmType TC_DefaultHashAlg::getHashType()
{
    return E_TC_CONHASH_DEFAULTHASH;
}

TC_HashAlgorithm *TC_HashAlgFactory::getHashAlg(TC_HashAlgorithmType hashType)
{
    TC_HashAlgorithm *ptrHashAlg = NULL;

    switch(hashType)
    {
        case E_TC_CONHASH_KETAMAHASH:
        {
            ptrHashAlg = new TC_KetamaHashAlg();
            break;
        }
        case E_TC_CONHASH_DEFAULTHASH:
        default:
        {
            ptrHashAlg = new TC_DefaultHashAlg();
            break;
        }
    }

    return ptrHashAlg;
}

TC_ConsistentHashNew::TC_ConsistentHashNew()
{
    _ptrHashAlg = TC_HashAlgFactory::getHashAlg(E_TC_CONHASH_DEFAULTHASH);
}

TC_ConsistentHashNew::TC_ConsistentHashNew(TC_HashAlgorithmType hashType)
{
    _ptrHashAlg = TC_HashAlgFactory::getHashAlg(hashType);
}

/**
 * @brief 节点比较.
 *
 * @param m1 node_T_new类型的对象，比较节点之一
 * @param m2 node_T_new类型的对象，比较节点之一
 * @return less or not 比较结果，less返回ture，否则返回false
 */
static bool less_hash(const TC_ConsistentHashNew::node_T_new & m1, const TC_ConsistentHashNew::node_T_new & m2)
{
    return m1.iHashCode < m2.iHashCode;
}

void TC_ConsistentHashNew::sortNode()
{
    sort(_vHashList.begin(), _vHashList.end(), less_hash);
}

void TC_ConsistentHashNew::printNode()
{
    map<unsigned int, unsigned int> mapNode;
    size_t size = _vHashList.size();

    for (size_t i = 0; i < size; i++)
    {
        if (i == 0)
        {
            unsigned int value = 0xFFFFFFFF - _vHashList[size - 1].iHashCode + _vHashList[0].iHashCode;
            mapNode[_vHashList[0].iIndex] = value;
        }
        else
        {
            unsigned int value = _vHashList[i].iHashCode - _vHashList[i - 1].iHashCode;

            if (mapNode.find(_vHashList[i].iIndex) != mapNode.end())
            {
                value += mapNode[_vHashList[i].iIndex];
            }

            mapNode[_vHashList[i].iIndex] = value;
        }

        cout << "printNode: " << _vHashList[i].iHashCode << "|" << _vHashList[i].iIndex << "|" << mapNode[_vHashList[i].iIndex] << endl;
    }

    map<unsigned int, unsigned int>::iterator it = mapNode.begin();
    double avg = 100;
    double sum = 0;

    while (it != mapNode.end())
    {
        double tmp = it->second;
        cerr << "result: " << it->first << "|" << it->second << "|" << (tmp * 100 * mapNode.size() / 0xFFFFFFFF - avg) << endl;
        sum += (tmp * 100 * mapNode.size() / 0xFFFFFFFF - avg) * (tmp * 100 * mapNode.size() / 0xFFFFFFFF - avg);
        it++;
    }

    cerr << "variance: " << sum / mapNode.size() << ", size: " << _vHashList.size() << endl;
}

int TC_ConsistentHashNew::addNode(const string & node, unsigned int index, int weight)
{
    if (_ptrHashAlg.get() == NULL)
    {
        return -1;
    }

    node_T_new stItem;
    stItem.iIndex = index;

    for (int j = 0; j < weight; j++)
    {
        string virtualNode = node + "_" + TC_Common::tostr<int>(j);

        // TODO: 目前写了2 种hash 算法，可以根据需要选择一种，
        // TODO: 其中KEMATA 为参考memcached client 的hash 算法，default 为原有的hash 算法，测试结论在表格里有
        if (_ptrHashAlg->getHashType() == E_TC_CONHASH_KETAMAHASH)
        {
	        vector<char> sMd5 = TC_MD5::md5bin(virtualNode);
            char *p = (char *) sMd5.data();

            for (int i = 0; i < 4; i++)
            {
                stItem.iHashCode = ((int32_t)(p[i * 4 + 3] & 0xFF) << 24)
                    | ((int32_t)(p[i * 4 + 2] & 0xFF) << 16)
                    | ((int32_t)(p[i * 4 + 1] & 0xFF) << 8)
                    | ((int32_t)(p[i * 4 + 0] & 0xFF));
                stItem.iIndex = index;
                _vHashList.push_back(stItem);
            }
        }
        else
        {
            stItem.iHashCode = _ptrHashAlg->hash(virtualNode.c_str(), virtualNode.length());
            _vHashList.push_back(stItem);
        }
    }

    return 0;
}

int TC_ConsistentHashNew::getIndex(const string & key, unsigned int & iIndex)
{
    if(_ptrHashAlg.get() == NULL || _vHashList.size() == 0)
    {
        iIndex = 0;
        return -1;
    }

    vector<char> data = TC_MD5::md5bin(key);
    int32_t iCode = _ptrHashAlg->hash(data.data(), data.size());

    return getIndex(iCode, iIndex);
}


int TC_ConsistentHashNew::getIndex(int32_t hashcode, unsigned int & iIndex)
{
    if(_ptrHashAlg.get() == NULL || _vHashList.size() == 0)
    {
        iIndex = 0;
        return -1;
    }

    // 只保留32位
    int32_t iCode = (hashcode & 0xFFFFFFFFL);

    int low = 0;
    int high = (int)_vHashList.size();

    if(iCode <= _vHashList[0].iHashCode || iCode > _vHashList[high-1].iHashCode)
    {
        iIndex = _vHashList[0].iIndex;
        return 0;
    }


    while (low < high - 1)
    {
        int mid = (low + high) / 2;
        if (_vHashList[mid].iHashCode > iCode)
        {
            high = mid;
        }
        else
        {
            low = mid;
        }
    }
    iIndex = _vHashList[low+1].iIndex;
    return 0;
}

}
