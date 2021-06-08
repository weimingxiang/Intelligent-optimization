import csv
from Const import *


class Csver():
    def __init__(self):
        pass

    def read_csv(self):
        lst = []
        with open(CSVPATH, "r", newline='') as csvs:
            data = csv.reader(csvs)
            for i in data:
                lst.append(i)
        return lst

    def write_csv(self, data):
        with open(CSVPATH, "w", newline='') as csvs:
            writer = csv.writer(csvs)
            writer.writerow(data)

    def append_csv(self, data):
        with open(CSVPATH, "a", newline='') as csvs:
            writer = csv.writer(csvs)
            writer.writerow(data)


if __name__ == '__main__':
    csver = Csver()
    csver.write_csv([1, 2])
    csver.append_csv([1, 2])
    data = csver.read_csv()
    print(data)
