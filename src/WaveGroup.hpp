#pragma once

#include <string>
#include <utility>
#include <vector>

namespace z2sound
{

class WaveBank;

class WaveGroup
{
public:
  struct Entry
  {
    uint16_t wave_id;
    uint32_t data_offset;
  };

  WaveGroup(WaveBank& wave_bank, size_t num_entries);

  const std::string& get_filename() const;
  void set_filename(std::string&& filename);

  std::vector<Entry> group_entries;

private:
  std::reference_wrapper<WaveBank> wave_bank_;
  std::string filename_;
};

}
