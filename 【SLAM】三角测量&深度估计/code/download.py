import sys
import requests
from bs4 import BeautifulSoup

choice='depth'

if choice=='depth':
    end_char = '.png'
    save_dir='/root/autodl-tmp/portland_hotel/depth/'
    html_file_name = 'Index of _data_Portland_hotel_depth.html'
elif choice=='image':
    end_char = '.jpg'
    save_dir='/root/autodl-tmp/portland_hotel/image/'
    html_file_name = 'Index of _data_Portland_hotel_image.html'
else:
    print("unsupported download choice!\n")
    sys.exit(0)

# 读取HTML文件
with open(html_file_name, 'r') as file:
    html_content = file.read()

# 创建BeautifulSoup对象
soup = BeautifulSoup(html_content, 'html.parser')

# 查找所有的img标签
img_tags = soup.find_all('a')

# 存储图片链接的数组
image_links = []

# 遍历img标签，提取PNG图片链接
for img in img_tags:
    src = img.get('href')
    if src.endswith(end_char):
        image_links.append(src)

# 打印图片链接数组
# print(image_links)
image_count = len(image_links)
print('get {} image_links'.format(image_count))

# 下载图片
fail_download = 0
current_process = 0
for link in image_links:
    response = requests.get(link)
    if response.status_code == 200:
        # 提取文件名
        filename = link.split('/')[-1]

        # 保存图片
        with open(save_dir+filename, 'wb') as file:
            file.write(response.content)
            # print(f"已下载图片: {filename}")
    else:
        fail_download += 1
        print(f"无法下载图片: {link}")
    current_process += 1
    print('downloading...{}/{}'.format(current_process, image_count))

print(choice+' download has finished in '+save_dir + '. and {} download failed !\n'.format(fail_download))
