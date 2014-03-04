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

#ifndef DEADLOCKOPERATOR_H
#define DEADLOCKOPERATOR_H

#include <stromx/runtime/OperatorKernel.h>
#include <stromx/runtime/Primitive.h>
#include <stromx/runtime/WriteAccess.h>

#include "stromx/test/Config.h"

namespace stromx {
namespace test {

class STROMX_TEST_API DeadlockOperator : public stromx::runtime::OperatorKernel
{
    enum Inputs 
    { 
        INPUT,
        OUTPUT
    };
    
    enum Parameters
    { 
        LOCK_PARAMETERS,
        LOCK_DATA,
        DUMMY
    };
    
public:
    DeadlockOperator();
    
    virtual OperatorKernel* clone() const { return new DeadlockOperator; }
    virtual void setParameter(const unsigned int id, const stromx::runtime::Data& value);
    const stromx::runtime::DataRef getParameter(const unsigned int id) const;
    virtual void activate();
    virtual void execute(stromx::runtime::DataProvider& provider);
    
private:
    static const std::vector<const stromx::runtime::Description*> setupInputs();
    static const std::vector<const stromx::runtime::Description*> setupOutputs();
    static const std::vector<const stromx::runtime::Parameter*> setupParameters();
    
    static const std::string TYPE;
    static const std::string PACKAGE;
    static const stromx::runtime::Version VERSION;
    
    stromx::runtime::Bool m_lockParameters;
    stromx::runtime::Bool m_lockData;
    bool m_dataHasBeenLocked;
    stromx::runtime::WriteAccess<stromx::runtime::UInt32> m_writeAccess;
    stromx::runtime::UInt8 m_dummy;
};

}
}

#endif // DEADLOCKOPERATOR_H
