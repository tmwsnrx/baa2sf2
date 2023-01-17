#include <fstream>
#include <iostream>

#include "AudioArchiveLoader.hpp"

int main(int, char**)
{
  std::ifstream sound_file{"Z2Sound.baa", std::ios::in | std::ios::binary};

  if (!sound_file)
  {
    std::cout << "Could not open Z2Sound.baa" << std::endl;
  }

  z2sound::AudioArchiveLoader loader{sound_file};
  auto audio_archive = loader.load();

  if (!audio_archive)
  {
    std::cout << "Failed to load audio archive" << std::endl;
  }
}
