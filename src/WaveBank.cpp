#include "WaveBank.hpp"

namespace z2sound
{

WaveBank::WaveBank(uint8_t id)
    : id_{id}
{
}

uint8_t
WaveBank::get_id() const
{
    return id_;
}

std::vector<WaveGroup>
WaveBank::get_wave_groups() const
{
    return wave_groups_;
}

const WaveBank::WaveTable &
WaveBank::get_wave_table() const
{
    return wave_table_;
}

}