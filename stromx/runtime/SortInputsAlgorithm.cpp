/* 
*  Copyright 2012 Matthias Fuchs
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

#include <boost/assert.hpp>
#include <boost/functional/hash.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/topological_sort.hpp>
#include <iostream>
#include "stromx/runtime/Description.h"
#include "stromx/runtime/InputConnector.h"
#include "stromx/runtime/Operator.h"
#include "stromx/runtime/SortInputsAlgorithm.h"
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
                
                bool operator()(const InputConnector & input1, const InputConnector & input2)
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
        
        void SortInputsAlgorithm::apply ( Stream& stream )
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
            boost::adjacency_matrix<boost::directedS> graph(numNodes);
            
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
                    OutputConnector output = stream.connectionSource(*opIter, (*inputIter)->id());
                    if(output.valid())
                    {
                        const Operator* source = output.op();
                        const Operator* target = *opIter;
                        
                        boost::add_edge(op2IdMap[source], op2IdMap[target], graph);
                    }
                }
            }
            
            // allocate a vector for the output
            std::vector<unsigned int> output(operators.size());
            std::vector<unsigned int>::iterator outputIter = output.begin();
            
            // run the algorithm
            boost::topological_sort(graph, outputIter);
            
            // map the operators to their ranks
            std::map<const Operator*, unsigned int> op2RankMap;
            for(unsigned int rank = 0; rank < output.size(); ++rank)
            {
                op2RankMap[id2OpMap[output[rank]]] = rank;
            }
            
            // reorder the input sequences
            const std::vector<Thread*> & threads = stream.threads();
            for(std::vector<Thread*>::const_iterator threadIter = threads.begin();
                threadIter != threads.end();
                ++threadIter)
            {
                Thread* thread = *threadIter;
                std::vector<InputConnector> inputs = thread->inputSequence();
                
                // delete all inputs
                while(thread->inputSequence().size())
                    thread->removeInput(thread->inputSequence().size() - 1);
                
                InputComparator comp(op2RankMap);
                std::sort(inputs.begin(), inputs.end(), comp);
                
                for(std::vector<InputConnector>::const_iterator inputIter = inputs.begin();
                    inputIter != inputs.end();
                    ++inputIter)
                {
                    // add the input to the thread
                    // cast away constness of the operator (non-const access is anyways
                    // available via Stream::operators())
                    thread->addInput(const_cast<Operator*>(inputIter->op()), inputIter->id());
                }
            }
        }
    }
}