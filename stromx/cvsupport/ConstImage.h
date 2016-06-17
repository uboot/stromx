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

#ifndef STROMX_CVSUPPORT_CONSTIMAGE_H
#define STROMX_CVSUPPORT_CONSTIMAGE_H

#include "stromx/cvsupport/Config.h"
#include <stromx/runtime/Enum.h>
#include <stromx/runtime/Image.h>
#include <stromx/runtime/OperatorKernel.h>
#include <stromx/runtime/RecycleAccess.h>

namespace stromx
{
    namespace runtime
    {
        class Image;
    }

    namespace cvsupport
    {
        /** \brief Outputs a configurable constant image. */
        class STROMX_CVSUPPORT_API ConstImage : public runtime::OperatorKernel
        {
        public:
            enum DataId
            {
                OUTPUT,
                IMAGE
            };
            
            ConstImage();
            virtual ~ConstImage();
            
            virtual OperatorKernel* clone() const { return new ConstImage; }
            virtual void setParameter(const unsigned int id, const runtime::Data& value);
            virtual const runtime::DataRef getParameter(const unsigned int id) const;
            virtual void execute(runtime::DataProvider& provider);
            
        private:
            static const std::vector<const runtime::Description*> setupInputs();
            static const std::vector<const runtime::Description*> setupOutputs();
            static const std::vector<const runtime::Parameter*> setupParameters();
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const runtime::Version VERSION;   
            
            runtime::Image* m_image;
            runtime::RecycleAccess m_imageAccess;
        };
    }
}

#endif // STROMX_CVSUPPORT_CONSTIMAGE_H
