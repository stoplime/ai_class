import os
import matplotlib.pyplot as plt
import numpy as np
from math import ceil

PATH = os.getcwd()
trainFile = os.path.join(PATH, "optdigits_train.txt")
testFile = os.path.join(PATH, "optdigits_test.txt")

class neural_net(object):
    def __init__(self):
        self.input_size = 64
        self.output_size = 10

        self.layers = []
        self.layers.append(self.input_size)
        self.layers.append(64)
        self.layers.append(32)
        self.layers.append(16)
        self.layers.append(self.output_size)

        self.num_layers = len(self.layers)

        self.weights = []
        self.inputA = []
        self.outputZ = []
        for i in range(self.num_layers-1):
            self.weights.append(np.random.rand(self.layers[i], self.layers[i+1])*0.001)
            self.inputA.append(0)
            self.outputZ.append(0)
        # for final output
        self.inputA.append(0)
    
    def forward(self, input_x):
        self.inputA[0] = input_x
        for i in range(self.num_layers-1):
            self.outputZ[i] = np.dot(self.inputA[i], self.weights[i])
            self.inputA[i+1] = self.activation(self.outputZ[i])
        self.outputY = self.inputA[self.num_layers-1]
        return self.outputY
    
    def activation(self, input_z):
        # sigmoid
        return 1/(1+np.exp(-input_z))
    
    def activation_prime(self, input_z):
        # sigmoid
        return np.exp(-input_z)/((1+np.exp(-input_z))**2)

    def loss_function(self, delta_y):
        return 0.5*np.sum((delta_y)**2)
    
    def get_gradient(self, delta_y):
        deltas = [0 for i in range(self.num_layers)]
        weights_prime = [0 for i in range(self.num_layers-1)]
        deltas[self.num_layers-1] = delta_y
        first = True
        for i in reversed(range(self.num_layers-1)):
            if first:
                first = False
                deltas[i] = deltas[i+1]*self.activation_prime(self.outputZ[i])
            else:
                deltas[i] = np.dot(deltas[i+1], self.weights[i+1].T)*self.activation_prime(self.outputZ[i])
            weights_prime[i] = np.dot(self.inputA[i].T, deltas[i])
        return weights_prime

def train(ann, train_input, train_output, epoch, batch_size):
    learning_rate = -0.001
    for i in range(epoch):
        delta_y = ann.forward(train_input) - train_output
        print("epoch: ", i," loss: ", np.average(ann.loss_function(delta_y)))
        max_size = train_input.shape[0]
        for j in range(ceil(max_size / batch_size)):
            begin = batch_size*j
            end = batch_size*(j+1)
            if end > max_size:
                end = max_size
            for k in range(batch_size):
                delta_y = ann.forward(train_input[begin:end]) - train_output[begin:end]
                dW = ann.get_gradient(delta_y)
                for l in range(ann.num_layers-1):
                    # print("dW[",l,"]: ", dW[l].shape)
                    ann.weights[l] += dW[l]*learning_rate
    delta_y = ann.forward(train_input) - train_output
    print("average loss: ", np.average(ann.loss_function(delta_y)))

        

with open(trainFile, 'r') as train_data:
    data = [line.split(',') for line in train_data.readlines()]
    for i in range(len(data)):
        for j in range(len(data[i])):
            data[i][j] = int(data[i][j])
    # print (len(data))
    trainX = np.asarray(data)[:,:-1]
    trainY = np.transpose(np.asarray(data)[:,-1:])
    trainY = np.reshape(trainY, (-1, 1))
    # print (trainY)

with open(testFile, 'r') as test_data:
    data = [line.split(',') for line in test_data.readlines()]
    for i in range(len(data)):
        for j in range(len(data[i])):
            data[i][j] = int(data[i][j])
    # print (len(data))
    testX = np.asarray(data)[:,:-1]
    testY = np.transpose(np.asarray(data)[:,-1:])[0]
    testY = np.reshape(testY, (-1, 1))

# train the nn
nn = neural_net()
print("shape of trainX: ", trainX.shape)
print("shape of trainY: ", trainY.shape)
print("shape of testX: ", testX.shape)
print("shape of testY: ", testY.shape)
train(nn, trainX, trainY, 10, 100)

# plt.imshow(np.reshape(trainX[5], (8,8)))
# plt.show()



    
