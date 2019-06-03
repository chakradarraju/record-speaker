import soundcard as sc

speakers = sc.all_speakers()
default_speaker = sc.default_speaker()
mics = sc.all_microphones()
default_mic = sc.default_microphone()


with default_mic.recorder(samplerate=44100) as mic, default_speaker.player(samplerate=44100) as sp:
    while True:
        data = mic.record(numframes=1024)
        sp.play(data)
