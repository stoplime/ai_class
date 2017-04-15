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

class nn_layer(object):
    
    def __init__(self, output_dim, input_dim=None, initial_weights=None):
        self.has_weights = True
        if initial_weights != None:
            self.weights = initial_weights
        elif input_dim != None:
            self.weights = np.random.rand(input_dim + 1, output_dim) * 0.001
        else:
            self.has_weights = False
            self.weights = None
        self.set_activation()
        # List of variables in class
        # self.activation_type holds the squash function type, currently only 'sigmoid'
        # self.node_output is the result of the forward pass, has shape (# images, output_dim)
        # self.activation_prime is the derivative of the activation with self.input_z
        # self.f_product is the dot product of the input and the weights including bias
        # self.weights has a shape (input_dim + 1, output_dim), where the +1 is for the bias
        # self.gradient is the result of the derivative of the forward pass with respect to the input
        # the gradient has shape (# images, input_dim)

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

            self.f_product = np.dot(input_bias, self.weights)
            self.activation(self.f_product)
        else:
            self.node_output = input_x

    def backward(self, derivative, learning_rate):
        # derivative is the next layer's gradients
        # derivative.shape = (# images, output_dim)
        if self.has_weights:
            self.back_activation()
            # update weights using SGD

            # print("activation_prime", self.activation_prime.shape)
            # print("input_bias", self.input_bias.shape)
            # print("derivative", derivative.shape)
            # print("weights", self.weights.shape)
            
            dw = np.dot(self.input_bias.T, self.activation_prime)
            self.weights = self.weights - dw * learning_rate

            weights_nobias = self.weights[:-1,:]
            # print("weights_nobias", weights_nobias.shape)

            self.gradient = np.dot(self.activation_prime * derivative, weights_nobias.T)
            # self.gradient *= derivative
    
    def get_node_output(self):
        return self.node_output

    def get_gradient(self):
        return self.gradient

    def get_weights(self):
        return self.weights

class neural_net(object):
    def __init__(self, hidden_layers=[64,32,16]):
        self.input_size = 64
        self.output_size = 10
        layer_dims = [self.input_size]
        for i in range(len(hidden_layers)):
            layer_dims.append(hidden_layers[i])
        layer_dims.append(self.output_size)

        self.layers = [nn_layer(self.input_size)]
        for i in range(len(layer_dims)-1):
            self.layers.append(nn_layer(layer_dims[i+1], input_dim=layer_dims[i]))
        # Set last layer to softmax (linear)
        self.layers[len(self.layers)-1].set_activation(type="linear")

    def forward_function(self, input_x):
        input_a = input_x

        for i in range(len(self.layers)):
            self.layers[i].forward(input_a)
            input_a = self.layers[i].get_node_output()
        # softmax
        self.softmax_result = self.softmax(input_a)
        max_index = self.softmax_result.argmax(axis=1)
        # max_index.shape = (# images,)
        return max_index

    def calculate_loss(self, label_y):
        if label_y.ndim == 1:
            label_y = label_y.reshape((label_y.shape[0], 1))

        self.correct_softmax = self.softmax_result[np.arange(label_y.shape[0]), label_y[:, 0]]
        # print("self.correct_softmax", self.correct_softmax[10])
        self.loss = -np.log(self.correct_softmax) #+ self.regularization()
        # print("weights", self.layers[1].get_weights()[15,5])

    def backprop(self, label_y, learning_rate):
        if label_y.ndim == 1:
            label_y = label_y.reshape((label_y.shape[0], 1))
        # self.softmax_result.shape = (# images, output_dim)
        # self.correct_softmax.shape = (# images,)
        self.calculate_loss(label_y)

        # set gradient starting with the loss relative to softmax
        self.gradient = -1/self.correct_softmax
        # derivative of softmax
        self.gradient = self.gradient.reshape((self.gradient.shape[0], 1)) * self.softmax_prime(label_y, self.correct_softmax)

        # self.gradient.shape = (# images, output_size)
        for i in reversed(range(len(self.layers)-1)):
            self.layers[i+1].backward(self.gradient, learning_rate)
            self.gradient = self.layers[i+1].get_gradient()

    def softmax(self, input_z):
        # input_z.shape = (# images, output_size)
        return np.exp(input_z)/np.sum(np.exp(input_z))

    def softmax_prime(self, label_y, correct_softmax):
        if correct_softmax.ndim == 1:
            correct_softmax = correct_softmax.reshape((correct_softmax.shape[0], 1))
        d_softmax = -(np.exp(correct_softmax)*np.exp(self.softmax_result))/np.square(np.sum(np.exp(self.softmax_result)))
        d_softmax_correct = correct_softmax*(1-correct_softmax)
        d_softmax[np.arange(label_y.shape[0]), label_y[:, 0]] = d_softmax_correct[0]
        return d_softmax

    def regularization(self):
        total = 0
        for i in range(len(self.layers)-1):
            total += np.sum(np.square(self.layers[i+1].get_weights()))
        return total

    def train(self, train_input, train_label, epoch, batch_size):
        learning_rate = 0.001
        for i in range(epoch):
            print("forward", self.forward_function(train_input))
            # self.forward_function(train_input)
            self.calculate_loss(train_label)
            print("epoch: {} loss: {}".format(i, \
            np.average(self.loss)))
            max_size = train_input.shape[0]
            for j in range(ceil(max_size / batch_size)):
                begin = batch_size*j
                end = batch_size*(j+1)
                if end > max_size:
                    end = max_size
                result = self.forward_function(train_input[begin:end])
                self.backprop(train_label[begin:end], learning_rate)
        # delta_y = self.forward(train_input) - train_output
        self.forward_function(train_input)
        self.calculate_loss(train_label)
        print("average loss: ", np.average(self.loss))

    # def save_weights(self, name):
    #     with open(os.path.join(PATH, "models", "model_"+name+".json"), 'w') as weight_file:
    #         weight_list = []
    #         for i in range(self.num_layers-1):
    #             weight_list.append(self.weights[i].tolist())
    #         json.dump(weight_list, weight_file, indent=4)

    # def load_weights(self, name, prefix="model_"):
    #     with open(os.path.join(PATH, "models", prefix+name+".json")) as weight_file:
    #         weight_list = json.load(weight_file)
    #         for i in range(self.num_layers-1):
    #             self.weights[i] = np.asarray(weight_list[i])

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

# train the nn
nn = neural_net()
# print("shape of trainX: ", trainX.shape)
# print("shape of trainY: ", trainY.shape)
# print("shape of testX: ", testX.shape)
# print("shape of testY: ", testY.shape)
nn.train(trainX, trainY, 1000, 100)
# nn.load_weights("test0")

# plt.imshow(np.reshape(trainX[5], (8,8)))
# plt.show()

def show_forward(ann, test_input, test_output, sample_size):
    fig = plt.figure()
    # fig.subtitle("Random samples from forward pass of the Test data", fontsize=16)
    fig.set_figwidth(sample_size)
    results = ann.forward_function(test_input)
    samples = random.sample(range(len(test_input)), sample_size)
    sample_results = results[samples]
    # print ("predict:", test_output.shape[0])
    correct = np.sum(np.where(results==test_output.reshape(test_output.shape[0]), np.ones(results.shape), np.zeros(results.shape)))
    # print("correct: ",correct.shape)
    print("accuracy: ", correct/test_output.shape[0])
    for i in range(sample_size):
        plot = plt.subplot(1,sample_size,i+1)
        temp = "P: "+str(sample_results[i])+", GT: "+str(test_output[samples[i]][0])
        # print(temp)
        plot.set_title(temp)
        # print(test_input[samples[i]].shape)
        plt.imshow(np.reshape(test_input[samples[i]], (8,8)))
    plt.show()

show_forward(nn, testX, testY, 5)
# nn.save_weights("test0")
