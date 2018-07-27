#!/usr/bin/python
#coding:utf-8
import yaml
def read_yaml():
    # 你的yaml格式文件路径
    path = 'conf.yaml'

    with open(path,'r') as file:
        # 将yaml格式内容转换成 dict类型
        load_data = yaml.load(file.read())
        print(load_data)
        print(load_data.get('complex'))
        s= load_data['complex']['languages']
        print s

    #with open(path, 'r') as file:
    #    # load_all返回的是一个迭代器对象，需要自己去遍历获取每一个段的转换后才python对象。
    #    load_all_data = yaml.load_all(file.read())
    #    # 遍历迭代器
    #    for data in load_all_data:
    #        print(data)
    #        print(data.get('complex'))

if __name__ == '__main__':
    read_yaml()
