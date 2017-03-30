# NIST Artificial Neural Network
import json
import os
from math import ceil
import random

import matplotlib.pyplot as plt
import numpy as np

PATH = os.getcwd()
trainFile = os.path.join(PATH, "optdigits_train.txt")
testFile = os.path.join(PATH, "optdigits_test.txt")

class neural_net(object):
    def __init__(self, hidden_layers=[64,32,16]):
        self.input_size = 64
        self.output_size = 10

        self.layers = []
        self.layers.append(self.input_size)
        for i in range(len(hidden_layers)):
            self.layers.append(hidden_layers[i])
        self.layers.append(self.output_size)

        self.num_layers = len(self.layers)

        self.weights = []
        self.inputA = []
        self.outputZ = []
        for i in range(self.num_layers-1):
            self.weights.append(np.random.rand(self.layers[i]+1, self.layers[i+1])*0.0001)
            self.inputA.append(0)
            self.outputZ.append(0)
        # for final output
        self.inputA.append(0)
    
    def forward(self, input_x):
        # input_x has shape (# of images, image data)
        if input_x.ndim == 1:
            self.inputA[0] = input_x.reshape((1, input_x.shape[0]))
        else:
            self.inputA[0] = input_x
        for i in range(self.num_layers-1):
            # print("inputA: ", self.inputA[i].shape)
            self.inputA[i] = np.insert(self.inputA[i], self.inputA[i].shape[1], 1, axis=1) # this is for the bias
            # print("inputA: ", self.inputA[i].shape)
            # print("weights: ", self.weights[i].shape)
            self.outputZ[i] = np.dot(self.inputA[i], self.weights[i])
            # print("outputZ: ", self.outputZ[i].shape)
            self.inputA[i+1] = self.activation(self.outputZ[i])
        outputY = self.inputA[self.num_layers-1]
        return outputY
    
    def activation(self, input_z):
        # sigmoid
        return 1/(1+np.exp(-input_z))
    
    def activation_prime(self, input_z):
        # sigmoid
        return np.exp(-input_z)/((1+np.exp(-input_z))**2)

    def loss_function(self, input_x, label_y, delta=0.1):
        # softmax
        # input_x.shape = (# of images, image data)
        # label_y.shape = (# of images, )
        # print ("input_x: ",input_x.shape)
        # print ("label_y: ",label_y.shape)

        self.dW = np.zeros_like(self.weights)

        ''' self.outputY.shape = (# of images, 10)'''
        self.outputY = self.forward(input_x)
        # print ("outputY: ",self.outputY.shape)
        ''' correct.shape = (# of images, 1)'''
        # print("label_y[:,0]: ", label_y[:,0].shape)
        correct = self.outputY[np.arange(label_y.shape[0]),label_y[:,0]]
        correct = correct.reshape((correct.shape[0],1))
        # print ("correct: ",correct.shape)
        ''' unnormalized_probabilities.shape = (# of images, 10)'''
        unnormalized_probabilities = np.exp(self.outputY)
        correct_exp = np.exp(correct)
        ''' probability.shape = (# of images, 1)'''
        probability = correct_exp/np.sum(unnormalized_probabilities)
        probability = probability.reshape((probability.shape[0],1))
        print("probability: ",probability.shape)
        return probability + self.regularization()

    def regularization(self):
        total = 0
        for i in range(len(self.weights)):
            total += np.sum(np.square(self.weights[i]))
        return total
    
    def get_gradient(self, losses):
        # losses.shape = (batch_size, )

        # deltas are a list of derivatives of the next delta with respect to the input
        # f = Wx
        # df = x
        # deltas[layers-1].shape = ()
        deltas = [0 for i in range(self.num_layers)]
        weights_prime = [0 for i in range(self.num_layers-1)]
        deltas[self.num_layers-1] = losses
        first = True
        for i in reversed(range(self.num_layers-1)):
            if first:
                first = False
                deltas[i] = deltas[i+1]*self.activation_prime(self.outputZ[i])
            else:
                deltas[i] = np.dot(deltas[i+1], self.weights[i+1].T)*self.activation_prime(self.outputZ[i])
            weights_prime[i] = np.dot(self.inputA[i].T, deltas[i])
        return weights_prime

    def train(self, train_input, train_output, epoch, batch_size):
        learning_rate = -0.001
        for i in range(epoch):
            # delta_y = self.forward(train_input) - train_output
            print("epoch: ", i," loss: ", np.average(self.loss_function(train_input, train_output)))
            max_size = train_input.shape[0]
            for j in range(ceil(max_size / batch_size)):
                begin = batch_size*j
                end = batch_size*(j+1)
                if end > max_size:
                    end = max_size
                for k in range(batch_size):
                    # delta_y.shape = (batch_size, )
                    delta_y = self.loss_function(train_input[begin:end], train_output[begin:end])
                    print("delta_y: ", delta_y.shape)
                    dW = self.get_gradient(delta_y)
                    for l in range(self.num_layers-1):
                        # print("dW[",l,"]: ", dW[l].shape)
                        self.weights[l] += dW[l]*learning_rate
        delta_y = self.forward(train_input) - train_output
        print("average loss: ", np.average(self.loss_function(delta_y)))

    def save_weights(self, name):
        with open(os.path.join(PATH, "models", "model_"+name+".json"), 'w') as weight_file:
            json.dump(self.weights, weight_file, indent=4)

    def load_weights(self, name, prefix="model_"):
        with open(os.path.join(PATH, "models", prefix+name+".json")) as weight_file:
            self.weights = json.load(weight_file)

'''
Grabs the data and runs any preprocessing
'''
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
# nn.train(trainX, trainY, 10, 100)

# plt.imshow(np.reshape(trainX[5], (8,8)))
# plt.show()

def show_forward(ann, test_input, test_output, sample_size):
    fig = plt.figure()
    # fig.subtitle("Random samples from forward pass of the Test data", fontsize=16)
    fig.set_figwidth(sample_size)
    results = ann.forward(test_input)
    predict = np.argmax(results, axis=1)
    samples = random.sample(range(len(test_input)), sample_size)
    sample_results = results[samples]
    sample_predict = predict[samples]
    # print ("predict:", test_output.shape[0])
    correct = np.sum(np.where(predict==test_output.reshape(test_output.shape[0]), np.ones(predict.shape), np.zeros(predict.shape)))
    # print("correct: ",correct.shape)
    print("accuracy: ", correct/test_output.shape[0])
    # print(sample_results.shape)
    for i in range(sample_size):
        plot = plt.subplot(1,sample_size,i+1)
        temp = "P: "+str(sample_predict[i])+", GT: "+str(test_output[samples[i]][0])
        # print(temp)
        plot.set_title(temp)
        # print(test_input[samples[i]].shape)
        plt.imshow(np.reshape(test_input[samples[i]], (8,8)))
    plt.show()

show_forward(nn, testX, testY, 5)