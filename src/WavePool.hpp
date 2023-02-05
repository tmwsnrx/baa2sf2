#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "WaveBank.hpp"
#include "WaveGroup.hpp"

namespace z2sound
{

class WavePool
{
public:
  explicit WavePool(WaveBank& wave_bank);

  void set_base_directory(std::string_view directory);
  void load_group(const WaveGroup& wave_group);

private:
  WaveBank& wave_bank_;
  std::filesystem::path base_directory_;
  std::vector<std::vector<int16_t>> wave_buffers_;
};

}