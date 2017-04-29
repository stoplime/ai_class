# SARSA GridWorld System: Steffen Lim
import json
import os
import random

import matplotlib.pyplot as plt
import numpy as np

random.seed(1)
np.random.seed(1)

class sarsa(object):
    def __init__(self, target=None, gridSize=(20, 20)):
        if target == None:
            target = np.random.randint(2, size=(gridSize[0], gridSize[1]))
        self.gridWorld = np.zeros((gridSize[0]+1, gridSize[1]+1))
        self.gridWorld[20,:] = -1;
        self.gridWorld[:,20] = -1;
        self.qTable = np.random.rand(gridSize[0], gridSize[1], 4) * 0.01
        self.eTable = np.zeros((gridSize[0], gridSize[1], 4))
        self.target = target
        self.currentState = (random.randint(0, self.gridWorld.shape[0]-2), random.randint(0, self.gridWorld.shape[1]-2))
        self.gridWorld[target] = 1

        self.learningRate = 0.01
        self.decayRate = 0.99
        self.exploreRatio = 0.9

    def choose_action(self, state=None):
        # returns an action
        if state == None:
            state = self.currentState
        
        if random.uniform(0, 1) < self.exploreRatio:
            # move randomly
            return random.randint(0,3)
        else:
            # move according to qTable
            return self.qTable[state].argmax(axis=0)
    
    def apply_action(self, state=None):
        # returns a state
        if state == None:
            state = self.currentState

        if self.action == 0:
            # if state[0]+1 >= self.gridWorld.shape[0]-1:
            #     return (0, state[1])
            return (state[0]+1, state[1])
        elif self.action == 1:
            # if state[1]+1 >= self.gridWorld.shape[1]-1:
            #     return (state[0], 0)
            return (state[0], state[1]+1)
        elif self.action == 2:
            # if state[0]-1 <= -self.gridWorld.shape[0]:
            #     return (0, state[1])
            return (state[0]-1, state[1])
        else:
            # if state[1]-1 <= -self.gridWorld.shape[1]:
            #     return (state[0], 0)
            return (state[0], state[1]-1)

    def draw_grid(self):
        world = np.copy(self.gridWorld)
        world[self.currentState] = 100
        print(" ")
        for i in range(self.gridWorld.shape[0]-1):
            for j in range(self.gridWorld.shape[1]-1):
                if(world[i,j] == 100):
                    print(".", end="")
                elif(world[i,j] == 1):
                    print("*", end="")
                else:
                    print("0", end="")
                print(" ", end="")
            print(" ")

    def train(self, epoch):
        for ep in range(epoch):
            # initialize state and action
            self.currentState = (random.randint(0, self.gridWorld.shape[0]-2), random.randint(0, self.gridWorld.shape[1]-2))
            self.action = self.choose_action()
            # loop till find gold
            print("epoch", ep)
            self.draw_grid()
            self.alive = True
            while(self.alive):
                # self.draw_grid()
                print(self.currentState)
                state_prime = self.apply_action()
                reward = self.gridWorld[state_prime]
                if(reward != 0):
                    self.alive = False;
                action_prime = self.choose_action(state=state_prime)

                current_index = (self.currentState[0], self.currentState[1], self.action)
                prime_index = (state_prime[0], state_prime[1], action_prime)

                delta = reward + self.exploreRatio*self.qTable[prime_index] - self.qTable[current_index]
                self.eTable[current_index] = self.eTable[current_index] + 1

                # print(delta.shape)
                # update q and e tables
                self.qTable = self.qTable + self.learningRate * delta * self.eTable
                self.eTable = self.eTable * self.exploreRatio * self.decayRate

                # update currentState and action
                self.currentState = state_prime
                self.action = action_prime
            print("done")

sarsa_test = sarsa(target=(15,3))
sarsa_test.train(100)

