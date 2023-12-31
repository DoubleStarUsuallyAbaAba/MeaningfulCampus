from __future__ import print_function
import os
import logging


def init_log(output_dir):
    """
    初始化日志文件
    :param output_dir:日志文件路径
    :return:
    """
    logging.basicConfig(level=logging.DEBUG,
                        format='%(asctime)s %(message)s',
                        datefmt='%Y%m%d-%H:%M:%S',
                        filename=os.path.join(output_dir, 'log.log'),
                        filemode='w')
    console = logging.StreamHandler()
    console.setLevel(logging.INFO)
    logging.getLogger('').addHandler(console)
    return logging


if __name__ == '__main__':
    pass
