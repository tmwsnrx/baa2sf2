#include "WaveHandle.hpp"

namespace z2sound
{

WaveHandle::WaveHandle(WaveInfo wave_info)
    : wave_info_{wave_info}
{
}

const WaveInfo &
WaveHandle::get_wave_info() const
{
    return wave_info_;
}

std::optional<WaveBuffer>
WaveHandle::get_buffer() const
{
    return buffer_;
}

void
WaveHandle::set_buffer(WaveBuffer wave_buffer)
{
    buffer_ = wave_buffer;
}

}