from PIL import Image
import os

BASEPATH = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))
IMAGEPATH = os.path.join(BASEPATH, "data", "image.png")
RESULTPATH = os.path.join(BASEPATH, "result")
LOGPATH = os.path.join(RESULTPATH, "runlog.log")
CSVPATH = os.path.join(RESULTPATH, "data.csv")
PICPATH = os.path.join(RESULTPATH,"pic")
FIGPATH = os.path.join(RESULTPATH,"fig.png")
YAMLPATH = os.path.join(RESULTPATH, "temp.yaml")

GENER = 10000
PIC_N = 10
FRAGS_N = 100
WIDTH = HEIGHT = 255
P_MUTA = 0.001
P_CROSS = 0.5
PICSIZE = (WIDTH, HEIGHT)

pic_aim = Image.open(IMAGEPATH)
pic_aim_pixels = pic_aim.load()
