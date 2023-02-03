#pragma once

#include <optional>
#include <string>
#include <vector>

#include "WaveGroup.hpp"
#include "WaveHandle.hpp"

namespace z2sound
{

class WaveBank
{
friend class WsysParser;

private:
  std::vector<WaveGroup> wave_groups_;
  std::vector<std::optional<WaveHandle>> wave_table_;
};

}
