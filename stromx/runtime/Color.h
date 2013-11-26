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

#ifndef STROMX_RUNTIME_COLOR_H
#define STROMX_RUNTIME_COLOR_H

#ifdef __GNUG__
    #include <tr1/cstdint>
#else
    #include <cstdint>
#endif

namespace stromx
{
    namespace runtime
    {
        /** RGB color. */
        class Color
        {
        public:
            /** Constructs an RGB with all color values set to 0 (i.e. black). */
            Color()
              : m_r(0), m_g(0), m_b(0) {}
            
            /** Constructs an RGB color with values in the range 0-255. */
            Color(const uint8_t r, const uint8_t g, const uint8_t b)
              : m_r(r), m_g(g), m_b(b) {}
            
            /** Returns the red value. */
            uint8_t r() const { return m_r; }
            
            /** Returns the green value. */
            uint8_t g() const { return m_g; }
            
            /** Returns the blue value. */
            uint8_t b() const { return m_b; }
            
        private:
            uint8_t m_r;
            uint8_t m_g;
            uint8_t m_b;
        };
    }
}

#endif // STROMX_RUNTIME_COLOR_H