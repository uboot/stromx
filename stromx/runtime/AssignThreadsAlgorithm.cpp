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
#include <boost/functional/hash.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/undirected_dfs.hpp>
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
    
    struct operator_thread_t {
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
    
    typedef std::map<boost::adjacency_list<>::vertex_descriptor, int> operator_thread_map;
    
    typedef boost::property<operator_thread_t, operator_thread_map,
            boost::property<thread_t, int,
            boost::property<connector_id_t, int,
            boost::property<boost::edge_color_t, boost::default_color_type> > > >
            EdgeProperty;
    typedef boost::property<operator_t, stromx::runtime::Operator*> VertexProperty;
    
    using namespace boost;
    typedef adjacency_list<boost::vecS,
                           boost::vecS,
                           boost::undirectedS, 
                           VertexProperty, 
                           EdgeProperty> Graph;
    
    class Visitor : public boost::dfs_visitor<>
    {
    public:    
        Visitor(int & numThreads) : m_numThreads(numThreads) { m_numThreads = 0; }
        
        void discover_vertex(Graph::vertex_descriptor u, const Graph & g) const
        {
            std::map<Graph::edge_descriptor, int> globalThreadMap; // edge to global thread
            std::map<int, Graph::edge_descriptor> operatorThreadMap; // local thread at operator to edge
            std::map<Graph::edge_descriptor, int> noThreadEdges; // edge to operator thread at operator
            
            // iterate over all edges
            typedef graph_traits<Graph>::out_edge_iterator out_edge_iter;
            std::pair<out_edge_iter, out_edge_iter> ep;
            for (ep = boost::out_edges(u, g); ep.first != ep.second; ++ep.first)
            {
                Graph::edge_descriptor e = *(ep.first);
                operator_thread_map operatorThreads = boost::get(operator_thread_t(), g, e);
                int operatorThread = operatorThreads[u];
                int globalThread = boost::get(thread_t(), g, e);
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
                    operatorThreadMap[i->second] = i->first;
                    globalThreadMap[i->first] = m_numThreads;
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
        
        void AssignThreadsAlgorithm::apply(Stream& stream)
        {            
            const std::vector<Operator*> & operators = stream.initializedOperators();
            if (operators.size() == 0)
                return;
            
            // create the graph
            Graph graph;
            
            std::map<const Operator*, Graph::vertex_descriptor> op2VertexMap;
            
            // fill the maps
            for(std::vector<Operator*>::const_iterator op = operators.begin();
                op != operators.end();
                ++op)
            {
                
                Graph::vertex_descriptor v = boost::add_vertex(graph);
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
                        const Description& output = source->info().output(connector.id());
                        
                        Graph::edge_descriptor e = boost::add_edge(op2VertexMap[source], op2VertexMap[target], graph).first;
                        
                        std::map<Graph::vertex_descriptor, int> operatorThreads;
                        operatorThreads[op2VertexMap[source]] = output.operatorThread();
                        operatorThreads[op2VertexMap[target]] = (*input)->operatorThread();
                        
                        boost::put(operator_thread_t(), graph, e, operatorThreads);
                        boost::put(thread_t(), graph, e, NO_THREAD);
                        boost::put(connector_id_t(), graph, e, (*input)->id());
                    }
                }
            }
            
            int numThreads = 0;
            Visitor v(numThreads);
            boost::undirected_dfs<>(graph, root_vertex(Graph::vertex_descriptor(0)).visitor(v)
                                    .edge_color_map(get(edge_color, graph)));
            
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
                if (thread == NO_THREAD)
                    continue; 
                    
                unsigned int id = boost::get(connector_id_t(), graph, *ei);
                Graph::vertex_descriptor v = boost::target(*ei, graph);
                Operator* op = boost::get(operator_t(), graph, v);
                stream.threads()[thread]->addInput(op, id);
            }
        }
    }
}