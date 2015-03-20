/* 
*  Copyright 2012 Matthias Fuchs
*
*  This file is part of stromx-studio.
*
*  Stromx-studio is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Stromx-studio is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with stromx-studio.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STROMX_RUNTIME_EXCEPTIONOPERATOR_H
#define STROMX_RUNTIME_EXCEPTIONOPERATOR_H

#include <boost/thread.hpp>

#include "stromx/runtime/OperatorKernel.h"
#include "stromx/runtime/Primitive.h"

namespace stromx
{
    namespace runtime
    {
        class ExceptionOperator : public stromx::runtime::OperatorKernel
        {    
        public:        
            enum Parameters
            { 
                THROW_DEACTIVATE,
                BLOCK_EXECUTE,
                THROW_ACTIVATE
            };   
            
            enum Outputs
            { 
                OUTPUT
            };
            
            ExceptionOperator();
            
            virtual OperatorKernel* clone() const { return new ExceptionOperator; }
            virtual void setParameter(const unsigned int id, const stromx::runtime::Data& value);
            const stromx::runtime::DataRef getParameter(const unsigned int id) const;
            virtual void execute(stromx::runtime::DataProvider& provider);
            virtual void activate();
            virtual void deactivate();
            virtual void interrupt();
            
        private:
            typedef boost::lock_guard<boost::mutex> lock_t;
            
            static const std::vector<const stromx::runtime::Description*> setupInputs();
            static const std::vector<const stromx::runtime::Description*> setupOutputs();
            static const std::vector<const stromx::runtime::Parameter*> setupParameters();
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const stromx::runtime::Version VERSION;
            
            bool isBlocked() const;
            
            mutable boost::mutex m_mutex;
            stromx::runtime::Bool m_blockExecute;
            stromx::runtime::Bool m_throwDeactivate;
            stromx::runtime::Bool m_throwActivate;
        };
    }
}

#endif // STROMX_RUNTIME_EXCEPTIONOPERATOR_H
