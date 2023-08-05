import time
import pyaudio
import wave
from pydub import AudioSegment
import os
from threading import Thread
import numpy as np
from iat_ws_python3 import main as audio2Text
import audioop

def record_audio(output_filename, duration=59):
    # 设置参数
    format = pyaudio.paInt16
    channels = 2
    rate = 16000 # 必须16k或8k
    chunk = 1024
    SILENT_THRESHOLD = 2000  # 设置静默阈值

    # 初始化PyAudio对象
    audio = pyaudio.PyAudio()

    # 打开音频流

    print('开始录音')
    # # 获取当前时间
    # start = time.time()
    stream = audio.open(format=format, channels=channels,
                        rate=rate, input=True,
                        frames_per_buffer=chunk)

    # 录制音频数据
    frames = []

    silence_counter = 0 # 记录静默帧的数量

    for i in range(0, int(rate / chunk * duration)):
        data = stream.read(chunk)
        frames.append(data)

        # 检测音频帧的音量
        volume = audioop.rms(data, 2)  # 计算音频帧的均方根值
        if volume < SILENT_THRESHOLD:
            silence_counter += 1
        else:
            silence_counter = 0

        # 如果连续静默帧超过一定数量，认为用户停止说话，结束录音
        if silence_counter > int(rate / chunk * 1):  # 0.5秒的静默时间
            break

    # 停止音频流
    print("录制完成")
    stream.stop_stream()
    stream.close()
    audio.terminate()

    # 保存音频为WAV文件
    # 如果不存在文件夹则创建
    if not os.path.exists('./audio/'):
        os.makedirs('./audio/')
    wav_filename = './audio/'+output_filename + ".wav"
    wave_file = wave.open(wav_filename, 'wb')
    wave_file.setnchannels(channels)
    wave_file.setsampwidth(audio.get_sample_size(format))
    wave_file.setframerate(rate)
    wave_file.writeframes(b''.join(frames))
    wave_file.close()

    print("音频保存为：" + wav_filename)

def wav2mp3(filepath, savepath):
    #  如果不存在文件夹则创建
    if not os.path.exists(savepath):
        os.makedirs(savepath)
    sourcefile = AudioSegment.from_wav(filepath)
    filename = filepath.split('/')[-1].split('.wav')[0].replace(' ', '_') + '.mp3'
    print('原文件：',filepath)
    print('导出文件：',savepath + filename)
    sourcefile.export(savepath + filename, format="mp3")

def main(filename, duration=59):
    record_audio(filename, duration)
    wav2mp3('./audio/'+filename+'.wav', './mp3/')
    text =  audio2Text('./mp3/'+filename+'.mp3')
    # 删除文件
    # os.remove('./audio/'+filename+'.wav')
    # os.remove('./mp3/'+filename+'.mp3')
    return text

# if __name__ == '__main__':
#     main('test', 5)
