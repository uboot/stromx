#include "RecycleAccessImpl.h"

#include "../Data.h"
#include "../DataContainer.h"
#include "../Exception.h"

namespace stream
{
    namespace impl
    {
        RecycleAccessImpl::RecycleAccessImpl(DataContainer& data)
        {
            add(data);
        }
        
        void RecycleAccessImpl::add(DataContainer& data)
        {
            if(data.empty())
                throw WrongArgument("Data container is empty.");
            
            lock_t lock(m_mutex);
            DataContainerImpl* container = data.m_impl.get();
            m_dataContainer.insert(container);
            container->getRecycleAccess(this);
        }

        void RecycleAccessImpl::recycle(DataContainerImpl* const container)
        {
            lock_t lock(m_mutex);
            
            BOOST_ASSERT(m_dataContainer.count(container));
            
            m_dataContainer.erase(container);
            m_data.push_back(container->data());
            m_cond.notify_all();
        }

        RecycleAccessImpl::~RecycleAccessImpl()
        {
            for(std::set<DataContainerImpl*>::iterator iter = m_dataContainer.begin();
                iter != m_dataContainer.end();
                ++iter)
            {
                (*iter)->returnRecycleAccess();
            }
            
            for(std::deque<Data*>::iterator iter = m_data.begin();
                iter != m_data.end();
                ++iter)
            {
                delete *iter;
            }
        }
        
        Data*const RecycleAccessImpl::operator()()
        {
            unique_lock_t lock(m_mutex);
            
            if(m_dataContainer.empty() && m_data.empty())
                return 0;
            
            try
            {
                while(m_data.empty())
                    m_cond.wait(lock);
            }
            catch(boost::thread_interrupted&)
            {
                throw Interrupt();
            }
            
            Data* value = m_data.front();
            m_data.pop_front();
            return value;
        }
        
        Data*const RecycleAccessImpl::operator()(const unsigned int timeout)
        {
            unique_lock_t lock(m_mutex);
            
            if(m_dataContainer.empty() && m_data.empty())
                return 0;
            
            boost::system_time const finish = boost::get_system_time() + boost::posix_time::millisec(timeout);
            
            try
            {
                while(m_data.empty())
                {
                    if(! m_cond.timed_wait(lock, finish))
                        throw Timeout();
                }
            }
            catch(boost::thread_interrupted&)
            {
                throw Interrupt();
            }
            
            Data* value = m_data.front();
            m_data.pop_front();
            return value;
        }
    }
} 
