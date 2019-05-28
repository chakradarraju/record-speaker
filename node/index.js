const portAudio = require('naudiodon');
const fs = require('fs');
const wavefile = require('wavefile');

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
  var ai = new portAudio.AudioIO({
    inOptions: {
      channelCount: 1,
      sampleFormat: portAudio.SampleFormat16Bit,
      sampleRate: 44100,
      deviceId: -1 // Use -1 or omit the deviceId to select the default device
    }
  });

  // Create a write stream to write out to a raw audio file
  var ws = fs.createWriteStream(file);
  ws.write(getWavHeaders(1, 44100, '16'), null, () => {
    console.log('starting');
    //Start streaming
    ai.pipe(ws);
    ai.start();
    console.log('done');
  });

  process.on('SIGINT', () => {
    console.log('Received SIGINT. Stopping recording.');
    ai.quit();
  });
}

const getWavHeaders = function(channels, rate, sampleSize) {
  const wav = new wavefile();
  wav.fromScratch(channels, rate, sampleSize, []);
  return wav.toBuffer();
}

record('record.wav');
