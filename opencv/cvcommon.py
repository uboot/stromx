# -*- coding: utf-8 -*-

import document
import package

# utilitiy functions
dcl = document.Document()
dcl.line("void checkEnumValue(const stromx::runtime::Enum & value, "
         "const stromx::runtime::EnumParameter* param, "
         "const stromx::runtime::OperatorKernel& op);")
dclIncludes = ["<stromx/runtime/Enum.h>",
               "<stromx/runtime/EnumParameter.h>",
               "<stromx/runtime/OperatorKernel.h>"]
dtn = document.Document()
dtn.line("void checkEnumValue(const stromx::runtime::Enum & value, "
         "const stromx::runtime::EnumParameter* param, "
         "const stromx::runtime::OperatorKernel& op)")
dtn.scopeEnter()
dtn.line("using namespace runtime;")
dtn.blank()
dtn.line("for(std::vector<EnumDescription>::const_iterator "
         "iter = param->descriptions().begin(); iter != "
         "param->descriptions().end(); ++iter)")
dtn.scopeEnter()
dtn.line(" if(value == iter->value())")
dtn.line("return;")
dtn.scopeExit()
dtn.line("throw stromx::runtime::WrongParameterValue(*param, op);")
dtn.scopeExit()
dtnIncludes = ["<stromx/runtime/OperatorException.h>"]
checkEnumValue = package.Function(dcl, dclIncludes, dtn, dtnIncludes)

dcl = document.Document()
dclIncludes = ["<stromx/runtime/NumericParameter.h>",
               "<stromx/runtime/OperatorException.h>"]
dcl.line("template<class T>");
dcl.line("void checkNumericValue(const T & value, const "
         "runtime::NumericParameter<T>* param, "
         "const stromx::runtime::OperatorKernel& op)");
dcl.scopeEnter()
dcl.line("if(value < runtime::data_cast<T>(param->min()))")
dcl.increaseIndent()
dcl.line("throw runtime::WrongParameterValue(*param, op);")
dcl.decreaseIndent()
dcl.line("if(value > runtime::data_cast<T>(param->max()))")
dcl.increaseIndent()
dcl.line("throw runtime::WrongParameterValue(*param, op);")
dcl.decreaseIndent()
dcl.scopeExit()
checkNumericValue = package.Function(dcl, dclIncludes)