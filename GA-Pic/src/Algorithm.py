# 轮盘、交叉、变异算法
import random
# from cython.parallel import *

import Picture
from Const import *


def roulette(pictures):
    '''
    轮盘赌，根据适应度随机再生成对应数量的picture
    :param pictures: 原始picture列表
    :return: pictures: 处理后picture列表
    '''
    temp_lst = []
    accumulator_fitness = [0 for i in range(PIC_N)]
    # 获取适应累加度
    accumulator_fitness[0] = pictures[0].get_it_fitness()
    for i in range(1, PIC_N):
        accumulator_fitness[i] = pictures[i].get_it_fitness(
        ) + accumulator_fitness[i - 1]
    # 获取轮盘赌后的基因值
    for i in range(PIC_N):
        rd = random.randrange(0, accumulator_fitness[PIC_N - 1])
        for j in range(PIC_N):
            if rd < accumulator_fitness[j]:
                temp_lst.append(pictures[j].get_it_gens())
                break
    # 反馈基因值值picture
    for i in range(PIC_N):
        pictures[i].set_it_gens(temp_lst[i])
    # 确保最佳适应度基因必有一个遗传
    # pic = best_filter(pictures)
    # pictures[random.randrange(PIC_N)].set_it_gens(pic.get_it_gens())
    return pictures


def crossover(pictures):
    '''
    基因交叉，随机选择两个进行基因交换，交换次数为PIC_N，交换概率为50%
    :param pictures: 原始picture列表
    :return: pictures: 处理后picture列表
    '''
    list = [i for i in range(PIC_N)]
    temp = Picture.Picture()
    for i in range(PIC_N // 2):
        i, k = random.sample(list, 2)
        list.remove(i)
        list.remove(k)
        if random.random() <= P_CROSS:
            tempa, tempb = cross(
                pictures[i].get_it_gens(), pictures[k].get_it_gens())
            temp.set_it_gens(tempa)
            if temp.get_it_fitness() > pictures[i].get_it_fitness():
                pictures[i].set_it_gens(tempa)
            temp.set_it_gens(tempb)
            if temp.get_it_fitness() > pictures[k].get_it_fitness():
                pictures[k].set_it_gens(tempb)
    return pictures


def cross(gen1, gen2, max_bit=80):
    """
        随机抽取一些位置，随机交换两个数的二进制位，交换位数不固定
        1^1=0 1^0=1
        t = a^b
        a^t = b
        b^t = a
        所以完全交换的话只需要连续异或两次即可
        只交换某一些位的话需要找出不同的点位（gen1^gen2），在将这些点位随机变为0即取消交换(&random.randrange(0,16))，剩下点位即为要交换的点位
    """
    # t = (gen1^gen2)&random.randrange(0,1<<(max_bit*FRAGS_N+1))#plan A
    t = gen1 ^ gen2
    for i in range(max_bit):
        t &= ~(0x1 << random.randrange(0, 80 * FRAGS_N))
    return gen1 ^ t, gen2 ^ t


def mutation(pictures):
    '''
    基因变异
    1^1=0 0^1=1
    :param pictures: 原始picture列表
    :return: pictures: 处理后picture列表
    '''
    temp = Picture.Picture()
    for pic in pictures:
        if random.random() > 0:
            gen = pic.get_it_gens()
            # t = random.randrange(0,1<<(max_bit*FRAGS_N+1))#效果不好
            # # print(bin(t))
            # gen = gen^t
            k = 0
            numS = random.randrange(0, 80 * FRAGS_N - 1)
            numE = random.randrange(numS, 80 * FRAGS_N)
            for i in range(numS, numE):  # 80*PIC_N
                if random.random() <= P_MUTA:
                    k |= 1 << (i)
            gen = gen ^ k
            temp.set_it_gens(gen)
            if temp.get_it_fitness() > pic.get_it_fitness():
                pic.set_it_gens(gen)
    return pictures


def best_filter(pictures):
    '''
    筛选适应度最好的图片
    :param pictures: 原始picture列表
    :return picture:适应度最好的图片
    '''
    picture = pictures[0]
    for pic in pictures:
        if picture.get_it_fitness() < pic.get_it_fitness():
            picture = pic
    return picture


if __name__ == '__main__':
    pics = [Picture.Picture() for i in range(PIC_N)]
    # pic = Picture.Picture()
    # for i in pics:
    #     print(bin(i.get_it_gens()))
    # pics = crossover(pics)
    # for i in pics:
    #     print(bin(i.get_it_gens()))
    # print(bin(pics[0].get_it_gens()))
    # pics = mutation(pics)
    # print(bin(pics[0].get_it_gens()))
    # # for i in range(PIC_N):
    # #     pics[i].save_it_img("G%d" %i)
    # #     print("g%d"  %pics[i].get_it_fitness())
    # # for k in range(5):
    # #     pics = roulette(pics)
    # #     for i in range(PIC_N):
    # #         pics[i].save_it_img("Q%d%d" %(k,i))
    # #         print("g%d"  %pics[i].get_it_fitness())
    # record_gens(pics)
    pics = mutation(pics)
