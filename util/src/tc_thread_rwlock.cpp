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

#include "util/tc_thread_rwlock.h"
#include <string.h>
#include <iostream>
#include <cassert>

namespace tars
{

TC_ThreadRWLocker::TC_ThreadRWLocker()
{
}

TC_ThreadRWLocker::~TC_ThreadRWLocker()
{
}

void TC_ThreadRWLocker::readLock() const
{
	_mutex.readLock();
}


void TC_ThreadRWLocker::writeLock() const
{
	_mutex.writeLock();
}

bool TC_ThreadRWLocker::tryReadLock() const
{
	return _mutex.tryReadLock();	
}

bool TC_ThreadRWLocker::tryWriteLock() const
{
	return _mutex.tryWriteLock();	
}

void TC_ThreadRWLocker::unReadLock() const
{
	_mutex.unReadLock();	
}

void TC_ThreadRWLocker::unWriteLock() const
{
	_mutex.unWriteLock();	
}

}


