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
#include <boost/graph/depth_first_search.hpp>
#include <iostream>
#include "stromx/runtime/Description.h"
#include "stromx/runtime/Input.h"
#include "stromx/runtime/Operator.h"
#include "stromx/runtime/SortInputsAlgorithm.h"
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
    
    struct connector_id_t {
        typedef boost::edge_property_tag kind;
    };
    
    struct operator_t {
        typedef boost::vertex_property_tag kind;
    };
    
    typedef boost::property<source_thread_t, int,
            boost::property<target_thread_t, int,
            boost::property<thread_t, int,
            boost::property<connector_id_t, int> > > > EdgeProperty;
    typedef boost::property<operator_t, stromx::runtime::Operator*> VertexProperty;
                                            
    typedef boost::directed_graph<VertexProperty, EdgeProperty, int> Graph;
    
    using namespace boost;
    
    class Visitor : public boost::default_dfs_visitor
    {
    public:    
        Visitor(int & numThreads) : m_numThreads(numThreads) { m_numThreads = 0; }
        
        void discover_vertex(Graph::vertex_descriptor u, const Graph & g) const
        {
            std::map<Graph::edge_descriptor, int> globalThreadMap;
            std::map<int, Graph::edge_descriptor> operatorThreadMap;
            std::map<Graph::edge_descriptor, int> noThreadEdges;
            
            // iterate over all out edges
            {
                typedef graph_traits<Graph>::out_edge_iterator out_edge_iter;
                std::pair<out_edge_iter, out_edge_iter> ep;
                for (ep = boost::out_edges(u, g); ep.first != ep.second; ++ep.first)
                {
                    Graph::edge_descriptor e = *(ep.first);
                    int globalThread = boost::get(thread_t(), g, e);
                    int operatorThread = boost::get(source_thread_t(), g, e);
                    if (globalThread == NO_THREAD)
                    {
                        noThreadEdges[e] = operatorThread;
                    }
                    else
                    {
                        globalThreadMap[e] = globalThread;
                        operatorThreadMap[operatorThread] = e;
                    }
                        
                }
            }
            
            // iterate over all in edges
            {
                typedef graph_traits<Graph>::in_edge_iterator in_edge_iter;
                std::pair<in_edge_iter, in_edge_iter> ep;
                for (ep = boost::in_edges(u, g); ep.first != ep.second; ++ep.first)
                {
                    Graph::edge_descriptor e = *(ep.first);
                    int globalThread = boost::get(thread_t(), g, e);
                    int operatorThread = boost::get(target_thread_t(), g, e);
                    if (globalThread == NO_THREAD)
                    {
                        noThreadEdges[e] = operatorThread;
                    }
                    else
                    {
                        globalThreadMap[e] = globalThread;
                        operatorThreadMap[operatorThread] = e;
                    }
                }
            }
            
            // iterate over all edges without thread
            typedef std::map<Graph::edge_descriptor, int>::iterator edge_iter;
            for (edge_iter i = noThreadEdges.begin(); i != noThreadEdges.end(); ++i)
            {
                if (operatorThreadMap.count(i->second))
                {
                    // an assigned edge for this operator thread exists
                    Graph::edge_descriptor e = operatorThreadMap[i->second];
                    int thread = globalThreadMap[e];
                    boost::put(thread_t(), const_cast<Graph&>(g), i->first, thread);
                }
                else
                {
                    // assign a new thread to this edge
                    boost::put(thread_t(), const_cast<Graph&>(g), i->first, m_numThreads);
                    m_numThreads++;
                }
            }
        }
        
    private:
        int & m_numThreads;
    };
}

namespace stromx
{
    namespace runtime
    {
        
        void AssignThreadsAlgorithm::apply (Stream& stream)
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
                Graph::vertex_descriptor v = graph.add_vertex();
                boost::put(operator_t(), graph, v, *op);
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
                        boost::put(connector_id_t(), graph, e, (*input)->id());
                    }
                }
            }
            
            int numThreads = 0;
            Visitor v(numThreads);
            boost::depth_first_search<>(graph, boost::visitor(v));
            
            // delete all threads
            while(stream.threads().size())
                stream.removeThread(stream.threads().front());
            
            // create threads
            for (int i = 0; i < numThreads; ++i)
                stream.addThread();
                
            // add the inputs to the threads
            graph_traits<Graph>::edge_iterator ei, ei_end;
            for (boost::tie(ei, ei_end) = edges(graph); ei != ei_end; ++ei)
            {
                int thread = boost::get(thread_t(), graph, *ei);
                unsigned int id = boost::get(connector_id_t(), graph, *ei);
                Graph::vertex_descriptor v = boost::target(*ei, graph);
                Operator* op = boost::get(operator_t(), graph, v);
                stream.threads()[thread]->addInput(op, id);
            }
            
            // sort the inputs
            SortInputsAlgorithm sortInputs;
            sortInputs.apply(stream);
        }
    }
}