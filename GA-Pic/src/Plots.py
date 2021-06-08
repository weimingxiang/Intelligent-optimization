import matplotlib.pyplot as plt
import time


class ResultPlot():
    def __init__(self):
        self.plt = plt
        self.plt.figure()

    def showplot(self, data):
        ydata = data
        xdata = range(len(data))
        xdata = [x*100 for x in xdata]
        self.plt.clf()
        self.plt.plot(xdata, ydata)
        self.plt.draw()
        self.plt.pause(0.1)

    def show(self):
        self.plt.show()

    def save(self):
        self.plt.savefig("figG.png")


if __name__ == '__main__':
    data = [172381, 172308, 172598, 172815, 173166, 173177, 173177, 173485, 174294, 176510, 176510, 176775, 176932,
            176483, 176491, 176695, 176746, 176970, 176865, 177105, 176974, 177154, 177098, 177226, 177367, 177369,
            177358]
    p = ResultPlot()
    for i in range(len(data)):
        time.sleep(1)
        d = data[:i]
        p.showplot(d)
    p.show()
