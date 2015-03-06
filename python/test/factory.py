# -*- coding: utf-8 -*-

#
#  Copyright 2011 Matthias Fuchs
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#

from stromx import runtime

factory = runtime.Factory()

runtime.register(factory)

stream = runtime.Stream()
kernel = factory.newOperator("runtime", "Counter")
op1 = stream.addOperator(kernel)
stream.hideOperator(op1)

op2 = runtime.Operator(runtime.Receive())
try:
  stream.hideOperator(op2)
except runtime.WrongArgument as e:
  print e

