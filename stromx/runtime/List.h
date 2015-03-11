/* 
*  Copyright 2015 Matthias Fuchs
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

#ifndef STROMX_RUNTIME_LIST_H
#define STROMX_RUNTIME_LIST_H

#include <vector>
#include "stromx/runtime/Data.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief %List of data values. */
        class STROMX_RUNTIME_API List : public Data
        {            
        public:
            /** Constructs an empty list. */
            List() {}
            explicit List(const std::vector<Data*> & content) : m_content(content) {}
            ~List();
            
            virtual const std::string & type() const { return TYPE; }
            virtual const Version & version() const { return VERSION; }
            virtual const std::string & package() const { return PACKAGE; }
            
            virtual const VariantHandle & variant() const;
            
            virtual Data* clone() const;
            
            /** Provides write access to the content of the list. */
            std::vector<Data*> & content() { return m_content; }
            
            /** Provides write access to the content of the list. */
            std::vector<const Data*> content() const
            {
                return std::vector<const Data*>(m_content.begin(), m_content.end()); 
            }
            
        private:
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const Version VERSION;
            
            std::vector<Data*> m_content;
        };  
        
        /** \cond */
        template <>
        class data_traits<List>
        {
        public:
            static const VariantHandle & variant() { return Variant::LIST; }
        };  
        /** \endcond */
        
        template <class T>
        class TypedList : public List
        {
        public:
            template <class src_t>
            TypedList(const std::vector<src_t> & srcContent) : List()
            {
                content().reserve(srcContent.size());
                for (typename std::vector<src_t>::const_iterator iter = srcContent.begin();
                     iter != srcContent.end(); ++iter)
                {
                    src_t & src = const_cast<src_t&>(*iter);
                    content().push_back(new T(src));
                }
            }
        };
    }
}


    
#endif // STROMX_RUNTIME_LIST_H
