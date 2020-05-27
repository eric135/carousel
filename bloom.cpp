/* Scalable logging library implementing the Carousel algorithm
 */

#include "bloom.hpp"

#include <functional>

namespace carousel {

Bloom::Bloom(size_t nBits)
  : m_nBits(nBits)
  , m_bits(nBits, false)
{
}

void
Bloom::add(const std::string& key)
{
  m_bits[hash1(key)] = true;
  m_bits[hash2(key)] = true;
  m_bits[hash3(key)] = true;
  m_bits[hash4(key)] = true;
  m_bits[hash5(key)] = true;
}

bool
Bloom::isEvidenced(const std::string& key) const
{
  return m_bits[hash1(key)] &&
         m_bits[hash2(key)] &&
         m_bits[hash3(key)] &&
         m_bits[hash4(key)] &&
         m_bits[hash5(key)];
}

void
Bloom::reset()
{
  m_bits.clear();
  m_bits.assign(m_nBits, false);
}

size_t
Bloom::hash1(const std::string& key) const
{
  // Implementation of SDBM hash function derived from http://www.partow.net/programming/hashfunctions/
  // Copyright 2002 Arash Partow
  //
  // Permission is hereby granted, free of charge, to any person obtaining a copy of this software
  // and associated documentation files (the "Software"), to deal in the Software without
  // restriction, including without limitation the rights to use, copy, modify, merge, publish,
  // distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
  // Software is furnished to do so, subject to the following conditions:
  //
  // The above copyright notice and this permission notice shall be included in all copies or
  // substantial portions of the Software.
  //
  // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
  // BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  // NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
  // DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  size_t hash = 0;
  for (size_t i = 0; i < key.size(); i++)
  {
    hash = static_cast<uint8_t>(key[i]) + (hash << 6) + (hash << 16) - hash;
  }
  return hash % m_nBits;
}

size_t
Bloom::hash2(const std::string& key) const
{
  // Implementation of DJP hash function derived from http://www.partow.net/programming/hashfunctions/
  // Copyright 2002 Arash Partow
  //
  // Permission is hereby granted, free of charge, to any person obtaining a copy of this software
  // and associated documentation files (the "Software"), to deal in the Software without
  // restriction, including without limitation the rights to use, copy, modify, merge, publish,
  // distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
  // Software is furnished to do so, subject to the following conditions:
  //
  // The above copyright notice and this permission notice shall be included in all copies or
  // substantial portions of the Software.
  //
  // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
  // BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  // NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
  // DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  size_t hash = 5381;
  for (size_t i = 0; i < key.size(); i++) {
    hash = ((hash << 5) + hash) + static_cast<uint8_t>(key[i]);
  }
  return hash % m_nBits;
}

size_t
Bloom::hash3(const std::string& key) const
{
  // Implementation of DEK hash function derived from http://www.partow.net/programming/hashfunctions/
  // Copyright 2002 Arash Partow
  //
  // Permission is hereby granted, free of charge, to any person obtaining a copy of this software
  // and associated documentation files (the "Software"), to deal in the Software without
  // restriction, including without limitation the rights to use, copy, modify, merge, publish,
  // distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
  // Software is furnished to do so, subject to the following conditions:
  //
  // The above copyright notice and this permission notice shall be included in all copies or
  // substantial portions of the Software.
  //
  // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
  // BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  // NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
  // DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  size_t hash = key.size();
  for (size_t i = 0; i < key.size(); i++) {
    hash = ((hash << 5) ^ (hash >> 27)) ^ static_cast<uint8_t>(key[i]);
  }
  return hash % m_nBits;
}

size_t
Bloom::hash4(const std::string& key) const
{
  // Implementation of JS hash function derived from http://www.partow.net/programming/hashfunctions/
  // Copyright 2002 Arash Partow
  //
  // Permission is hereby granted, free of charge, to any person obtaining a copy of this software
  // and associated documentation files (the "Software"), to deal in the Software without
  // restriction, including without limitation the rights to use, copy, modify, merge, publish,
  // distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
  // Software is furnished to do so, subject to the following conditions:
  //
  // The above copyright notice and this permission notice shall be included in all copies or
  // substantial portions of the Software.
  //
  // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
  // BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  // NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
  // DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  size_t hash = 1315423911;
  for (size_t i = 0; i < key.size(); i++) {
    hash ^= ((hash << 5) + static_cast<uint8_t>(key[i]) + (hash >> 2));
  }
  return hash % m_nBits;
}

size_t
Bloom::hash5(const std::string& key) const
{
  // Implementation of PJW hash function derived from http://www.partow.net/programming/hashfunctions/
  // Copyright 2002 Arash Partow
  //
  // Permission is hereby granted, free of charge, to any person obtaining a copy of this software
  // and associated documentation files (the "Software"), to deal in the Software without
  // restriction, including without limitation the rights to use, copy, modify, merge, publish,
  // distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
  // Software is furnished to do so, subject to the following conditions:
  //
  // The above copyright notice and this permission notice shall be included in all copies or
  // substantial portions of the Software.
  //
  // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
  // BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  // NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
  // DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  const size_t BitsInUnsignedInt = sizeof(size_t) * 8;
  const size_t ThreeQuarters = (BitsInUnsignedInt  * 3) / 4;
  const size_t OneEighth = BitsInUnsignedInt / 8;
  const size_t HighBits = static_cast<size_t>(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);

  size_t hash = 0;
  size_t test = 0;
  for (size_t i = 0; i < key.size(); i++) {
    hash = (hash << OneEighth) + static_cast<uint8_t>(key[i]);
    if ((test = hash & HighBits) != 0) {
      hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
    }
  }
  return hash;
}

} // namespace carousel
