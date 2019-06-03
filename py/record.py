  
import soundcard as sc
import wave
import scipy.io.wavfile
import math
import numpy as np

lb = sc.all_microphones(include_loopback=True)[0]
print(lb)
data = []
i = 0
with lb.recorder(samplerate=44100) as rcdr:
   #` , wave.open('recorded.wav', 'wb') as f:
   # f.setnchannels(2)
   # f.setsampwidth(2)
   # f.setframerate(44100)

    while i < 1000:
        d = rcdr.record()
        data.append(d)
        i += 1
    final = np.concatenate(data, axis=0)
    print(np.max(final))
    print(np.min(final))
    print(final.shape)
    print(final.dtype)
    finalint = np.int16(final * 32767)
    print(finalint.dtype)
    print(finalint)
    scipy.io.wavfile.write("recorded.wav", 44100, finalint)



