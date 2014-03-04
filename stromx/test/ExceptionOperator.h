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

#ifndef EXCEPTIONOPERATOR_H
#define EXCEPTIONOPERATOR_H

#include <stromx/runtime/OperatorKernel.h>
#include <stromx/runtime/Primitive.h>

#include "stromx/test/Config.h"

namespace stromx {
namespace test {

class STROMX_TEST_API ExceptionOperator : public stromx::runtime::OperatorKernel
{
    enum Inputs 
    { 
        INPUT
    };
    
    enum Parameters
    { 
        THROW_INITIALIZE,
        THROW_ACTIVATE,
        THROW_EXECUTE,
        THROW_DEACTIVATE,
        THROW_DEINITIALIZE
    };
    
public:
    ExceptionOperator();
    
    virtual OperatorKernel* clone() const { return new ExceptionOperator; }
    virtual void setParameter(const unsigned int id, const stromx::runtime::Data& value);
    const stromx::runtime::DataRef getParameter(const unsigned int id) const;
    virtual void execute(stromx::runtime::DataProvider& provider);
    virtual void activate();
    virtual void initialize();
    virtual void deactivate();
    virtual void deinitialize();
    
private:
    static const std::vector<const stromx::runtime::Description*> setupInputs();
    static const std::vector<const stromx::runtime::Description*> setupOutputs();
    static const std::vector<const stromx::runtime::Parameter*> setupParameters();
    
    static const std::string TYPE;
    static const std::string PACKAGE;
    static const stromx::runtime::Version VERSION;
    
    stromx::runtime::Bool m_throwInitialize;
    stromx::runtime::Bool m_throwActivate;
    stromx::runtime::Bool m_throwExecute;
    stromx::runtime::Bool m_throwDeactivate;
    stromx::runtime::Bool m_throwDeinitialize;
};

}
}

#endif // PARAMETEROPERATOR_H
