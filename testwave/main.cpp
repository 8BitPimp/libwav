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

  wave_t wav;
  if (!wav.load(args[1])) {
    return 1;
  }

  if (!wav.save("out.wav")) {
    return 1;
  }

  return 0;
}
