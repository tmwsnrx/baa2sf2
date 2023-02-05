#include "WavePool.hpp"

#include <cassert>
#include <fstream>
#include <istream>

#include "AfcDecoder.hpp"

namespace z2sound
{

WavePool::WavePool(WaveBank& wave_bank)
: wave_bank_{wave_bank} {}

void WavePool::set_base_directory(std::string_view directory)
{
  base_directory_ = std::filesystem::path{directory};
}

void WavePool::load_group(const WaveGroup& wave_group)
{
  std::filesystem::path wave_archive_file{wave_group.get_filename()};
  auto full_path = base_directory_ / wave_archive_file;

  std::ifstream wave_archive{full_path.c_str(), std::ios::binary | std::ios::in};
  if (!wave_archive)
  {
    return;
  }

  for (auto& entry : wave_group.group_entries)
  {
    auto& wave_handle = wave_bank_.get_wave_table()[entry.wave_id];
    const auto& wave_info = wave_handle->get_wave_info();

    wave_archive.seekg(wave_info.data_offset);

    const size_t num_buffers = wave_info.data_length / 9;

    std::vector<uint8_t> encoded_samples;
    encoded_samples.reserve(wave_info.data_length);

    std::vector<int16_t> decoded_samples(num_buffers * 16, 0);

    for (size_t offset = 0; offset < wave_info.data_length; offset++)
    {
      uint8_t datum{};
      wave_archive.read(reinterpret_cast<char*>(&datum), 1);
      encoded_samples.emplace_back(datum);
    }

    AfcDecoder afc_decoder;

    for (size_t i = 0; i < num_buffers; i++)
    {
      assert((i + 1) * 16 <= decoded_samples.size());
      std::span<const uint8_t, 9> source{encoded_samples.cbegin() + i * 9, encoded_samples.cbegin() + (i + 1) * 9};
      std::span<int16_t, 16> destination{decoded_samples.begin() + i * 16, decoded_samples.begin() + (i + 1) * 16};
      afc_decoder.decode_buffer(source, destination);
    }

    const auto& data = wave_buffers_.emplace_back(std::move(decoded_samples));

    WaveBuffer wave_buffer{data.cbegin(), data.cend()};
    wave_handle->set_buffer(wave_buffer);
  }
}

}