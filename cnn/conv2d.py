import numpy as np
import cv2
import os
import sys
import math
from functools import reduce

def im2col(image, ksize, stride):
    # image.shape = [batch, height, width, channel]
    image_col = []

    for i in range(0, image.shape[1] - ksize + 1, stride):
        for j in range(0, image.shape[2] - ksize + 1, stride):
            cur_col = image[:, i:i+ksize, j:j+ksize, :].reshape([-1])
            image_col.append(cur_col)
    image_col = np.array(image_col)
    return image_col

class Conv2D(object):
    def __init__(self, shape, output_channels, ksize=3, stride=1, method="VALID"):
        # shape = [batchsize, height, width, channel]
        self.input_shape = shape
        self.output_channels = output_channels
        self.input_channels = shape[-1]
        self.method = method
        self.batchsize = shape[0]
        self.stride = stride
        self.ksize = ksize

        weight_scale = math.sqrt(reduce(lambda x, y: x * y, shape) / self.output_channels)
        self.weights = np.random.standard_normal((ksize, ksize, self.input_channels, self.output_channels)) / weight_scale
        self.bias = np.random.standard_normal(self.output_channels) / weight_scale
        height, width = shape[1], shape[2]

        if method == "VALID":
            # the result is [(height - ksize + 1) / stride, (width - ksize + 1) / stride]
            self.eta = np.zeros([self.batchsize, (height - ksize + 1) // stride, (width - ksize + 1) // stride, self.output_channels])
        elif method == "SAME":
            self.eta = np.zeros([self.batchsize, height // stride, width // stride, self.output_channels])

        self.w_gradient = np.zeros_like(self.weights)
        self.b_gradient = np.zeros_like(self.bias)

        self.output_shape = self.eta.shape

        if (height - ksize) % stride != 0:
            print("Error stride for such tensor!")

        if (width - ksize) % stride != 0:
            print("Error stride for such tensor!")

    def forward(self, x):
        col_weight = self.weights.reshape([-1, self.output_channels])
        if self.method == "SAME":
            # pad zeros
            x = np.pad(x, ((0, 0), (self.ksize // 2), (self.ksize // 2), (0, 0)), "constant", constant_values=0)
        self.col_image = []
        conv_out = np.zeros_like(self.eta)

        for i in range(self.batchsize):
            img_i = x[i][np.newaxis]
            self.col_image_i = im2col(img_i, self.ksize, self.stride)
            conv_out[i] = np.reshape(np.dot(self.col_image_i, col_weight) + self.bias, self.eta[0].shape)
            self.col_image.append(self.col_image_i)

        # 记录X
        self.col_image = np.array(self.col_image)
        return conv_out

    def gradient(self, eta):
        self.eta = eta
        col_eta = np.reshape(eta, [self.batchsize, -1, self.output_channels])
        for i in range(self.batchsize):
            # 可以根据矩阵求导法则导出下面的偏导式子
            # https://zhuanlan.zhihu.com/p/24709748
            # 用全微分公式来退矩阵偏导会好很多
            # 其中 tr(AT*B) 两阵的点积
            # Ycol = Xcol * Wcol + bcol
            # Xcol is constant
            # dY = Xcol * dWcol
            # dJ = tr((pianJ / pianY)T * dY)
            #    = tr(etaT * Xcol * dWcol)
            #    = tr((XcolT * eta)T * dWcol)
            # pianJ / pianWcol = XcolT * eta
            # 如何理解下面式子呢
            # col_image[i].T shape is [NUM, Y] (NUM is ksize * ksize * Cin) col_eta[i] shape is [Y, Cout]
            # the the dot result is [ksize, ksize, Cin, Cout] 也就是weight的梯度
            self.w_gradient += np.dot(self.col_image[i].T, col_eta[i]).reshape(self.weights.shape)
        # bias 其实同理 不过需要注意的是bias 特征图所有的width 和height共享，不同通道不同
        # 也就是bias的shape 是[1, 1, 1, Cout]
        self.b_gradient += np.sum(col_eta, axis[0, 1])

        # 还有就是如何将梯度回传这个问题了
        # 这个回传方法是根据https://zhuanlan.zhihu.com/p/40951745 避免了对im2col的逆向计算
        if self.method == "VALID":
            pad_eta = np.pad(self.eta, ((0, 0), (self.ksize - 1, self.ksize - 1), (self.ksize - 1, self.ksize - 1), (0, 0)), "constant", constant_values = 0)
        elif self.method == "SAME":
            pad_eta = np.pad(self.eta, ((0, 0), (self.ksize // 2, self.ksize // 2), (self.ksize // 2, self.ksize // 2), (0, 0)), "constant", constant_values = 0)

        # rotate 180 degree
        flip_weights = np.flipud(np.fliplr(self.weights))
        flip_weights = np.transpose(flip_weights, axes=[0, 1, 3, 2])
        col_flip_weights = flip_weights.reshape([-1, self.input_channels])
        col_pad_eta = np.array([im2col(pad_eta[i][np.newaxis], self.ksize, self.stride) for i in range(self.batchsize)])
        next_eta = np.dot(col_pad_eta, col_flip_weights)
        next_eta = np.reshape(next_eta, self.input_shape)
        return next_eta

    def backward(self, alpha=0.00001, weight_decay=0.0004):
        # alpha is learning rate
        # weight_decay equal add l2 regularization
        # gradient 推倒可以推出这个weight_decay w1 = (1-weight_decay) * w0 - g
        self.weights *= (1 - weight_decay)
        # self.bias # 一般bias不加正则，加了正则之后网络会欠拟合
        self.weights = self.weights - alpha * self.w_gradient
        self.bias = self.bias - alpha * self.b_gradient

        self.w_gradient = np.zeros_like(self.w_gradient)
        self.b_gradient = np.zeros_like(self.b_gradient)
