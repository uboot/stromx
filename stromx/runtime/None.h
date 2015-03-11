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

#ifndef STROMX_RUNTIME_NONE_H
#define STROMX_RUNTIME_NONE_H

#include "stromx/runtime/Data.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief Undefined and empty data object. */
        class STROMX_RUNTIME_API None : public Data
        {
        public:
            None() {}
            
            virtual const Version & version() const { return VERSION; }
            virtual const std::string & type() const { return TYPE; }
            virtual const std::string & package() const { return PACKAGE; }
            
            virtual const VariantHandle & variant() const { return Variant::NONE; }
            
            virtual Data* clone() const { return new None; }
            
        private:
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const Version VERSION;
        };     
        
        /** \cond */
        template <>
        class data_traits<None>
        {
        public:
            static const VariantHandle & variant() { return Variant::NONE; }
        };
        /** \endcond */
    }
}

#endif // STROMX_RUNTIME_NONE_H
