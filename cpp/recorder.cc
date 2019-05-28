#include <iostream>

#include "portaudio.h"
#include "recorder.h"

using namespace std;

namespace strings {

struct CallbackData {
  unsigned long index;
  unsigned long size;
  short* data;
};

void CopyData(const short* rptr, short* wptr, int frames, CallbackData* data) {
    for (int i = 0; i < frames; i++) *wptr++ = rptr == NULL ? 0 : *rptr++;
}

static int RecordCallback(const void* input_buffer, void* output_buffer,
        unsigned long frames_per_buffer, const PaStreamCallbackTimeInfo* time_info,
        PaStreamCallbackFlags status_flags, void *user_data) {
    auto data = (CallbackData*)user_data;
    const short* rptr = (const short*)input_buffer;
    short* wptr = &data->data[data->index];
    auto frames_left = data->size - data->index;
    auto frames = frames_left <= frames_per_buffer ? frames_left : frames_per_buffer;
    CopyData(rptr, wptr, frames, data);
    data->index += frames;
    auto ret = frames_left <= frames_per_buffer ? paComplete : paContinue;
    return ret;
}

bool RecordInternal(short* recording, int secs) {
    cout << "Recording to: " << recording << " secs: " << secs << endl;
    PaStream* stream;
    PaStreamParameters inputParams;
    inputParams.device = Pa_GetDefaultInputDevice();
    if (inputParams.device == paNoDevice) {
        cout << "No default input device" << endl;
        return false;
    }
    auto device_info = Pa_GetDeviceInfo(inputParams.device);
    cout << "Got default device: " << device_info->name 
        << " inp: " << device_info->maxInputChannels
        << " out: " << device_info->maxOutputChannels
        << " hostApi: " << device_info->hostApi
        << " id: " << inputParams.device << endl;
    inputParams.channelCount = 1;
    inputParams.sampleFormat = paInt16;
    inputParams.suggestedLatency = device_info->defaultLowInputLatency;
    inputParams.hostApiSpecificStreamInfo = NULL;
    CallbackData data;
    data.data = recording;
    data.size = secs * 44100;
    data.index = 0;
    auto err = Pa_OpenStream(
        &stream,
        &inputParams,
        NULL,
        44100,
        4410,
        paClipOff,
        RecordCallback,
        &data);
    if (err != paNoError) return false;
    cout << "Opened stream, status: " << err << endl;
    err = Pa_StartStream(stream);
    cout << "Started stream, status: " << err << endl;
    if (err != paNoError) return false;
    cout << "Started recording, start speaking " << stream << endl;
    while ((err = Pa_IsStreamActive(stream)) == 1) Pa_Sleep(500);
    cout << "Done recording, status: " << err << endl;
    if (err < 0) return false;
    err = Pa_CloseStream(stream);
    cout << "Closed stream, status: " << err << endl;
    if (err != paNoError) return false;
    return true;
}

bool Record(short* recording, int secs) {
    bool success = Pa_Initialize() == paNoError;
    if (success) success = RecordInternal(recording, secs);
    Pa_Terminate();
    return success;
}

}  // namespace strings