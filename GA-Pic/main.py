# coding=utf-8
import src
import Picture
import Algorithm
from Const import *
from Csv import Csver
from Logger import Log
from Plots import ResultPlot

from Const import GENER

if __name__ == '__main__':
    log = Log().get_logger(__name__)
    note = Csver()
    plots = ResultPlot()
    geners = [Picture.Picture() for i in range(PIC_N)]
    # gens = Yamlw.read_gens()
    lst = [0, 0]  # csv数据结构为lst[0]为基因值，其余为每百代最好适应度
    flst = []
    log.info("初始化完成")
    i = 0
    # 开始迭代
    while True:
        # for i in range(GENER):

        picture = Algorithm.best_filter(geners)  # 获取最好适应度
        flst.append(picture.get_it_fitness())  # 记录最好适应度
        plots.showplot(flst)
        log.info("当前第 %d 代 fitness=%d" % (i, flst[-1]))
        geners = Algorithm.roulette(geners)  # 选择(轮盘赌)
        geners = Algorithm.crossover(geners)  # 交叉
        geners = Algorithm.mutation(geners)  # 变异
        # 每百代进行记录
        if i % 100 == 0:
            # picture = Algorithm.best_filter(geners)#获取最好适应度
            # log.info("当前最好适应度为：%d" % picture.get_it_fitness())
            genlst = []  # 基因记录
            for pic in geners:
                genlst.append(pic.get_it_gens())
            lst[0] = genlst  # 记录基因值
            lst[1] = flst
            note.write_csv(lst)  # 写入CSV
            log.info("数据已记录")
            picture.save_it_img("".join([PICPATH, "\Graph%d" % i]))
            log.info("已生成图片")
        plots.save()
        i += 1
