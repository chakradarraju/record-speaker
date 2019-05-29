const portAudio = require('naudiodon');
const fs = require('fs');
const wavefile = require('wavefile');
const streamBuffers = require('stream-buffers');

const play = function(file) {
  // Create an instance of AudioIO with outOptions, which will return a WritableStream
  var ao = new portAudio.AudioIO({
    outOptions: {
      channelCount: 2,
      sampleFormat: portAudio.SampleFormat16Bit,
      sampleRate: 16000,
      deviceId: -1 // Use -1 or omit the deviceId to select the default device
    }
  });
   
  // Create a stream to pipe into the AudioOutput
  // Note that this does not strip the WAV header so a click will be heard at the beginning
  var rs = fs.createReadStream(file);
   
  // Start piping data and start streaming
  rs.pipe(ao);
  ao.start();
}

const record = function(file) {
  // Create an instance of AudioIO with inOptions, which will return a ReadableStream
  const channels = 2;
  const rate = 44100;
  var ai = new portAudio.AudioIO({
    inOptions: {
      channelCount: channels,
      sampleFormat: portAudio.SampleFormat16Bit,
      sampleRate: rate,
      deviceId: -1,  // Use -1 or omit the deviceId to select the default device
    }
  });

  const buffer = new streamBuffers.WritableStreamBuffer();
  ai.pipe(buffer);
  ai.start();

  process.on('SIGINT', () => {
    console.log('Received SIGINT. Stopping recording.');
    ai.quit(() => {
      const ws = fs.createWriteStream(file);
      const wav = new wavefile();
      wav.fromScratch(channels, rate, '16', new Int16Array(buffer.getContents().buffer));
      ws.write(wav.toBuffer());
      ws.end();
      ws.on('close', () => {
        console.log('bytes written: ' + ws.bytesWritten);
      });
    });
  });
}

record('record.wav');
