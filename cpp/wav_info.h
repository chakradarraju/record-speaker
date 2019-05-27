#ifndef BA4CE8D2_2134_1525_DA3F_C6CF9810D3E7
#define BA4CE8D2_2134_1525_DA3F_C6CF9810D3E7

namespace strings {

struct WavInfo {
  long num_blocks;
  long sample_rate;
  int num_channels;
  int sample_size;

  WavInfo(long nb, long sr, int nc, int ss) : num_blocks(nb), sample_rate(sr), num_channels(nc), sample_size(ss) {}

  long payload_size() const {
    return num_samples() * sample_size;
  }

  long block_size() const {
    return num_channels * sample_size;
  }

  long num_samples() const {
    return num_blocks * num_channels;
  }

  void print() const {
    std::cout << "num_blocks: " << num_blocks
      << " sample_rate: " << sample_rate
      << " num_channels: " << num_channels
      << " sample_size: " << sample_size << std::endl;
  }
};

}  // namespace strings

#endif /* BA4CE8D2_2134_1525_DA3F_C6CF9810D3E7 */
