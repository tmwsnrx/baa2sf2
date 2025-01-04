#include "WaveGroup.hpp"

#include "AfcDecoder.hpp"
#include "WaveBank.hpp"
#include "WaveHandle.hpp"

#include <cstdint>

namespace z2sound
{

  WaveGroup::WaveGroup(WaveBank &wave_bank, size_t num_entries)
      : wave_bank_{std::ref(wave_bank)}
  {
    group_entries.reserve(num_entries);
    Entry default_entry{.wave_id = 0xffff};
    std::fill(group_entries.begin(), group_entries.end(), default_entry);
  }

  const std::string &WaveGroup::get_filename() const
  {
    return filename_;
  }

  void WaveGroup::set_filename(std::string &&filename)
  {
    filename_ = filename;
  }

}