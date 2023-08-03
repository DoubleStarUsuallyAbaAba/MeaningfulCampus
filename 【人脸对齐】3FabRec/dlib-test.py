# 1.导入库
import cv2
import dlib
import matplotlib.pyplot as plt
import os
left = []
top = []
right = []
bottom = []
filenamelist = []
small = 0.8
big = 1.2
# 2.方法：显示图片
def show_image(img, title):
    # BGR to RGB
    img_RGB = img[:, :, ::-1]
    plt.title(title)
    plt.imshow(img_RGB)
    plt.axis('off')


# 3.方法：绘制图片中检测到的人脸
def plot_rectangle(img, faces):
    for face in faces:
        # 画出人脸框，蓝色（BGR色彩体系），画笔宽度为2
        #cv2.rectangle(img, (face.left(), face.top()), (face.right(), face.bottom()), (0, 0, 255), 2)
        left.append(face.left())
        top.append(face.top())
        right.append(face.right())
        bottom.append(face.bottom())
    for i in range(0,len(left)):
        print(left[i])
        print(top[i])
        print(right[i])
        print(bottom[i])
        print('-------------------------\n')
        print(int(left[i]*small))
        print(int(top[i]*small))
        print(int(right[i]*big))
        print(int(bottom[i]*big))
        print('========================================\n')
    return img


# 4.主函数：
def dlib_main():
    for filename in os.listdir("/home/ai/3FabRec/images/"):
        print(filename)
        # 5.读取图片
        image = cv2.imread('/home/ai/3FabRec/images/'+filename)
        # 6.灰度转换
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

        # 7.调用dlib库中的检测器
        face_detect = dlib.get_frontal_face_detector()
        faces = face_detect(gray, 1)

        # 8.绘制图片中检测到的人脸
        img_result = plot_rectangle(image.copy(), faces)

        # 9.创建画布
        #plt.figure(figsize=(9, 6))
        #plt.suptitle('face detection with dlib', fontsize=14, fontweight='bold')
        # 10.显示最终的检测结果
        for i in range(0,len(left)):
            padding = 55
            print(image.shape[0])
            print(image.shape[1])
            if(top[i]<padding):
                top[i]=0
            else:
                top[i] -= padding
            if (left[i] < padding):
                left[i] = 0
            else:
                left[i] -= padding
            if (image.shape[1]-right[i] < padding):
                right[i] = image.shape[1]
            else:
                right[i] += padding
            if (image.shape[0] - bottom[i] < padding):
                bottom[i] = image.shape[0]
            else:
                bottom[i] += padding
            picture = img_result[
                int(top[i]):int(bottom[i]),
                int(left[i]):int(right[i])]
            if(picture.size != 0):
                cv2.imwrite(
                   '/home/ai/3FabRec/picture_for_demo/'
                    +filename[:-4]+'-'+str(i)+filename[-4:], picture)
        left.clear()
        top.clear()
        right.clear()
        bottom.clear()

if __name__ == "__main__":
    dlib_main()