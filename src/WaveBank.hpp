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
    friend class WavePool;

    friend class WsysParser;

public:
    using WaveTable = std::vector<std::optional<WaveHandle>>;

    explicit WaveBank(uint8_t id);

    [[nodiscard]] uint8_t
    get_id() const;

    [[nodiscard]] std::vector<WaveGroup>
    get_wave_groups() const;

    [[nodiscard]] const WaveTable &
    get_wave_table() const;

private:
    uint8_t id_;
    std::vector<WaveGroup> wave_groups_;
    WaveTable wave_table_;
};

}
