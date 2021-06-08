#coding=utf-8
from PIL import Image,ImageDraw
import random
from Const import *
'''
__author__ = "HM"
编码定义两个类：
1、带透明度的三角类
2、图片类
'''
class IndePic():
    '''
    80位
    '''
    def __init__(self):
        # self.rand_init()
        self.gen = 0
        self.new_it_gen()

    # def rand_init(self):
    #     #感觉没有必要分别初始化,直接生成80位
    #     self.colorA = random.randrange(0,255)#透明度
    #     self.colorR = random.randrange(0,255)<<8
    #     self.colorG = random.randrange(0,255)<<16
    #     self.colorB = random.randrange(0,255)<<24
    #     self.posAx = random.randrange(0,255)<<32
    #     self.posAy = random.randrange(0,255)<<40
    #     self.posBx = random.randrange(0,255)<<48
    #     self.posBy = random.randrange(0,255)<<56
    #     self.posCx = random.randrange(0,255)<<64
    #     self.posCy = random.randrange(0,255)<<72

    def new_it_gen(self):
        #新生一个基因值
        # self.gen = self.colorA|self.colorR|self.colorG|self.colorB|self.posAx|self.posAy|self.posBx|self.posBy|self.posCx|self.posCy
        self.gen = random.randrange(0,2**80)
        return self.gen

    def get_pos(self):
        self.posAx = (self.gen >> 32) & 0xff
        self.posAy = (self.gen >> 40) & 0xff
        self.posBx = (self.gen >> 48) & 0xff
        self.posBy = (self.gen >> 56) & 0xff
        self.posCx = (self.gen >> 64) & 0xff
        self.posCy = (self.gen >> 72) & 0xff
        return [(self.posAx,self.posAy),(self.posBx, self.posBy),(self.posCx, self.posCy)]

    def get_color(self):
        self.colorA = self.gen & 0xff
        self.colorR = (self.gen >> 8) & 0xff
        self.colorG = (self.gen >> 16) & 0xff
        self.colorB = (self.gen >> 24) & 0xff
        return (self.colorR, self.colorG, self.colorB, self.colorA)

class Picture():
    '''
    80*frags位
    '''
    def __init__(self):
        self.__gens = 0
        self.__fitness = 0
        self.__img = 0
        # 生成FRAGS_N个三角
        self.__frags = [IndePic() for n in range(FRAGS_N)]#初始化三角碎片frags
        self._cal_total_gens()#计算gens
        # self._draw_it()#绘制img
        self._cal_pic_fitness()  # 计算fitness

    def get_it_gens(self):
        '''
        获取基因值
        :return:
        '''
        return self.__gens

    def get_it_fitness(self):
        '''
        获取适应度
        :return:
        '''
        return self.__fitness

    def get_it_img_value(self):
        '''
        获取图片类值<class 'PIL.Image.Image'>
        :return:
        '''
        return self.__img

    def save_it_img(self,name):
        '''
        保存图片
        :param name: 图片名称，需加路径名称
        :return:
        '''
        self.__img.save(r"%s.png" %name)

    def set_it_gens(self,gens):
        '''
        配置基因值，从外部传递进入的基因值，进行相关运算
        :param gens: 从外部传递进入的基因值
        '''
        self.__gens = gens
        # self._cal_frags_from_gen()#计算frags
        # self._draw_it()#绘制img
        self._cal_pic_fitness()#计算fitness

    def _cal_total_gens(self):
        '''
        根据你frags计算基因值
        :return:
        '''
        for i in range(FRAGS_N):
            self.__gens = self.__frags[i].gen|self.__gens<<(80)
            # print(str(i) + "   " + hex(self.frags[i].gen))

    def _cal_frags_from_gen(self):
        #根据基因值倒退frags
        #k值是因为基因组合是堆栈形式进出，如果不对K进行倒数处理，会造成这里的pics[0] = 之前的pics[3]，pics[1] = 之前的pics[2]，绘制图形时造成透明度结合问题
        temp_gen = self.__gens
        for i in range(FRAGS_N):
            k = FRAGS_N - i - 1
            self.__frags[k].gen = temp_gen & 0xffffffffffffffffffff
            # print(str(k) + "   " + hex(self.frags[k].gen))
            temp_gen = temp_gen>>80

    def _draw_it(self):
        #利用PIL绘制图形
        self._cal_frags_from_gen()  # 计算frags
        bg = Image.new("RGBA", PICSIZE, color=(255,255,255,0))
        draw = ImageDraw.Draw(bg)
        draw.polygon([(0, 0), (0, HEIGHT), (WIDTH, HEIGHT), (WIDTH, 0)], fill=(255, 255, 255, 255))#白色背景，不加为透明背景
        for pic in self.__frags:
            ims = Image.new("RGBA", PICSIZE, color=(255,255,255,0))
            draw = ImageDraw.Draw(ims)
            draw.polygon(pic.get_pos(), fill=pic.get_color())
            bg = Image.alpha_composite(bg, ims)
        # bg.save(r"G.png")
        self.__img = bg

    def _cal_pic_score(self):
        '''
        与原图比较获取平均相对分数，分数越小与原图越接近
        纯白与纯黑图片平均分数相差最大为195075
        :param pic_pixels: 图片文件
        :return:score//255//255 平均分数，0-195075
        '''
        score = 0
        pic_pixels = self.__img.load()
        for i in range(WIDTH):
            for j in range(HEIGHT):
                score += ((pic_aim_pixels[i, j][0] - pic_pixels[i, j][0]) ** 2
                          + (pic_aim_pixels[i, j][1] - pic_pixels[i, j][1]) ** 2
                          + (pic_aim_pixels[i, j][2] - pic_pixels[i, j][2]) ** 2)  ###

        return score // 255 // 255

    def _cal_pic_fitness(self):
        '''
        获取适应度，fitness=195075-score
        越大越接近目标图片
        :param pic: <class 'PIL.Image.Image'>
        :return:fitness: 适应度
        '''
        self._draw_it()
        score = self._cal_pic_score()
        self.__fitness = 195075 - score
        return self.__fitness


if __name__ == '__main__':
    pass
    # pic1 = IndePic()
    # pic2 = IndePic()
    # pics = [pic1,pic2]
    # draw_it(pics)

    # pic = Picture()
    # pic.save_it_img("G")
    # print(pic.get_it_gens())
    # pic.set_it_gens(1826059669373588451847133446543070815374508850928812634075804427997353993539556496425542680356390)
    # pic.save_it_img("G1")