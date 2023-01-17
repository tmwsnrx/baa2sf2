#include <fstream>
#include <iostream>

#include "Poco/ConsoleChannel.h"
#include "Poco/Logger.h"

#include "AudioArchiveLoader.hpp"

Poco::Logger& create_logger();

int main(int, char**)
{
  Poco::Logger& logger = create_logger();

  std::ifstream sound_file{"Z2Sound.baa", std::ios::in | std::ios::binary};

  if (!sound_file)
  {
    std::cout << "Could not open Z2Sound.baa" << std::endl;
  }

  z2sound::AudioArchiveLoader loader{sound_file, logger};
  auto audio_archive = loader.load();

  if (!audio_archive)
  {
    std::cout << "Failed to load audio archive" << std::endl;
  }
}

Poco::Logger& create_logger()
{
  Poco::Logger& logger = Poco::Logger::root();
  Poco::AutoPtr<Poco::ColorConsoleChannel> console_channel{new Poco::ColorConsoleChannel};

  logger.setChannel(console_channel);
  logger.setLevel(Poco::Message::PRIO_TRACE);

  return logger;
}