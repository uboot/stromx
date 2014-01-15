/* 
 *  Copyright 2013 Matthias Fuchs
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

#ifndef STROMX_RUNTIME_POSITION_H
#define STROMX_RUNTIME_POSITION_H


namespace stromx
{
    namespace runtime
    {
        /** \brief 2D position. */
        class Position
        {
        public:
            /** Constructs a 2D position with both coordinates set to 0. */
            Position() : m_x(0), m_y(0) {}
            
            /** Constructs a 2D position with float coordinates. */
            Position(const float x, const float y) : m_x(x), m_y(y) {}
            
            /** Returns the x-coordinate. */
            float x() const { return m_x; }
            
            /** Returns the y-coordinate. */
            float y() const { return m_y; }
            
        private:
            float m_x;
            float m_y;
        };
    }
}

#endif // STROMX_RUNTIME_POSITION_H