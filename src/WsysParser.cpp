#include "WsysParser.hpp"

namespace z2sound
{

WsysParser::WsysParser(uint32_t group, std::istream& stream, std::streamoff base_offset, Poco::Logger& logger)
: group_{group}, base_offset_{base_offset}, reader_{stream, Poco::BinaryReader::BIG_ENDIAN_BYTE_ORDER}, logger_{logger}
{}

std::optional<WaveBank> WsysParser::parse()
{
  WaveBank bank;

  uint32_t wave_table_size{};
  reader_.stream().seekg(8, std::ios::cur);
  reader_ >> wave_table_size;
  bank.wave_table_ = std::vector<std::optional<WaveHandle>>{wave_table_size, std::nullopt};

  uint32_t winf_offset{};
  reader_ >> winf_offset;

  uint32_t wbct_offset{};
  reader_ >> wbct_offset;

  reader_.stream().seekg(base_offset_ + wbct_offset + 8);

  uint32_t group_count{};
  reader_ >> group_count;
  bank.wave_groups_.reserve(group_count);

  logger_.debug("Found %u samples in %u groups", wave_table_size, group_count);

  for (size_t group = 0; group < group_count; group++)
  {
    reader_.stream().seekg(base_offset_ + wbct_offset + 12 + group * 4);

    uint32_t scne_offset{};
    reader_ >> scne_offset;
    reader_.stream().seekg(base_offset_ + scne_offset + 12);

    uint32_t cdf_offset{};
    reader_ >> cdf_offset;

    uint32_t num_waves{};
    reader_.stream().seekg(base_offset_ + cdf_offset + 4);
    reader_ >> num_waves;

    uint32_t group_offset{};
    reader_.stream().seekg(base_offset_ + winf_offset + 8 + group * 4);
    reader_ >> group_offset;
    reader_.stream().seekg(base_offset_ + group_offset);

    WaveGroup wave_group;
    reader_ >> wave_group.filename;

    for (size_t wave = 0; wave < num_waves; wave++)
    {
      reader_.stream().seekg(base_offset_ + group_offset + 0x74 + wave * 4);

      uint32_t wave_offset{};
      reader_ >> wave_offset;
      reader_.stream().seekg(base_offset_ + wave_offset + 1);

      WaveInfo wave_info;
      reader_ >> wave_info.codec;
      reader_ >> wave_info.root_key;
      reader_.stream().seekg(1, std::ios::cur);

      reader_ >> wave_info.sample_rate;
      reader_ >> wave_info.data_offset;
      reader_ >> wave_info.data_length;

      reader_ >> wave_info.loop_indicator;
      reader_ >> wave_info.loop_start;
      reader_ >> wave_info.loop_end;

      reader_ >> wave_info._unknown2;
      reader_ >> wave_info.num_samples;

      reader_ >> wave_info._unknown3;
      reader_ >> wave_info._unknown4;

      reader_.stream().seekg(base_offset_ + cdf_offset + 8 + wave * 4);

      uint32_t cdf_entry_offset{};
      reader_ >> cdf_entry_offset;
      reader_.stream().seekg(base_offset_ + cdf_entry_offset + 2);

      uint16_t wave_id{};
      reader_ >> wave_id;

      WaveHandle wave_handle;
      wave_handle.wave_info_ = wave_info;

      bank.wave_table_[wave_id] = wave_handle;
    }

    bank.wave_groups_.emplace_back(wave_group);
  }

  return bank;
}

}