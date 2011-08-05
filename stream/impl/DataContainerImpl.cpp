#include "DataContainerImpl.h"

#include "../Exception.h"
#include "../Recycler.h"
#include "../Data.h"

#include <boost/assert.hpp>

namespace stream
{
    namespace impl
    {
        DataContainerImpl::DataContainerImpl(Data* const data)
        : m_readAccessCounter(0),
            m_writeAccess(false),
            m_recycleAccess(0),
            m_data(data)
        {
            if(! data)
                throw InvalidArgument();
        }
        
        void DataContainerImpl::getReadAccess()
        {
            unique_lock_t lock(m_mutex);
            
            try
            {
                while(m_writeAccess)
                    m_cond.wait(lock);
            }
            catch(boost::thread_interrupted&)
            {
                throw Interrupt();
            } 
            
            m_readAccessCounter++;
        }
        
        void DataContainerImpl::getReadAccess(const unsigned int timeout)
        {
            unique_lock_t lock(m_mutex);
            
            boost::system_time const finish = boost::get_system_time() + boost::posix_time::millisec(timeout);
            
            try
            {
                while(m_writeAccess)
                {
                    if(! m_cond.timed_wait(lock, finish))
                        throw Timeout();
                }
            }
            catch(boost::thread_interrupted&)
            {
                throw Interrupt();
            } 
            
            m_readAccessCounter++;
        }
        
        DataContainerImpl::~DataContainerImpl()
        {
            recycle();
            
            delete m_data;
        }

        void DataContainerImpl::returnReadAccess()
        {
            lock_t lock(m_mutex);
            
            BOOST_ASSERT(m_readAccessCounter);
            m_readAccessCounter--;
            
            m_cond.notify_all();
        }
            
        void DataContainerImpl::getWriteAccess()
        {
            unique_lock_t lock(m_mutex);
            
            try
            {
                while(m_readAccessCounter || m_writeAccess)
                    m_cond.wait(lock);
            }
            catch(boost::thread_interrupted&)
            {
                throw Interrupt();
            } 
                
            m_writeAccess = true;
        }
            
        void DataContainerImpl::getWriteAccess(const unsigned int timeout)
        {
            unique_lock_t lock(m_mutex);
            
            boost::system_time const finish = boost::get_system_time() + boost::posix_time::millisec(timeout);
            
            try
            {
                while(m_readAccessCounter || m_writeAccess)
                {
                    if(! m_cond.timed_wait(lock, finish))
                        throw Timeout();
                }
            }
            catch(boost::thread_interrupted&)
            {
                throw Interrupt();
            } 
                
            m_writeAccess = true;
        }

        void DataContainerImpl::returnWriteAccess()
        {
            lock_t lock(m_mutex);
            
            BOOST_ASSERT(m_writeAccess);
            m_writeAccess = false;
            
            m_cond.notify_all();
        }

        void DataContainerImpl::getRecycleAccess(Recycler*const recycler)
        {        
            if(! recycler)
                throw InvalidArgument();
            
            unique_lock_t lock(m_mutex);
            
            try
            {
                while(m_recycleAccess)
                    m_cond.wait(lock);
            }
            catch(boost::thread_interrupted&)
            {
                throw Interrupt();
            } 
                
            m_recycleAccess = recycler;
        }

        void DataContainerImpl::returnRecycleAccess()
        {
            lock_t lock(m_mutex);
            
            BOOST_ASSERT(m_recycleAccess);
            m_recycleAccess = 0;
            
            m_cond.notify_all();
        }
        
        void DataContainerImpl::recycle()
        {
            lock_t lock(m_mutex);
            
            if(m_recycleAccess)
            {
                m_recycleAccess->recycle(this);
                m_data = 0;
            }        
        }
    }
} 
