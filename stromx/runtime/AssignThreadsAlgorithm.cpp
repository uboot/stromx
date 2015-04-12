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

#include "stromx/runtime/AssignThreadsAlgorithm.h"

#include <boost/assert.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <iostream>
#include "stromx/runtime/Description.h"
#include "stromx/runtime/Input.h"
#include "stromx/runtime/Operator.h"
#include "stromx/runtime/Stream.h"
#include "stromx/runtime/Thread.h"

namespace
{
    static const int NO_THREAD = -1;
    
    struct source_thread_t {
        typedef boost::edge_property_tag kind;
    };
    
    struct target_thread_t {
        typedef boost::edge_property_tag kind;
    };
    
    struct thread_t {
        typedef boost::edge_property_tag kind;
    };
    
    typedef boost::property<source_thread_t, int,
                            boost::property<target_thread_t, int,
                                            boost::property<thread_t, int> > > EdgeProperty;
    typedef boost::directed_graph<stromx::runtime::Operator*, EdgeProperty> Graph;
    
    using namespace boost;
    
    class Visitor : public boost::default_bfs_visitor
    {
    public:        
        void discover_vertex(Graph::vertex_descriptor u, const Graph & g) const
        {
            typedef graph_traits<Graph>::out_edge_iterator out_edge_iter;
            std::pair<out_edge_iter, out_edge_iter> ep;
            for (ep = boost::out_edges(u, g); ep.first != ep.second; ++ep.first)
            {
                Graph::edge_descriptor e = *(ep.first);
                std::cout << boost::get(source_thread_t(), g, e) << std::endl;
                std::cout << boost::get(target_thread_t(), g, e) << std::endl;
                std::cout << boost::get(thread_t(), g, e) << std::endl;
            }
            
            std::cout << u << std::endl;
        }
    };
}

namespace stromx
{
    namespace runtime
    {
        
        void AssignThreadsAlgorithm::apply ( Stream& stream )
        {            
            const std::vector<Operator*> & operators = stream.initializedOperators();
            
            // create the graph
            Graph graph;
            
            std::map<const Operator*, Graph::vertex_descriptor> op2VertexMap;
            
            // fill the maps
            for(std::vector<Operator*>::const_iterator op = operators.begin();
                op != operators.end();
                ++op)
            {
                Graph::vertex_descriptor v = graph.add_vertex(*op);
                op2VertexMap[*op] = v;
            }
                
            
            // iterate over each operator
            for(std::vector<Operator*>::const_iterator op = operators.begin();
                op != operators.end();
                ++op)
            {
                const std::vector<const Description*> & inputs = (*op)->info().inputs();
                for(std::vector<const Description*>::const_iterator input = inputs.begin();
                    input != inputs.end();
                    ++input)
                {
                    Output connector = stream.connectionSource(*op, (*input)->id());
                    if(connector.valid())
                    {
                        const Operator* source = connector.op();
                        const Operator* target = *op;
                        const Description* output = source->info().outputs()[connector.id()];
                        
                        Graph::edge_descriptor e = graph.add_edge(op2VertexMap[source], op2VertexMap[target]).first;
                        boost::put(source_thread_t(), graph, e, output->operatorThread());
                        boost::put(target_thread_t(), graph, e, (*input)->operatorThread());
                        boost::put(thread_t(), graph, e, NO_THREAD);
                    }
                }
            }
            
            
            Visitor v;
            boost::breadth_first_search(graph, *(boost::vertices(graph).first), boost::visitor(v));
        }
    }
}