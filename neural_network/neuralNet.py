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
    def __init__(self, hidden_layers=[32]):
        self.input_size = 64
        self.output_size = 10

        np.random.seed(10)

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
            self.weights.append(np.random.rand(self.layers[i]+1, self.layers[i+1])*1e-3)
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

    def loss_function_forward(self, input_x):
        # softmax
        # input_x.shape = (# of images, image data)
        # label_y.shape = (# of images, )
        # print ("input_x: ",input_x.shape)
        # print ("label_y: ",label_y.shape)

        ''' self.outputY.shape = (# of images, self.output_size)'''
        self.outputY = self.forward(input_x)
        # print ("outputY: ",self.outputY.shape)
        # print ("correct: ",correct.shape)
        ''' unnormalized_probabilities.shape = (# of images, 10)'''
        unnormalized_probabilities = np.exp(self.outputY)

        probabilities = unnormalized_probabilities/np.sum(unnormalized_probabilities)
        # print("probabilities", probabilities.shape)
        probabilities = -np.log(probabilities)

        return probabilities + self.regularization()

    def loss_function(self, input_x, label_y):
        # softmax
        # input_x.shape = (# of images, image data)
        # label_y.shape = (# of images, )
        # print ("input_x: ",input_x.shape)
        # print ("label_y: ",label_y.shape)

        # self.softmax_dW = np.zeros((input_x.shape[0], self.output_size))

        ''' self.outputY.shape = (# of images, self.output_size)'''
        self.outputY = self.forward(input_x)
        # print ("outputY: ",self.outputY.shape)
        ''' correct.shape = (# of images, 1)'''
        # print("label_y[:,0]: ", label_y[:,0].shape)
        correct = self.outputY[np.arange(label_y.shape[0]), label_y[:, 0]]
        correct = correct.reshape((correct.shape[0], 1))
        # print ("correct: ",correct.shape)
        ''' unnormalized_probabilities.shape = (# of images, 10)'''
        unnormalized_probabilities = np.exp(self.outputY)
        correct_exp = np.exp(correct)
        # print ("correct_exp: ",correct_exp.shape)
        ''' correct_probability.shape = (# of images, 1)'''
        correct_probability = correct_exp/np.sum(unnormalized_probabilities)
        correct_probability = correct_probability.reshape((correct_probability.shape[0], 1))
        # print("correct_probability: ", correct_probability.shape)

        probabilities = unnormalized_probabilities/np.sum(unnormalized_probabilities)
        # print("probabilities", probabilities.shape)
        probabilities = -np.log(probabilities)

        softmax_dW = -1/probabilities
        self.softmax_dW = softmax_dW * -np.exp(self.outputY)*correct_exp/np.sum(np.square(unnormalized_probabilities))
        self.softmax_dW[np.arange(label_y.shape[0]), label_y[:, 0]] = (softmax_dW * correct_probability)[:,0]
        # print("self.softmax_dW ", self.softmax_dW.shape) 

        return probabilities + self.regularization()

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
                # print("deltas[i+1] ", deltas[i+1].shape)
                # print("self.activation_prime(self.outputZ[i]) ", \
                # self.activation_prime(self.outputZ[i]).shape)
                deltas[i] = deltas[i+1]*self.activation_prime(self.outputZ[i])
            else:
                # print("deltas[i+1] ", deltas[i+1].shape)
                # print("self.weights[i+1].T ", self.weights[i+1].T.shape)
                # print("self.outputZ[i] ", self.outputZ[i].shape)
                active = self.activation_prime(self.outputZ[i])
                deltas[i] = np.dot(deltas[i+1], self.weights[i+1].T)[:,:-1] * active
                # print("self.activation_prime(self.outputZ[i]) ", \
                # np.insert(active, active.shape[1], 1, axis=1)
                # print("deltas", deltas[i][10,10])
                # print("inputA", self.inputA[i][10,10])
            weights_prime[i] = np.dot(self.inputA[i].T, deltas[i])
            # print("weights_prime[i] ", weights_prime[i].shape)
        return weights_prime

    def train(self, train_input, train_output, epoch, batch_size):
        learning_rate = .001
        for i in range(epoch):
            # delta_y = self.forward(train_input) - train_output
            print("epoch: {} loss: {}".format(i, np.average(self.loss_function_forward(train_input))), end="\r")

            # print("weight test: ", self.weights[0][10,10])

            max_size = train_input.shape[0]
            for j in range(ceil(max_size / batch_size)):
                begin = batch_size*j
                end = batch_size*(j+1)
                if end > max_size:
                    end = max_size
                # delta_y.shape = (batch_size, 1)
                delta_y = self.loss_function(train_input[begin:end], train_output[begin:end])
                # print("delta_y: ", delta_y.shape)
                # batch_loss_bool = np.zeros((batch_loss.shape[0], 10))
                # batch_loss_bool[np.arange(batch_loss.shape[0]), batch_loss[:,0]] = 1
                # batch_delta = np.where(batch_loss_bool == 1, np.ones(batch_loss_bool.shape)-sco, )
                dW = self.get_gradient(self.softmax_dW)
                for k in range(self.num_layers-1):
                    # print("dW[",k,"]: ", dW[k].shape)
                    # print("weights[",k,"]: ", self.weights[k].shape)
                    # print("delta test:", (dW[k]*learning_rate)[0,0])
                    delta_dw = self.weights[k]*dW[k]*learning_rate
                    # if k == 0 and begin == 0: print(delta_dw[10,10], "\t", self.weights[k][10,10])
                    self.weights[k] -= delta_dw
            # print("weight test: ", self.weights[0][10,10])
        # delta_y = self.forward(train_input) - train_output
        print("epoch: {} loss: {}".format(epoch, np.average(self.loss_function_forward(train_input))))
        print("average loss: ", np.average(self.loss_function_forward(train_input)))

    def save_weights(self, name):
        with open(os.path.join(PATH, "models", "model_"+name+".json"), 'w') as weight_file:
            weight_list = []
            for i in range(self.num_layers-1):
                weight_list.append(self.weights[i].tolist())
            json.dump(weight_list, weight_file, indent=4)

    def load_weights(self, name, prefix="model_"):
        with open(os.path.join(PATH, "models", prefix+name+".json")) as weight_file:
            weight_list = json.load(weight_file)
            for i in range(self.num_layers-1):
                self.weights[i] = np.asarray(weight_list[i])

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
print("shape of trainX: ", trainX.shape)
print("shape of trainY: ", trainY.shape)
print("shape of testX: ", testX.shape)
print("shape of testY: ", testY.shape)
nn.train(trainX, trainY, 1000, 100)
# nn.load_weights("test0")

# plt.imshow(np.reshape(trainX[5], (8,8)))
# plt.show()

def show_forward(ann, test_input, test_output, sample_size):
    fig = plt.figure()
    # fig.subtitle("Random samples from forward pass of the Test data", fontsize=16)
    fig.set_figwidth(sample_size)
    results = ann.loss_function_forward(test_input)
    predict = np.argmax(results, axis=1)
    # print("results: ", results)
    # print("predict", predict.shape)
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
# nn.save_weights("test0")