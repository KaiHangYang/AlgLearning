import numpy as np

class Relu(object):
    def __init__(self, shape):
        self.eta = np.zeros(shape)
        self.x = np.zeros(shape)
        self.output_shape = shape

    def forward(self, x):
        self.x = x
        return np.maximum(x, 0)

    def gradient(self, eta):
        # X 小于 0的梯度为0
        # 在 大于等于0情况下梯度为1 * eta
        self.eta = eta
        self.eta[self.x<0]=0
        return self.eta
