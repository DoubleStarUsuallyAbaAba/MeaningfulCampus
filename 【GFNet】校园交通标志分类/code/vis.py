import torchvision.transforms as transforms
import torchvision.datasets as datasets

import torch.multiprocessing
torch.multiprocessing.set_sharing_strategy('file_system')

from utils import *
from network import *
from configs import *

import math
import argparse

import models.resnet as resnet
import models.densenet as densenet
from models import create_model

import os
import cv2
from image_net_classes import get_image_net_class

current_folder='tmp'

device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

parser = argparse.ArgumentParser(description='Inference code for GFNet')

parser.add_argument('--num_classes', default=1000, type=int,
                    help='num_classes of dataset (ImageNet)')

parser.add_argument('--data_url', default='/root/autodl-tmp/GFNet/data/', type=str,
                    help='path to the dataset (ImageNet)')

parser.add_argument('--output_dir', default='/root/autodl-tmp/output/', type=str,
                    help='path to the output vis images') 

parser.add_argument('--checkpoint_path', default='/root/autodl-tmp/GFNet/output/GF-resnet50_patch-size-96_T5_train-stage3/model_best.pth.tar', type=str,
                    help='path to the pre-train model (default: none)')

args = parser.parse_args()

def get_image_list(folder_path):
    image_list = []
    for filename in sorted(os.listdir(folder_path)):
        if filename.endswith(".jpg") or filename.endswith(".png") or filename.endswith(".JPEG"):
            image_list.append(filename)
    return image_list

def draw_bounding_box(image_path, output_path, tl_x, tl_y, rb_x, rb_y, bbox_text):
    image = cv2.imread(image_path)
    # cv2.rectangle(image, (tl_x, tl_y), (rb_x, rb_y), (0, 255, 0), 2)
    # 定义文本框中文字的位置
    text_pos = (15, 25)
    # 定义文本框中文字的颜色和字体
    text_color = (0, 255, 0)  # 绿色
    font = cv2.FONT_HERSHEY_SIMPLEX
    # 在文本框上添加文字
    cv2.putText(image, bbox_text, text_pos, font, 0.7, text_color, 2)
    cv2.imwrite(output_path, image)


def main():

    # load pretrained model
    checkpoint = torch.load(args.checkpoint_path)

    print('checkpoint.keys()')
    print(checkpoint.keys())

    try:
        #model_arch = checkpoint['model_name']
        #patch_size = checkpoint['patch_size']
        #prime_size = checkpoint['patch_size']
        #maximum_length = len(checkpoint['flops'])

        model_arch = 'resnet50'
        patch_size = 96
        prime_size = 96
        maximum_length = 40
    except:
        print('Error: \n'
              'Please provide essential information'
              'for customized models (as we have done '
              'in pre-trained models)!\n'
              'At least the following information should be Given: \n'
              '--model_name: name of the backbone CNNs (e.g., resnet50, densenet121)\n'
              '--patch_size: size of image patches (i.e., H\' or W\' in the paper)\n'
              '--flops: a list containing the Multiply-Adds corresponding to each '
              'length of the input sequence during inference')

    model_configuration = model_configurations[model_arch]

    # create model
    assert 'resnet' in model_arch

    model = resnet.resnet50(pretrained=False)
    model_prime = resnet.resnet50(pretrained=False)            

    visdir = args.data_url + 'vis/'

    normalize = transforms.Normalize(mean=[0.485, 0.456, 0.406],
                                        std=[0.229, 0.224, 0.225])

    vis_loader = torch.utils.data.DataLoader(
        datasets.ImageFolder(visdir, transforms.Compose([
            transforms.Resize(int(model_configuration['image_size']/model_configuration['crop_pct']), interpolation = model_configuration['dataset_interpolation']),
            transforms.CenterCrop(model_configuration['image_size']),
            transforms.ToTensor(),
            normalize])),
        batch_size=1, shuffle=False, num_workers=1, pin_memory=False)

    state_dim = model_configuration['feature_map_channels'] * math.ceil(patch_size/32) * math.ceil(patch_size/32)
    
    memory = Memory()
    policy = ActorCritic(model_configuration['feature_map_channels'], state_dim, model_configuration['policy_hidden_dim'], model_configuration['policy_conv'])
    fc = Full_layer(model_configuration['feature_num'], model_configuration['fc_hidden_dim'], model_configuration['fc_rnn'])

    model = nn.DataParallel(model.cuda())
    model_prime = nn.DataParallel(model_prime.cuda())
    policy = policy.cuda()
    fc = fc.cuda()

    state_dict = checkpoint['model_state_dict']
    new_state_dict = {k if 'module.' in k else 'module.' + k: v for k, v in state_dict.items()}
    # print(checkpoint.keys())
    model.load_state_dict(new_state_dict)

    prime_state_dict = checkpoint['model_prime_state_dict']
    new_prime_state_dict = {k if 'module.' in k else 'module.' + k: v for k, v in prime_state_dict.items()}
    # print(checkpoint.keys())
    model_prime.load_state_dict(new_prime_state_dict)


    # model.load_state_dict(checkpoint['model_state_dict'])
    # model_prime.load_state_dict(checkpoint['model_prime_state_dict'])
    fc.load_state_dict(checkpoint['fc'])
    policy.load_state_dict(checkpoint['policy'])

    images = get_image_list('/root/autodl-tmp/GFNet/data/vis/'+current_folder+'/')

    print('generate logits on test samples...')
    generate_logits(model_prime, model, fc, memory, policy, vis_loader, maximum_length, prime_size, patch_size, model_arch, images)
    print('finish ...')



def generate_logits(model_prime, model, fc, memory, policy, dataloader, maximum_length, prime_size, patch_size, model_arch, images):

    logits_list = []
    targets_list = []

    model.eval()
    model_prime.eval()
    fc.eval()

    print('successfully read following {} images\n'.format(len(images)), images)
    length_of_dataloader = len(dataloader)
    for i, (x, target) in enumerate(dataloader):
        print('------------------------------')
        print('current index is {}/{}'.format(i+1, length_of_dataloader))

        logits_temp = torch.zeros(maximum_length, x.size(0), args.num_classes)

        target_var = target.cuda()
        input_var = x.cuda()

        input_prime = get_prime(input_var, prime_size, model_configurations[model_arch]['prime_interpolation'])

        with torch.no_grad():

            output, state = model_prime(input_prime)
            
            output = fc(output, restart=True)

            logits_temp[0] = F.softmax(output, 1)

            for patch_step in range(1, maximum_length):

                with torch.no_grad():
                    if patch_step == 1:
                        action = policy.act(state, memory, restart_batch=True)
                    else:
                        action = policy.act(state, memory)
                patches = get_patch(input_var, action, patch_size)
                output, state = model(patches)
                output = fc(output, restart=False)
                output = F.softmax(output, 1)

                if patch_step == maximum_length-1:
                    assert input_var.size(2) == input_var.size(3) # H==W
                    image_size = input_var.size(2)
                    patch_coordinate = torch.floor(action * (image_size - patch_size)).int()
                    # print('patch_coordinate shape', patch_coordinate.shape)
                    # print('patch_coordinate', patch_coordinate)
                    pred_confidence, pred_index = output.topk(1, 1, True, True)
                    # print('predict confidence shape', pred_confidence.shape)
                    # print('predicted confidence ', pred_confidence)
                    pred_index = pred_index.t()
                    # print('predict index shape', pred_index.shape)
                    # print('predict index', pred_index)
                    print('predicted class is <'+get_image_net_class(pred_index) + '> ,and the confidence is {}'.format(pred_confidence))

                    image_path = os.path.join(args.data_url, 'vis', current_folder, images[i])
                    output_path = os.path.join(args.output_dir, images[i])

                    ori_image = cv2.imread(image_path)

                    width = math.floor(patch_size / x.shape[3] * ori_image.shape[1])
                    height = math.floor(patch_size / x.shape[2] * ori_image.shape[0])
                    xx = math.ceil(patch_coordinate[0][1].int() / x.shape[3] * ori_image.shape[1])
                    yy = math.ceil(patch_coordinate[0][0].int() / x.shape[2] * ori_image.shape[0])
                    tl_x = max(xx, 0)
                    tl_y = max(yy, 0)
                    tl_x = min(tl_x, ori_image.shape[1])
                    tl_y = min(tl_y, ori_image.shape[0])
                    rb_x = max(xx+width, 0)
                    rb_y = max(yy+height, 0)
                    rb_x = min(rb_x, ori_image.shape[1])
                    rb_y = min(rb_y, ori_image.shape[0])

                    bbox_text = get_image_net_class(pred_index)+' '+str(pred_confidence[0][0].cpu().float())[6:]

                    draw_bounding_box(image_path, output_path, tl_x, tl_y, rb_x, rb_y, bbox_text)


        memory.clear_memory()


if __name__ == '__main__':
    main()
