#include "stromx/runtime/MatrixWrapper.h"

namespace stromx
{
    namespace runtime
    {
        class MatrixImpl : public stromx::runtime::MatrixWrapper
        {
        public:
            MatrixImpl() : m_data(0) {}
            
            MatrixImpl(const unsigned int rows, const unsigned int cols, const ValueType valueType) 
              : m_data(0) 
            {
                allocate(rows, cols, valueType);
            }
            const Version & version() const { return VERSION; }
            const std::string & type() const { return TYPE; }
            const std::string & package() const { return PACKAGE; }
            
            Data* clone() const { return new MatrixImpl(); }
            
        protected:
            void allocate(const unsigned int rows, const unsigned int cols, const ValueType valueType)
            {
                if(m_data)
                {
                    delete [] m_data;
                    m_data = 0;
                }
                
                unsigned int valueSize = Matrix::valueSize(valueType);
                unsigned int bufferSize = rows * cols * valueSize;
                m_data = new uint8_t[bufferSize];
                
                setBuffer(m_data, bufferSize);
                initializeMatrix(rows, cols, cols * valueSize, m_data, valueType);
            }
            
        private:
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const Version VERSION;
            uint8_t* m_data;
        };
    }
}
