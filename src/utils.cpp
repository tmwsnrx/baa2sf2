#include "utils.hpp"

namespace z2sound
{

std::string marker_to_string(uint32_t marker)
{
  std::array<char, 4> marker_characters{
    static_cast<char>(marker >> 24),
    static_cast<char>((marker >> 16) & 0xff),
    static_cast<char>((marker >> 8) & 0xff),
    static_cast<char>(marker & 0xff)
  };

  return std::string{marker_characters.cbegin(), marker_characters.cend()};
}

}
