#include <fstream>
#include <iostream>
#include <vector>

#include "Poco/ConsoleChannel.h"
#include "Poco/Logger.h"

#include "AudioArchiveLoader.hpp"
#include "BaaConverter.hpp"
#include "WavePool.hpp"

Poco::Logger &
create_logger();

int
main(int argc, char **argv)
{
    if (argc!=4)
    {
        std::cout << "Usage: " << argv[0] << " <*.baa> <wave folder> <output folder>" << std::endl;
        return -1;
    }

    std::filesystem::path baaPath = argv[1];
    std::string waveFolder = argv[2];
    std::filesystem::path outputFolder{argv[3]};

    Poco::Logger &logger = create_logger();

    std::ifstream sound_file{baaPath, std::ios::in | std::ios::binary};

    if (!sound_file)
    {
        logger.error("Could not open %s", baaPath.string());
    }

    z2sound::AudioArchiveLoader loader{sound_file, logger};
    auto audio_archive = loader.load();

    if (!audio_archive)
    {
        logger.error("Failed to load audio archive");
    }

    logger.information("Archive has %u wave banks", static_cast<unsigned>(audio_archive->wave_banks_.size()));

    for (auto &[wave_bank_id, wave_bank] : audio_archive->wave_banks_)
    {
        logger.information("Wave bank %u has %u wave groups",
                           static_cast<unsigned>(wave_bank_id),
                           static_cast<unsigned>(wave_bank.get_wave_groups().size()));

        z2sound::WavePool wave_pool{wave_bank};
        wave_pool.set_base_directory(waveFolder);

        for (auto &group : wave_bank.get_wave_groups())
        {
            logger.information("Loading %s", group.get_filename());
            wave_pool.load_group(group);
        }

        logger.information("Finished loading wave groups");

        BaaConverter baa_converter{*audio_archive};

        logger.information("Converting wave bank %u to SoundFont...", static_cast<unsigned>(wave_bank_id));
        auto sf2 = baa_converter.to_sf2(wave_bank_id);

        if (!sf2.has_value())
        {
            logger.error("Failed converting wave bank %u to SoundFont", static_cast<unsigned>(wave_bank_id));
            return -1;
        }

        logger.information("Done.");

        std::ostringstream filename_stream;
        filename_stream << baaPath.stem().string() << "_" << std::to_string(wave_bank_id) << ".sf2";
        std::filesystem::path outputFile = outputFolder/filename_stream.str();

        std::ofstream sf2_file{outputFile, std::ios::binary};

        if (!sf2_file)
        {
            logger.error("Could not open output file %s for writing", outputFile.string());
            return -1;
        }

        sf2->Write(sf2_file);
        logger.information("Finished writing " + outputFile.string());
    }
}

Poco::Logger &
create_logger()
{
    Poco::Logger &logger = Poco::Logger::root();
    Poco::AutoPtr<Poco::ColorConsoleChannel> console_channel{new Poco::ColorConsoleChannel};

    logger.setChannel(console_channel);
    logger.setLevel(Poco::Message::PRIO_TRACE);

    return logger;
}