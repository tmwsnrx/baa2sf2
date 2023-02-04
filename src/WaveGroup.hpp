#pragma once

#include <string>

namespace z2sound
{

class WaveGroup
{
public:
  void set_filename(std::string&& filename);

private:
  std::string filename_;
};

}
