#include "wave.h"
#include "_file.h"

#if !defined(_MSC_VER)
#define PACK__ __attribute__((__packed__))
#else
#define PACK__
#pragma pack(push, 1)
#endif

namespace {

constexpr uint32_t fourcc(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
  return (d << 24) | (c << 16) | (b << 8) | a;
}

enum {
  FMT_PCM = 1,
  FCC_RIFF = fourcc('R', 'I', 'F', 'F'),
  FCC_WAVE = fourcc('W', 'A', 'V', 'E'),
  FCC_FMT = fourcc('f', 'm', 't', ' '),
  FCC_DATA = fourcc('d', 'a', 't', 'a'),
};

struct PACK__ riff_t {
  uint32_t chunk_id_;
  uint32_t chunk_size_;
  uint32_t format_;
};

struct PACK__ fmt_t {
  uint32_t chunk_id_;
  uint32_t chunk_size_;
  uint16_t format_;
  uint16_t channels_;
  uint32_t sample_rate_;
  uint32_t byte_rate_;
  uint16_t block_align_;
  uint16_t bit_depth_;
};

struct PACK__ data_t {
  uint32_t chunk_id_;
  uint32_t chunk_size_;
  // after starts sample data
};

#if defined(_MSC_VER)
#pragma pack(pop)
#endif

} // namespace

bool wave_t::load(const char *path) {

  file_reader_t file;
  if (!file.open(path)) {
    return false;
  }

  riff_t riff;
  fmt_t fmt;
  data_t data;

  // read riff header
  if (!file.read(riff)) {
    return false;
  }
  if (riff.chunk_id_ != FCC_RIFF) {
    return false;
  }
  if (riff.format_ != FCC_WAVE) {
    return false;
  }

  // read format chunk
  if (!file.read(fmt)) {
    return false;
  }
  if (fmt.chunk_id_ != FCC_FMT) {
    return false;
  }
  if (fmt.format_ != FMT_PCM) {
    return false;
  }
  if (fmt.bit_depth_ & 7 /* multiple of 8 */) {
    return false;
  }
  if (fmt.channels_ != 1 && fmt.channels_ != 2) {
    return false;
  }

  // read data chunk
  if (!file.read(data)) {
    return false;
  }
  if (data.chunk_id_ != FCC_DATA) {
    return false;
  }
  // sanity check on file size
  size_t file_size = 0;
  if (!file.size(file_size)) {
    return false;
  }
  if (data.chunk_size_ >= file_size) {
    return false;
  }
  // read sample data
  sample_bytes_ = data.chunk_size_;
  samples_ = std::make_unique<uint8_t[]>(data.chunk_size_);
  if (!file.read(samples_.get(), data.chunk_size_)) {
    return false;
  }
  // copy into output structure
  bit_depth_ = fmt.bit_depth_;
  sample_rate_ = fmt.sample_rate_;
  channels_ = fmt.channels_;
  // success
  return true;
}

bool wave_t::save(const char *path) {

  file_writer_t file(path);
  if (!file.is_open()) {
    return false;
  }

  // write riff header
  riff_t riff;
  riff.chunk_id_ = FCC_RIFF;
  riff.format_ = FCC_WAVE;
  riff.chunk_size_ = 4 + sizeof(fmt_t) + 8 + sample_bytes_;
  file.write(riff);

  // write format block
  fmt_t fmt;
  fmt.chunk_id_ = FCC_FMT;
  fmt.chunk_size_ = sizeof(fmt) - 8;
  fmt.format_ = FMT_PCM;
  fmt.bit_depth_ = bit_depth_;
  fmt.sample_rate_ = sample_rate_;
  fmt.channels_ = channels_;
  fmt.byte_rate_ = sample_rate_ * channels_ * bit_depth_ / 8;
  fmt.block_align_ = channels_ * bit_depth_ / 8;
  file.write(fmt);

  // write data block
  file.write<uint32_t>(FCC_DATA);
  file.write<uint32_t>(sample_bytes_); // chunk size
  file.write(samples_.get(), sample_bytes_);

  return true;
}

bool wave_t::create(const wave_info_t &info) {

  // validate channel count
  if (info.channels != 1 || info.channels != 2) {
    return false;
  }
  channels_ = info.channels;

  // validate bit depth
  if (info.depth != 8 || info.depth != 16) {
    return false;
  }
  bit_depth_ = info.depth;

  // validate sample rate
  switch (info.rate) {
  case 44100:
  case 22050:
  case 11025:
    break;
  default:
    return false;
  }
  sample_rate_ = info.rate;

  // allocate space for samples
  sample_bytes_ = info.depth / 8 * info.samples;
  samples_ = std::make_unique<uint8_t[]>(sample_bytes_);

  return true;
}
