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
        class ExceptionOperator : public OperatorKernel
        {    
        public:        
            enum Parameters
            { 
                OUTPUT,
                THROW_DEACTIVATE,
                BLOCK_EXECUTE,
                THROW_ACTIVATE
            };
            
            ExceptionOperator();
            
            virtual OperatorKernel* clone() const { return new ExceptionOperator; }
            virtual void setParameter(const unsigned int id, const Data& value);
            const DataRef getParameter(const unsigned int id) const;
            virtual void execute(DataProvider& provider);
            virtual void activate();
            virtual void deactivate();
            virtual void interrupt();
            
        private:
            typedef boost::lock_guard<boost::mutex> lock_t;
            
            static const std::vector<const Input*> setupInputs();
            static const std::vector<const Output*> setupOutputs();
            static const std::vector<const Parameter*> setupParameters();
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const Version VERSION;
            
            bool isBlocked() const;
            
            mutable boost::mutex m_mutex;
            Bool m_blockExecute;
            Bool m_throwDeactivate;
            Bool m_throwActivate;
        };
    }
}

#endif // STROMX_RUNTIME_EXCEPTIONOPERATOR_H
