import os
import cv2

def extract_frames(video_path, output_folder):
    # 打开视频文件
    video = cv2.VideoCapture(video_path)
    
    # 确保视频文件已经成功打开
    if not video.isOpened():
        print("无法打开视频文件:", video_path)
        return
    
    # 创建输出文件夹（如果不存在）
    os.makedirs(output_folder, exist_ok=True)
    
    # 逐帧提取并保存图像
    frame_count = 0
    while True:
        # 读取一帧图像
        ret, frame = video.read()
        
        # 检查是否成功读取到帧
        if not ret:
            break
        
        # 生成帧图像文件名
        frame_filename = f"frame_{frame_count}.jpg"
        
        # 保存帧图像到输出文件夹
        frame_path = os.path.join(output_folder, frame_filename)
        cv2.imwrite(frame_path, frame)
        
        frame_count += 1
    
    # 释放视频对象
    video.release()
    
    print("帧提取完成")

def create_video(frames_folder, output_video_path, fps=10.0):
    # 获取所有帧图像文件的路径
    frame_files = [os.path.join(frames_folder, frame) for frame in os.listdir(frames_folder) if frame != '.ipynb_checkpoints']
    frame_files = sorted(frame_files)
    print(frame_files)
    
    # 检查是否找到帧图像文件
    if len(frame_files) == 0:
        print("未找到帧图像文件")
        return
    
    # 读取第一帧图像，获取图像尺寸
    sample_frame = cv2.imread(frame_files[0])
    height, width, _ = sample_frame.shape
    
    # 创建视频编码器
    fourcc = cv2.VideoWriter_fourcc(*"mp4v")  # 使用MP4编码器
    video_writer = cv2.VideoWriter(output_video_path, fourcc, fps, (width, height))
    
    # 逐帧写入视频
    for frame_file in frame_files:
        frame = cv2.imread(frame_file)
        video_writer.write(frame)
    
    # 释放视频编码器对象
    video_writer.release()
    
    print("视频合成完成")



video_path = "input_video.mp4"
output_folder = "/root/autodl-tmp/output"
output_video_path = "output_video.mp4"

# 提取帧图像
# extract_frames(video_path, output_folder)

# 创建新视频
os.remove(output_video_path)
create_video(output_folder, output_video_path)

