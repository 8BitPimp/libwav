#include "../libwav/wave.h"


int main(const int argc, const char **args) {

  if (argc < 2) {
    return -1;
  }

  wave_t wav;
  if (!wav.load(args[1])) {
    return 1;
  }

  wav.save("out.wav");

  return 0;
}
