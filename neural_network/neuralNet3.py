# Neural Net V3 on NIST dataset
# by Steffen Lim
# runs on Python3

import json
import os
import random
from math import ceil
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches


import numpy as np

PATH = os.getcwd()
trainFile = os.path.join(PATH, "optdigits_train.txt")
testFile = os.path.join(PATH, "optdigits_test.txt")

# np.random.seed(1)

class nn_layer(object):
    def __init__(self, output_dim, input_dim=None, initial_weights=None):
        self.has_weights = True
        if initial_weights != None:
            self.weights = initial_weights
        elif input_dim != None:
            self.weights = 2*np.random.rand(input_dim + 1, output_dim) - 1
        else:
            self.has_weights = False
            self.weights = None
        self.set_activation()
    
    def set_activation(self, type='sigmoid'):
        self.activation_type = type
        
    def activation(self, input_z):
        if self.activation_type == 'linear':
            self.node_output = input_z
        else:
            # default to sigmoid
            self.node_output = 1/(1+np.exp(-input_z))

    def back_activation(self):
        if self.activation_type == 'linear':
            self.activation_prime = np.ones_like(self.node_output)
        else:
            # default to sigmoid
            self.activation_prime = self.node_output*(1-self.node_output)

    def forward(self, input_x):
        # inputs are the previous layers ouput after the activation
        # input.shape = (# images, input dimension)
        if self.has_weights:
            if input_x.ndim == 1:
                input_x = input_x.reshape((1, input_x.shape[0]))
            # input_bias contains the input with a row of 1's for the bias
            input_bias = np.insert(input_x, input_x.shape[1], 1, axis=1)
            self.input_bias = input_bias

            self.activation(np.dot(input_bias, self.weights))
        else:
            self.node_output = input_x

    def backward(self, derivative, learning_rate):
        # derivative is the next layer's gradients
        # derivative.shape = (# images, output_dim)
        if self.has_weights:
            self.back_activation()
            # update weights using SGD
            # print("prime", dw.shape)
            # print("dw", dw)

            weights_nobias = self.weights[:-1,:]
            # print("weights_nobias", weights_nobias.shape)

            self.gradient = derivative * self.activation_prime
            dw = np.dot(self.input_bias.T, self.gradient)
            self.gradient = np.dot(self.gradient, weights_nobias.T)
            self.weights = self.weights + dw * learning_rate
    
    def get_node_output(self):
        return self.node_output

    def get_gradient(self):
        return self.gradient

    def get_weights(self):
        return self.weights
    
    def set_weights(self, weights):
        if weights.shape == self.weights.shape:
            self.weights = weights

class neural_net(object):
    def __init__(self, hidden_layers=[32]):
        self.input_size = 64
        self.output_size = 10
        layer_dims = [self.input_size]
        for i in range(len(hidden_layers)):
            layer_dims.append(hidden_layers[i])
        layer_dims.append(self.output_size)

        self.layers = [nn_layer(self.input_size)]
        for i in range(len(layer_dims)-1):
            self.layers.append(nn_layer(layer_dims[i+1], input_dim=layer_dims[i]))

    def forward_function(self, input_x):
        input_a = input_x

        for i in range(len(self.layers)):
            self.layers[i].forward(input_a)
            input_a = self.layers[i].get_node_output()
        self.result = input_a

        max_index = self.result.argmax(axis=1)
        # max_index.shape = (# images,)
        return max_index

    def calculate_loss(self, label_y):
        if label_y.ndim == 1:
            label_y = label_y.reshape((label_y.shape[0], 1))

        self.correct_results = self.result[np.arange(label_y.shape[0]), label_y[:, 0]]
        goal = np.zeros((label_y.shape[0], 10))
        goal[np.arange(label_y.shape[0]), label_y[:, 0]] = 1
        # print("self.correct_results", self.correct_results[10])
        self.loss = -np.log(self.correct_results)

    def backprop(self, label_y, learning_rate):
        if label_y.ndim == 1:
            label_y = label_y.reshape((label_y.shape[0], 1))
        # self.result.shape = (# images, output_dim)
        # self.correct_results.shape = (# images,)
        self.calculate_loss(label_y)

        goal = np.zeros((label_y.shape[0], 10))
        goal[np.arange(label_y.shape[0]), label_y[:, 0]] = 1

        self.gradient = goal - self.result

        # self.gradient.shape = (# images, output_size)
        for i in reversed(range(len(self.layers)-1)):
            self.layers[i+1].backward(self.gradient, learning_rate)
            self.gradient = self.layers[i+1].get_gradient()

    def train(self, train_input, train_label, test_in, test_label, epoch, batch_size):
        learning_rate = 0.001
        self.loss_overtime = []
        self.test_loss = []
        self.accuracy_overtime = []
        for i in range(epoch):
            # calculate the train loss and accuracy
            results = self.forward_function(train_input)
            # print("forward", results[0:16])
            self.calculate_loss(train_label)
            print("epoch: {} loss: {}".format(i, np.average(self.loss)), end='\r')
            self.loss_overtime.append(np.average(self.loss))
            correct = np.sum(np.where(results==train_label.reshape(train_label.shape[0]), np.ones(results.shape), np.zeros(results.shape)))
            self.accuracy_overtime.append(correct/train_label.shape[0])
            # calculate the test loss
            test_result = self.forward_function(test_in)
            self.calculate_loss(test_label)
            self.test_loss.append(np.average(self.loss))

            # run batches
            max_size = train_input.shape[0]
            batch_data = train_input
            for j in range(ceil(max_size / batch_size)):
                begin = batch_size*j
                end = batch_size*(j+1)
                if end > max_size:
                    end = max_size
                result = self.forward_function(batch_data[begin:end])
                self.backprop(train_label[begin:end], learning_rate)
        # final check of train loss and accuracy
        results = self.forward_function(train_input)
        self.calculate_loss(train_label)
        self.loss_overtime.append(np.average(self.loss))
        print("average loss: ", np.average(self.loss))
        correct = np.sum(np.where(results==train_label.reshape(train_label.shape[0]), np.ones(results.shape), np.zeros(results.shape)))
        self.accuracy_overtime.append(correct/train_label.shape[0])
        print("train accuracy: ", correct/train_label.shape[0])

        test_result = self.forward_function(test_in)
        self.calculate_loss(test_label)
        self.test_loss.append(np.average(self.loss))
        
    def save_weights(self, name):
        with open(os.path.join(PATH, "models", "model_"+name+".json"), 'w') as weight_file:
            weight_list = []
            for i in range(len(self.layers)-1):
                weight_list.append(self.layers[i+1].get_weights().tolist())
            json.dump(weight_list, weight_file, indent=4)

    def load_weights(self, name, prefix="model_"):
        with open(os.path.join(PATH, "models", prefix+name+".json")) as weight_file:
            weight_list = json.load(weight_file)
            for i in range(len(self.layers)-1):
                self.layers[i+1].set_weights(np.asarray(weight_list[i]))

    def get_overtime(self):
        # print(len(self.loss_overtime))
        return self.loss_overtime, self.accuracy_overtime
    
    def get_test_loss(self):
        return self.test_loss

'''
Grabs the data and runs any preprocessing
'''
with open(trainFile, 'r') as train_data:
    data = [line.split(',') for line in train_data.readlines()]
    for i in range(len(data)):
        for j in range(len(data[i])):
            data[i][j] = int(data[i][j])
    # print (len(data))
    trainX = np.asarray(data)[:, :-1]
    trainY = np.transpose(np.asarray(data)[:, -1:])
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

def show_forward(ann, test_input, test_output, sample_size, save):
    fig = plt.figure()
    # fig.subtitle("Random samples from forward pass of the Test data", fontsize=16)
    fig.set_figwidth(sample_size)
    results = ann.forward_function(test_input)
    samples = random.sample(range(len(test_input)), sample_size)
    sample_results = results[samples]
    # print ("predict:", test_output.shape[0])
    correct = np.sum(np.where(results==test_output.reshape(test_output.shape[0]), np.ones(results.shape), np.zeros(results.shape)))
    # print("correct: ",correct.shape)
    print("test accuracy: ", correct/test_output.shape[0])
    for i in range(sample_size):
        plot = plt.subplot(1,sample_size,i+1)
        temp = "P: "+str(results[i]) #+", GT: "+str(test_output[i][0])
        # print(temp)
        plot.set_title(temp)
        # print(test_input[samples[i]].shape)
        plt.imshow(np.reshape(test_input[i], (8,8)))
    # plt.show()

    blue_patch = mpatches.Patch(color='blue', label='train loss')
    orange_patch = mpatches.Patch(color='orange', label='test loss')
    plt.legend(handles=[blue_patch, orange_patch])
    # plot_loss = fig.add_subplot(111)
    loss, accu = ann.get_overtime()
    test_loss = ann.get_test_loss()
    plt.xlabel('epoch')
    plt.ylabel('loss')
    plt.plot(np.arange(len(loss)), loss, np.arange(len(test_loss)), test_loss)
    # plt.show()
    plt.savefig(os.path.join(PATH, "figures", "figure_"+save+".png"))

# train the nn
nn = neural_net(hidden_layers=[128, 32])
# Normalizing the data
# trainX -= 8
# testX -= 8
# trainX = trainX / 8
# testX = testX / 8
# trainX = trainX / 16
# testX = testX / 16


nn.train(trainX, trainY, testX, testY, 200, 50)
# nn.load_weights("")

nn.save_weights("w0u0n0z0_200_0")
show_forward(nn, testX, testY, 10)

'''
test2: the first working nn, 500 epochs, h-layers [64, 16], acc: 0.83
test3: set up using 2000 epochs,            acc: 0.955
test4: set up using 8000 epochs,            acc: 0.974958263773
test5: 2000 epochs, h-layer [64, 32, 16],   acc: 0.960489705064
test6: 2000 epochs, h-layer [64, 16, 16],   acc: 0.960489705064
test7: 2000 epochs, h-layer [128, 16, 16],  acc: 0.968280467446
test8: 2000 epochs, h-layer [128, 32, 16],  acc: 0.965498052309
test9: 2000 epochs, h-layer [96, 16, 16],   acc: 0.959933222037
test10: 2000 epochs, h-layer [96, 32, 16],  acc: 0.959933222037
test11: 2000 epochs, h-layer [128, 16],     acc: 0.967723984418
test12: 2000 epochs, h-layer [128, 32],     acc: 0.971619365609
test13: 2000 epochs, h-layer [128, 64],     acc: 0.969949916528

test14: 200 epochs, h-layer [128, 32], Zero-centered Normalized Data  acc: 0.944351697273
test14: 200 epochs, h-layer [128, 32], Non-zero-centered Unormalized Data  acc: 0.927657206455
test14: 200 epochs, h-layer [128, 32], Zero-centered Unormalized Data  acc: 0.918196994992
test14: 200 epochs, h-layer [128, 32], Non-Zero-centered Normalized Data  acc: 0.936560934891

test15: 200 epochs, h-layer [128, 32], Non-Zero-centered Normalize1d Data, figure 1 & 2
test16: 8000 epochs, h-layer [128, 32], figure_3, acc: 0.972732331664

test17: 2000 epochs, h-layer [128, 128, 128, 32], acc: 0.967167501391

'''

for w in range(2):
    for u in range(2):
        for z in range(2):
            if z == 0:
                train_in = trainX - 8
                test_in = testX - 8
            else:
                train_in = trainX
                test_in = testX
            for n in range(2):
                if n == 0:
                    if z == 0:
                        train_in = train_in/8
                        test_in = test_in/8
                    else:
                        train_in = train_in/16
                        test_in = test_in/16
                for epoch_index, epoch_type in enumerate([200, 500, 1000]):
                    for a in range(3):
                        nn = neural_net(hidden_layers=[128, 32])
