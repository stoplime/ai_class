# SARSA GridWorld System: Steffen Lim
import json
import os
import random
from progressbar import ProgressBar, Bar, Percentage

import matplotlib.pyplot as plt
import numpy as np
import tkinter as tk

# random.seed(1)
# np.random.seed(1)

class sarsa(object):
    def __init__(self, target=None, gridSize=(20, 20)):
        if target == None:
            target = np.random.randint(2, size=(gridSize[0], gridSize[1]))
        self.gridWorld = np.zeros((gridSize[0]+1, gridSize[1]+1))
        self.gridWorld[20,:] = -1;
        self.gridWorld[:,20] = -1;
        self.qTable = np.random.rand(gridSize[0]+1, gridSize[1]+1, 4) * 0.01
        self.eTable = np.zeros((gridSize[0]+1, gridSize[1]+1, 4))
        self.target = target
        self.currentState = (random.randint(0, self.gridWorld.shape[0]-2), random.randint(0, self.gridWorld.shape[1]-2))
        self.gridWorld[target] = 1

        self.learningRate = 0.01
        self.decayRate = 0.999
        self.exploreRatio = 0.9
        self.edecay_rate = 0.5

        # Create a grid of None to store the references to the tiles
        self.tiles = [[None for i in range(gridSize[0])] for j in range(gridSize[1])]

        # Create the window, a canvas and the mouse click event binding
        root = tk.Tk()
        self.c = tk.Canvas(root, width=500, height=500, borderwidth=5, background='white')
        self.c.pack()
        self.c.bind("<Button-1>", self.callback_walls)

        self.col_width = 500/(self.gridWorld.shape[0]-1)
        self.row_height = 500/(self.gridWorld.shape[1]-1)

        self.c.create_rectangle(target[0]*self.col_width, target[1]*self.row_height, (target[0]+1)*self.col_width, (target[1]+1)*self.row_height, fill="gold")

        root.mainloop()
        
    def insert_walls(self, wall_locations=[]):
        for i in range(len(wall_locations)):
            if(wall_locations[i] != self.target):
                self.gridWorld[wall_locations[i][0], wall_locations[i][1]] = -1

    def remove_walls(self, wall_locations=[]):
        for i in range(len(wall_locations)):
            if(wall_locations[i] != self.target):
                self.gridWorld[wall_locations[i][0], wall_locations[i][1]] = 0

    def choose_action(self, state=None, custom_explore_ratio=None):
        # returns an action
        if state == None:
            state = self.currentState

        if custom_explore_ratio == None:
            exploreRatio = self.exploreRatio
        else:
            exploreRatio = custom_explore_ratio
        
        if random.uniform(0, 1) < exploreRatio:
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

    def print_grid(self):
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

    def plot_table(self, table):
        if(table.ndim == 3):
            x = np.arange(table.shape[0]-1)
            y = np.arange(table.shape[1]-1)
            max_indicies = np.argmax(table, axis=2)

            u = table[:-1,:-1,1] - table[:-1,:-1,3]
            v = table[:-1,:-1,0] - table[:-1,:-1,2]
        else:
            x = table[:,0]
            y = table[:,1]
            u = -table[:,2]
            v = table[:,3]
        
        plt.quiver(y, x, u, v, np.arctan2(v, u), angles='xy', scale_units='xy', scale=0.1, pivot='mid')
        # plt.barbs(x, y, u*1000, v*1000, np.arctan2(v, u), length=5, pivot='middle')

        plt.gca().invert_xaxis()
        plt.show()
    
    def forward(self, init_state=None):
        path = []
        if init_state == None:
            self.currentState = (random.randint(0, self.gridWorld.shape[0]-2), random.randint(0, self.gridWorld.shape[1]-2))
        else:
            self.currentState = init_state
        self.action = self.choose_action(custom_explore_ratio=0.2)
        self.alive = True
        reach_goal = False
        path.append(self.currentState)
        while(self.alive):
            state_prime = self.apply_action()
            path.append(state_prime)
            reward = self.gridWorld[state_prime]
            if(reward != 0):
                if(state_prime == self.target):
                    reach_goal = True
                self.alive = False;
            action_prime = self.choose_action(state=state_prime, custom_explore_ratio=0.2)
            self.currentState = state_prime
            self.action = action_prime
        return reach_goal, path

    def train(self, epoch):
        goal_count = 0
        pbar = ProgressBar(widgets=["epochs: ", Percentage(), " ",  Bar()], maxval=epoch).start()
        for ep in range(epoch):
            # initialize state and action
            self.currentState = (random.randint(0, self.gridWorld.shape[0]-2), random.randint(0, self.gridWorld.shape[1]-2))
            self.action = self.choose_action()
            # loop till find gold
            pbar.update(ep)
            self.alive = True
            while(self.alive):
                # self.draw_grid()
                state_prime = self.apply_action()
                reward = self.gridWorld[self.currentState]
                r_prime = self.gridWorld[state_prime]
                if(self.gridWorld[state_prime] != 0):
                    if(state_prime == self.target):
                        goal_count += 1
                    self.alive = False;
                action_prime = self.choose_action(state=state_prime)

                current_index = (self.currentState[0], self.currentState[1], self.action)
                prime_index = (state_prime[0], state_prime[1], action_prime)

                # delta = reward + self.exploreRatio*self.qTable[prime_index] - self.qTable[current_index]
                delta = r_prime + self.exploreRatio*self.qTable[prime_index] - self.qTable[current_index]
                # delta = reward + r_prime*self.exploreRatio + self.exploreRatio*self.qTable[prime_index] - self.qTable[current_index]
                self.eTable[current_index] = self.eTable[current_index] + 1
                
                # update q and e tables
                self.qTable = self.qTable + self.learningRate * delta * self.eTable
                self.eTable = self.eTable * self.exploreRatio * self.decayRate

                # update currentState and action
                self.currentState = state_prime
                self.action = action_prime
        pbar.finish()
        # print("goal: ", goal_count)
        print("accuracy: ", goal_count/epoch)

    def callback_walls(self, event):
        if(event.x == self.target[0] and event.y == self.target[1]):
            return
        # Calculate column and row number
        col = event.x/self.col_width
        row = event.y/self.row_height
        col = int(col)
        row = int(row)
        # If the tile is not filled, create a rectangle
        if not self.tiles[row][col]:
            self.tiles[row][col] = self.c.create_rectangle(col*self.col_width, row*self.row_height, (col+1)*self.col_width, (row+1)*self.row_height, fill="black")
            self.insert_walls([(row, col)])
        # If the tile is filled, delete the rectangle and clear the reference
        else:
            self.c.delete(self.tiles[row][col])
            self.tiles[row][col] = None
            self.remove_walls([(row, col)])



# Set number of rows and columns
ROWS = 20
COLS = 20

sarsa_test = sarsa(target=(16,3), gridSize=(ROWS, COLS))

sarsa_test.train(10000)
# sarsa_test.train(1000)
_is_done = False
# while not _is_done:
_is_done, path = sarsa_test.forward()
path = np.asarray(path)
direction = path[1:]-path[:-1]
direction = np.concatenate((direction, np.zeros((1, 2))), axis=0)
print(path.shape)
# print(direction.shape)
# print(path)
# print(direction)

direction[:,0], direction[:,1]  = direction[:,1], -direction[:,0]
direction = -direction*0.1

path = np.concatenate((path, direction), axis=1)
path = np.concatenate((path, np.array([[0, 0, 0, 0], [20, 0, 0, 0], [0, 20, 0, 0], [20, 20, 0, 0]])), axis=0)

# print(path.shape)

sarsa_test.plot_table(path)
# sarsa_test.plot_table(sarsa_test.eTable)
# sarsa_test.plot_table(np.clip(sarsa_test.eTable, 0, 0.1))
sarsa_test.plot_table(sarsa_test.qTable)
# sarsa_test.plot_table(np.clip(sarsa_test.qTable, -0.1, 0.1))
# print("accuracy", sarsa_test.accuracy)

