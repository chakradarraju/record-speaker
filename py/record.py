import soundcard as sc
import wave

lb = sc.all_microphones(include_loopback=True)[1]
rcdr = lb.recorder(44100)

with wave.open('recorded.wav', 'wb') as f:
    f.setnchannels(1)
    f.setsampwidth(2)
    f.setframerate(44100)
    f.setnframes(44100)
    f.writeframes(rcdr.record(44100))
