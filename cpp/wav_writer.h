#ifndef BC6F9108_E134_1525_DA39_208FF7F71FD5
#define BC6F9108_E134_1525_DA39_208FF7F71FD5

#include <iostream>
#include "wav_info.h"
#define HEADER_SIZE 44

namespace strings {

template<typename SampleType>
void WriteSample(std::ostream& o, SampleType sample, unsigned size = sizeof(SampleType)) {
  for (; size; --size, sample >>= 8) o.put(static_cast<char>(sample & 0xFF));
}

template<typename SampleType>
bool WriteSamples(std::ostream& o, SampleType* samples, int n) {
  for (int i = 0; i < n; i++) {
    WriteSample(o, samples[i]);
    if (o.bad()) return false;
  }
  return true;
}

template<typename SampleType>
bool WriteSamples(std::ostream& o, SampleType* samples, const WavInfo& info) {
  return WriteSamples(o, samples, info.num_samples());
}

void WriteRIFFHeaders(std::ostream& o, const WavInfo& info) {
  o << "RIFF";
  WriteSample(o, info.payload_size() + HEADER_SIZE - 8, 4);
  o << "WAVE";
}

void WriteFmtHeaders(std::ostream& o, const WavInfo& info) {
  o << "fmt ";
  WriteSample(o, 16, 4);  // Size of rest of fmt bytes
  WriteSample(o, 1, 2);  // Linear quantization
  WriteSample(o, info.num_channels, 2);
  WriteSample(o, info.sample_rate, 4);
  WriteSample(o, info.sample_rate * info.block_size() * 8, 4);
  WriteSample(o, info.block_size(), 2);
  WriteSample(o, info.sample_size * 8, 2);
  o << "data";
  WriteSample(o, info.payload_size(), 4);
}

bool WriteHeaders(std::ostream& o, const WavInfo& info) {
  o.seekp(0);
  WriteRIFFHeaders(o, info);
  WriteFmtHeaders(o, info);
  return !o.bad();
}

template<typename SampleType>
bool WriteWav(std::ostream& o, SampleType* samples, WavInfo& info) {
  // See: http://soundfile.sapp.org/doc/WaveFormat/
  if (!WriteHeaders(o, info)) return false;
  return WriteSamples(o, samples, info);
}

template<typename SampleType>
bool AppendWav(std::ostream& o, SampleType* samples, WavInfo& info) {
  o.seekp(0, std::ios_base::end);
  auto pos = o.tellp();
  if (pos == -1 || ((pos - HEADER_SIZE) % info.block_size()) != 0) return false;
  return WriteSamples(o, samples, info);
}

}  // namespace strings

#endif /* BC6F9108_E134_1525_DA39_208FF7F71FD5 */