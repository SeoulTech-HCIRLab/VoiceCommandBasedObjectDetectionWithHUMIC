import os
import numpy as np

import IPython
import requests
import torch
import torchaudio

import pyaudio
import wave

from collections import deque
from queue import Queue, Full
from array import array
import concurrent.futures

torch.random.manual_seed(0)
device = torch.device("cuda")

CHUNK = 1024
BUFF = CHUNK * 10
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 16000

SILENCE_THREASHOLD = 30000

class AUDIO_UTILS:
    def __init__(self):
        self.bundle = torchaudio.pipelines.WAV2VEC2_ASR_BASE_960H
        self.model = self.bundle.get_model().to(device)
        self.p = pyaudio.PyAudio()

        self.SPEECH_FILE = "test.wav"

        self.labels = self.bundle.get_labels()

    def wave_to_str(self, frames):

        waveFile = wave.open(self.SPEECH_FILE, 'wb')
        waveFile.setnchannels(1)
        waveFile.setsampwidth(self.p.get_sample_size(pyaudio.paInt16))
        waveFile.setframerate(RATE)
        waveFile.writeframes(b''.join(frames))
        waveFile.close()

        waveform, sample_rate = torchaudio.load(self.SPEECH_FILE)
        waveform = waveform.to(device)

        if sample_rate != self.bundle.sample_rate:
            waveform = torchaudio.functional.resample(waveform, 
                    sample_rate, self.bundle.sample_rate)

        #decoding audio
        blank = 0
        with torch.inference_mode():
            print("speech recognition..")
            emission, _ = self.model(waveform)

            indices = torch.argmax(emission[0], dim=-1)  # [num_seq,]
            indices = torch.unique_consecutive(indices, dim=-1)
            indices = [i for i in indices if i != blank]
            output = "".join([self.labels[i] for i in indices])

        target = output.replace("|", " ").lower()

        return target

    def listen(self):

        frames = list()
        SILENCE_SECONDS = 3

        # open stream
        audio = pyaudio.PyAudio()
        stream = audio.open(
            format=FORMAT,
            channels=CHANNELS,
            rate=RATE,
            input=True,
            frames_per_buffer=CHUNK
        )

        # FIXME: release initial noisy data (1sec)
        for _ in range(0, int(RATE / CHUNK)):
            data = stream.read(CHUNK, exception_on_overflow=False)

        is_started = False
        vol_que = deque(maxlen=SILENCE_SECONDS)

        print('start listening')
        while True:
            try:
                # define temporary variable to store sum of volume for 1 second
                vol_sum = 0

                # read data for 1 second in chunk
                for _ in range(0, int(RATE / CHUNK)):
                    data = stream.read(CHUNK, exception_on_overflow=False)

                    # get max volume of chunked data and update sum of volume
                    vol = max(array('h', data))
                    vol_sum += vol

                    # if status is listening, check the volume value
                    if not is_started:
                        if vol >= SILENCE_THREASHOLD:
                            print('start of speech detected')
                            is_started = True

                    # if status is speech started, write data
                    if is_started:
                        frames.append(data)

                # if status is speech started, update volume queue and check silence
                if is_started:
                    vol_que.append(vol_sum / (RATE / CHUNK) < SILENCE_THREASHOLD)
                    if len(vol_que) == SILENCE_SECONDS and all(vol_que):
                        print('end of speech detected')
                        break
            except Full:
                pass

        # close stream
        stream.stop_stream()
        stream.close()
        audio.terminate()

        return frames
