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

#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/assert.hpp>
#include <iostream>
#include "stromx/runtime/Description.h"
#include "stromx/runtime/Input.h"
#include "stromx/runtime/Operator.h"
#include "stromx/runtime/AssignThreadsAlgorithm.h"
#include "stromx/runtime/Stream.h"
#include "stromx/runtime/Thread.h"

namespace stromx
{
    namespace runtime
    {
        namespace
        {
            class InputComparator
            {
            public:
                InputComparator(const std::map<const Operator*, unsigned int> & op2RankMap)
                  : m_op2RankMap(op2RankMap)
                {}
                
                bool operator()(const Input & input1, const Input & input2)
                {
                    
                    std::map<const Operator*, unsigned int>::const_iterator rank1 = m_op2RankMap.find(input1.op());
                    BOOST_ASSERT(rank1 != m_op2RankMap.end());
                    
                    std::map<const Operator*, unsigned int>::const_iterator rank2 = m_op2RankMap.find(input2.op());
                    BOOST_ASSERT(rank2 != m_op2RankMap.end());
                    
                    return rank1->second > rank2->second ? true : false;
                }
                
            private:
                const std::map<const Operator*, unsigned int> & m_op2RankMap;
            };
        }
        
        void AssignThreadsAlgorithm::apply ( Stream& stream )
        {
            const std::vector<Operator*> & operators = stream.initializedOperators();
            unsigned int numNodes = operators.size();
            
            if(! numNodes)
                return;
            
            std::map<const Operator*, unsigned int> op2IdMap;
            std::map<unsigned int, const Operator*> id2OpMap;
            
            // fill the maps
            unsigned int i = 0;
            for(std::vector<Operator*>::const_iterator opIter = operators.begin();
                opIter != operators.end();
                ++opIter, ++i)
            {
                op2IdMap[*opIter] = i;
                id2OpMap[i] = *opIter;
            }
                
            // create the graph
            typedef 
            boost::adjacency_list <boost::vecS,
                                   boost::vecS, 
                                   boost::directedS,
                                   boost::no_property,
                                   unsigned int> 
                                  OperatorThreadGraph;
            OperatorThreadGraph graph(numNodes);
            
            // iterate over each operator
            for(std::vector<Operator*>::const_iterator opIter = operators.begin();
                opIter != operators.end();
                ++opIter)
            {
                const std::vector<const Description*> & inputs = (*opIter)->info().inputs();
                for(std::vector<const Description*>::const_iterator inputIter = inputs.begin();
                    inputIter != inputs.end();
                    ++inputIter)
                {
                    Output output = stream.connectionSource(*opIter, (*inputIter)->id());
                    if(output.valid())
                    {
                        const Operator* source = output.op();
                        const Operator* target = *opIter;
                        
                        boost::add_edge(op2IdMap[source], op2IdMap[target], 0, graph);
                    }
                }
            }
        }
    }
}