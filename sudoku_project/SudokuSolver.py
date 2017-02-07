from tkinter import *
import math
import numpy as np

puzzles = ["easy1_p.txt", "easy2_p.txt", "easy3_p.txt", "medium1_p.txt", "medium2_p.txt", "medium3_p.txt", "hard1_p.txt", "hard2_p.txt", "hard3_p.txt"]
solutions = ["easy1_s.txt", "easy2_s.txt", "easy3_s.txt", "medium1_s.txt", "medium2_s.txt", "medium3_s.txt", "hard1_s.txt", "hard2_s.txt", "hard3_s.txt"]
currentPuzzle = 0
iterCount = 0
running = True

class globalCanvas(Frame):
    def get_canvas(self):
        return self.canvas

    def __init__(self, parent):
        Frame.__init__(self, parent)
        self.canvas = Canvas(self)
        self.parent = parent
        self.parent.title("Sudoku")
        self.pack(fill=BOTH, expand=1)


class SudokuUI():
    ''' getters and setters '''

    def get_gridCoord(self):
        return self.__gridCoord.copy()

    def get_initValue(self):
        return self.__initValue.copy()

    def get_aisolutionValue(self):
        return self.__solutionValue

    def get_x0(self):
        return self.__x0

    def set_x0(self, x0):
        if (x0 >= 0):
            self.__x0 = x0

    def get_y0(self):
        return self.__y0

    def set_y0(self, y0):
        if (y0 >= 0):
            self.__y0 = y0

    def get_length(self):
        return self.__length

    def set_length(self, value):
        self.__length = value

    ''' constructor '''

    def __init__(self, canvas):
        self.canvas = canvas
        self.__gridCoord = np.zeros((9, 9), [('x', 'i4'), ('y', 'i4')])
        self.__initValue = np.zeros((9, 9), int)
        self.__solutionValue = np.zeros((9, 9), int)
        self.__x0 = 0
        self.__y0 = 0
        self.__length = 900

    ''' methods '''

    def print_puzzleName(self, problemFile):
        self.canvas.create_text(500,
                                7,
                                anchor=CENTER, font="Helvetica",
                                text=problemFile,
                                fill="black")

    def import_problem(self, problemFile):
        importFile = open(problemFile, 'r')
        for j in range(9):
            line = importFile.readline()
            split = line.split()
            for i in range(9):
                self.__initValue[i, j] = int(split[i])
                # print self.__initValue

    def drawGrid(self):
        stride = self.__length / 9

        for i in range(9):
            for j in range(9):
                self.__gridCoord[i, j]['x'] = self.__x0 + (stride * i)
                self.__gridCoord[i, j]['y'] = self.__y0 + (stride * j)
        # print self.__gridCoord

        for i in range(10):
            color = "blue" if i % 3 == 0 else "gray"
            self.canvas.create_line(self.__x0 + (i * stride),
                                    self.__y0,
                                    self.__x0 + (i * stride),
                                    self.__y0 + self.__length,
                                    fill=color)
            self.canvas.create_line(self.__x0,
                                    self.__y0 + i * stride,
                                    self.__x0 + self.__length,
                                    self.__y0 + i * stride,
                                    fill=color)

    def drawInitValues(self):
        stride = self.__length / 9
        for i in range(9):
            for j in range(9):
                if self.__initValue[i, j] != 0:
                    self.canvas.create_text(self.__gridCoord[i, j]['x'] + stride / 2,
                                            self.__gridCoord[i, j]['y'] + stride / 2,
                                            anchor=CENTER, font="Helvetica",
                                            text=self.__initValue[i, j],
                                            fill="black")

    def drawSolvedValues(self):
        stride = self.__length / 9
        for i in range(9):
            for j in range(9):
                if self.__solutionValue[i, j] != 0:
                    self.canvas.create_text(self.__gridCoord[i, j]['x'] + stride / 2,
                                            self.__gridCoord[i, j]['y'] + stride / 2,
                                            anchor=CENTER, font="Helvetica",
                                            text=self.__solutionValue[i, j],
                                            fill="green")

    def drawPossible(self, possibles):
        stride = self.__length / 9
        for i in range(9):
            for j in range(9):
                possible_str = ""
                possible_num = 0
                for p in range(9):
                    if possibles[i][j][p] == True:
                        possible_num += 1
                        possible_str += str(p + 1)
                        possible_str += ","
                if possible_num > 1:
                    self.canvas.create_text(self.__gridCoord[i, j]['x'] + stride / 2,
                                            self.__gridCoord[i, j]['y'] + stride / 2,
                                            anchor=CENTER, font="Helvetica",
                                            text=possible_str,
                                            fill="blue")

    # draws everything on the canvas all at once
    def draw(self):
        self.canvas.pack(side=BOTTOM, fill=BOTH, expand=1)

    def clear(self):
        self.canvas.delete(ALL)
        self.draw()

    def checkSolution(self, solution):
        validSolution = True
        for i in range(9):
            for j in range(9):
                if self.__initValue[i, j] != solution[i, j]:
                    validSolution = False
        if validSolution:
            self.canvas.create_text(self.__x0,
                                    self.__y0 + self.__length + 10,
                                    anchor=W, font="Helvetica",
                                    text="The Solution is Correct!",
                                    fill="green")
            print("Solution is Correct")
        else:
            self.canvas.create_text(self.__x0,
                                    self.__y0 + self.__length + 10,
                                    anchor=W, font="Helvetica",
                                    text="The Solution is Incorrect!",
                                    fill="red")
            print("Solution is Incorrect")

class SudokuAI():
    ''' global vars '''
    __workingValue = np.zeros((9, 9), int)
    __possibleSpace = np.ones((9, 9, 9), bool)

    def get_workingValue(self):
        return self.__workingValue

    def get_possibleSpace(self):
        return self.__possibleSpace

    ''' constructor '''

    def __init__(self, problem_set):
        self.__workingValue = problem_set

    ''' methods '''

    def import_problem(self, problem_set):
        self.__workingValue = problem_set

    def keep_numPossibility(self, x, y, num):
        for p in range(9):
            if p != num:
                self.__possibleSpace[x, y, p] = False

    def get_nonet(self, posx, posy):
        # nonet contains coordinates x,y for all the locations of a nonet of coordinate posx, posy
        # (0,0) (1,0) (2,0)
        # (0,1) (1,1) (2,1)
        # (0,2) (1,2) (2,2)
        nonet = np.zeros(9, [('x', 'i4'), ('y', 'i4')])
        for i in range(3):
            for j in range(3):
                nonet[i+3*j]['x'] = i+3*posx
                nonet[i+3*j]['y'] = j+3*posy
        return nonet

    def checkValidity(self, value, posx, posy):
        for i in range(9):
            if self.__workingValue[i, posy] == value:
                return False
        for j in range(9):
            if self.__workingValue[posx, j] == value:
                return False
        for k in range(9):
            if self.__workingValue[self.get_nonet(posx, posy)[k]['x'], self.get_nonet(posx, posy)[k]['y']] == value:
                return False
        return True

    # removes the possibilities from an already occupied space
    def rule0(self):
        for i in range(9):
            for j in range(9):
                if self.__workingValue[i, j] != 0:
                    for k in range(9):
                        self.__possibleSpace[i, j, k] = False

    # removes the possibility of a number if it already exists in a row
    def rule1(self):
        # print type(self.__possibleSpace[:,0][0])
        # iterating through the rows
        for j in range(9):
            # iterating through the elements to find an existing value
            for i in range(9):
                if self.__workingValue[i, j] != 0:
                    # iterates through the elements again to find empty values
                    for ii in range(9):
                        if self.__workingValue[ii, j] == 0:
                            # remove the possibility of the existing element in the empty space
                            self.__possibleSpace[ii, j, self.__workingValue[i, j] - 1] = False
                            # print self.__possibleSpace

    # removes the possibility of a number if it already exists in a column
    def rule2(self):
        # print type(self.__possibleSpace[:,0][0])
        # iterating through the cols
        for i in range(9):
            # iterating through the elements to find an existing value
            for j in range(9):
                if self.__workingValue[i, j] != 0:
                    # iterates through the elements again to find empty values
                    for jj in range(9):
                        if self.__workingValue[i, jj] == 0:
                            # remove the possibility of the existing element in the empty space
                            self.__possibleSpace[i, jj, self.__workingValue[i, j] - 1] = False
                            # print self.__possibleSpace

    # removes the possibility of a number if it already exists in a nonet
    def rule3(self):
        # iterating through the nonets
        for n in range(9):
            nonet = self.get_nonet(int(n/3), (n-3*int(n/3)))
            #print(nonet)
            for elem in range(9):
                if self.__workingValue[nonet[elem]['x'], nonet[elem]['y']] != 0:
                    for elem2 in range(9):
                        if self.__workingValue[nonet[elem2]['x'], nonet[elem2]['y']] == 0:
                            self.__possibleSpace[nonet[elem2]['x'], nonet[elem2]['y'],
                                                 self.__workingValue[nonet[elem]['x'], nonet[elem]['y']] - 1] = False

    # selects a number if the possible number occurs only once in a row
    def rule4(self):
        for i in range(9):
            for num in range(9):
                numCount = 0
                numJ = 0
                for j in range(9):
                    if self.__possibleSpace[i, j, num] == True:
                        numCount += 1
                        numJ = j
                if numCount == 1:
                    self.keep_numPossibility(i, numJ, num)

    # selects a number if the possible number occurs only once in a column
    def rule5(self):
        for j in range(9):
            for num in range(9):
                numCount = 0
                numI = 0
                for i in range(9):
                    if self.__possibleSpace[i, j, num] == True:
                        numCount += 1
                        numI = i
                if numCount == 1:
                    self.keep_numPossibility(numI, j, num)

    # selects a number if the possible number occurs only once in a nonet
    def rule6(self):
        for n in range(9):
            nonet = self.get_nonet(int(n/3), (n-3*int(n/3)))
            for num in range(9):
                numCount = 0
                numN = 0
                for elem in range(9):
                    if self.__possibleSpace[nonet[elem]['x'], nonet[elem]['y'], num] == True:
                        numCount += 1
                        numN = elem
                if numCount == 1:
                    self.keep_numPossibility(nonet[numN]['x'], nonet[numN]['y'], num)

    def updateValues(self, displayValues):
        countUpdates = 0
        newValues = np.zeros((9, 9), int)
        for i in range(9):
            for j in range(9):
                countPossible = 0
                ans = 0
                for k in range(9):
                    if self.__possibleSpace[i, j, k] == True:
                        countPossible += 1
                        ans = k
                if countPossible == 1:
                    countUpdates += 1
                    newValues[i, j] = ans + 1
        self.__workingValue += newValues
        displayValues += newValues
        if countUpdates == 0:
            return True
        else:
            return False

    def reset(self, displayValues):
        self.__workingValue = np.zeros((9, 9), int)
        self.__possibleSpace = np.ones((9, 9, 9), bool)
        for i in range(9):
            for j in range(9):
                displayValues[i, j] = 0

def iterateCallBack(ai, env, check, countUpdates):
    # run ai rules
    global currentPuzzle
    global iterCount
    global running
    env.clear()
    check.clear()

    ai.rule0()
    ai.rule1()
    ai.rule2()
    ai.rule3()
    ai.rule4()
    ai.rule5()
    ai.rule6()
    if ai.updateValues(env.get_aisolutionValue()) == False:
        env.drawGrid()
        env.drawInitValues()
        env.drawSolvedValues()
        env.drawPossible(ai.get_possibleSpace())
        env.print_puzzleName(puzzles[currentPuzzle])
        env.draw()
        iterCount += 1
        print ("iteration "+str(iterCount))
    else:
        env.drawGrid()
        env.drawInitValues()
        env.drawSolvedValues()
        env.drawPossible(ai.get_possibleSpace())

        check.import_problem(solutions[currentPuzzle])
        check.drawGrid()
        check.drawInitValues()
        check.checkSolution(ai.get_workingValue())
        check.draw()
        print("done puzzle " + str(puzzles[currentPuzzle]))
        # iterate through next puzzle
        currentPuzzle += 1
        iterCount = 0

        try:
            ai.reset(env.get_aisolutionValue())
            env.import_problem(puzzles[currentPuzzle])
            ai.import_problem(env.get_initValue())
        except:
            running = False


def main():
    countUpdates = 0
    root = Tk()
    canvas = globalCanvas(root)

    check = SudokuUI(canvas.get_canvas())
    check.set_x0(1000)
    check.set_y0(20)
    check.set_length(450)
    environment = SudokuUI(canvas.get_canvas())
    environment.set_x0(10)
    environment.set_y0(20)

    environment.import_problem(puzzles[currentPuzzle])
    environment.print_puzzleName(puzzles[currentPuzzle])

    ai = SudokuAI(environment.get_initValue())
    b = Button(text="iterate", command=lambda: iterateCallBack(ai, environment, check, countUpdates))
    b.pack()

    environment.drawGrid()
    environment.drawInitValues()
    # environment.drawPossible(ai.get_possibleSpace())
    environment.draw()
    w, h = root.winfo_screenwidth(), root.winfo_screenheight()
    root.geometry("%dx%d+0+0" % (w, h))

    while running:
        root.update_idletasks()
        root.update()

if __name__ == '__main__':
    main()
