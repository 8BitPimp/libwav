//  ____     ___ _________  __      __  _________   ____
// |    |   |   |______   \/  \    /  \/  _  \   \ /   /\
// |    |   |   ||    |  _/\   \/\/   /  /_\  \   Y   / /
// |    |___|   ||    |   \ \        /    |    \     / /
// |________|___||________/\ \__/\__/\____|____/\___/ /
// \________\____\\_______\/  \_\/\_\/\___\____\/\__\/
//

#include "../libwave/wave.h"

int main(const int argc, const char **args) {

  if (argc < 2) {
    return 1;
  }

  // load wav file
  wave_t wav_1;
  if (!wav_1.load(args[1])) {
    return 1;
  }

  // save wave file
  if (!wav_1.save("out.wav")) {
    return 1;
  }

  // load saved file
  wave_t wav_2;
  if (!wav_2.load("out.wav")) {
    return 1;
  }

  return 0;
}
