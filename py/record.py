  
import soundcard as sc
import wave
import scipy.io.wavfile
import math
import numpy as np

lb = sc.all_microphones(include_loopback=True)[1]
print(lb)
data = []
i = 0
with lb.recorder(samplerate=44100) as rcdr, wave.open('recorded.wav', 'wb') as f:
    f.setnchannels(2)
    f.setsampwidth(2)
    f.setframerate(44100)

    while i < 100:
        d = rcdr.record()
        print(d.shape)
        data.append(d)
        i += 1
    final = np.concatenate(data, axis=0)
    print(final.shape)
    scipy.io.wavfile.write("recorded2.wav", 44100, final)



