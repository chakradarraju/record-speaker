
#include <iostream>
#include <fstream>

#include "wav_writer.h"
#include "wav_info.h"
#include "recorder.h"
#include "exception_handler.h"

using namespace std;

typedef short int16;

int main() {
  strings::InstallHandler();
  const string filename = "record.wav";
  const long rate = 44100;
  long secs_to_rec = 2;
  auto info = strings::WavInfo(secs_to_rec * rate, rate, 1, sizeof(int16));
  int16* samples = (int16*)malloc(info.payload_size());
  if (samples == NULL) {
    cout << "Unable to allocate space" << endl;
    return 1;
  }
  cout << "Starting recording..." << endl;
  if (!strings::Record(samples, secs_to_rec)) {
    cout << "Failed to record" << endl;
    return 1;
  }
  ofstream f(filename, ios::binary);
  bool write_success = strings::WriteWav(f, samples, info);
  cout << (write_success ? "Success" : "Failed") << " writing to " << filename << endl;
  return 0;
}
