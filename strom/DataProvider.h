/* 
 *  Copyright 2011 Matthias Fuchs
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef STREAM_DATAPROVIDER_H
#define STREAM_DATAPROVIDER_H

namespace strom
{
    class Data;
    class DataContainer;
    class Id2DataMapper;
  
    class DataProvider
    {
    public:
        virtual ~DataProvider() {}
        
        virtual void testForInterrupt() = 0;
        virtual void sleep(const unsigned int microseconds) = 0;
        
        virtual void receiveInputData(const Id2DataMapper& mapper) = 0;
        virtual void sendOutputData(const Id2DataMapper& mapper) = 0;
    };
}

#endif // STREAM_DATAPROVIDER_H