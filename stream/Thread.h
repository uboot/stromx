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

#ifndef STREAM_THREADINTERFACE_H
#define STREAM_THREADINTERFACE_H

namespace stream
{
    class InputNode;
    
    class Thread
    {      
    public:
        enum Status
        {
            INACTIVE,
            ACTIVE,
            DEACTIVATING,
        };
        
        virtual ~Thread() {}
        virtual const Status status() const = 0;
        
        virtual const std::string & name() const = 0;
        virtual void setName(const std::string& name) = 0;
        virtual const std::vector<InputNode*> & nodeSequence() const = 0;
        
        virtual void addNode(InputNode* const op) = 0;
        virtual void insertNode(const unsigned int position, InputNode* const op) = 0;
        virtual void removeNode(const unsigned int position) = 0;
    };
}

#endif // STREAM_THREADINTERFACE_H
