import logging
from logging.handlers import TimedRotatingFileHandler
from Const import *


class Log():
    def __init__(self):
        self.root_logger = logging.getLogger("RootLog")
        self.log_format = logging.Formatter('%(asctime)s--%(levelname)s--%(message)s', datefmt='%Y/%m/%d-%H:%M:%S')
        self.log_level = logging.INFO
        self.root_logger.setLevel(self.log_level)
        self.fhandle = TimedRotatingFileHandler(LOGPATH, when="D")
        self.fhandle.setLevel(self.log_level)
        self.fhandle.setFormatter(self.log_format)
        self.chandle = logging.StreamHandler()
        self.chandle.setLevel(self.log_level)
        self.chandle.setFormatter(self.log_format)
        self.root_logger.addHandler(self.chandle)
        self.root_logger.addHandler(self.fhandle)

    def get_logger(self, name):
        return logging.getLogger("RootLog." + name)


if __name__ == '__main__':
    log = Log().get_logger(__name__)
    log.info("hellp")
