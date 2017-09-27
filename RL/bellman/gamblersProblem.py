import random
import warnings
from matplotlib import pyplot as plt
import numpy as np


class Gambler:
    def __init__(self, prob_win=.4, gamma=1, max_state=100):
        self.v = np.zeros(max_state + 1)
        self.v[max_state] = 1
        self.bias = prob_win
        self.gamma = gamma
        self.max_state = max_state

    def bellman(self, state, action):
        value = (1 - self.bias) * self.gamma * self.v[state - action]
        win_state = min(state + action, self.max_state)
        if win_state == self.max_state:
            value += self.bias # prob*(1) i.e. no next_state
        else:
            value += self.bias * self.gamma * self.v[win_state] # zero reward
        return value

    def Q(self, state):
        q = np.zeros(state + 1)
        for action in range(1, state + 1):
            q[action] = self.bellman(state, action)
        return q

    def sweep(self):
        old_v = np.copy(self.v)
        for state in range(1, self.max_state):
            self.v[state] = np.amax(self.Q(state))
        return np.amax(np.abs(self.v - old_v))

    def policy(self, epsilon=1e-8):
        p = np.zeros(self.max_state + 1)
        for state in range(1, self.max_state):
            best_so_far, best_so_far_value = 0, -1
            for action, value in np.ndenumerate(self.Q(state)):
                if best_so_far_value < value - epsilon: #player is encouraged for smaller bets
                    best_so_far, best_so_far_value = action[0], value
            p[state] = best_so_far
        return p

    def policy_family(self, epsilon=1e-8):
        x, y = [], []
        for state in range(1, self.max_state):
            q = self.Q(state)
            best = np.amax(q)
            for action, value in np.ndenumerate(q):
                if value >= best - epsilon:
                    x.append(state)
                    y.append(action[0])
        return x, y


def main():
    aj = Gambler(prob_win=.4)
    plot_sweeps = {1,2,3,4,8,16,32}
    #aj = Gambler(prob_win=.55)
    #plot_sweeps = {1,2,4,16,64,256,1024,2048}
    legends = []

    for i in range(1, max(plot_sweeps) + 1):
        delta = aj.sweep()
        if i in plot_sweeps:
            plt.plot(aj.v)
            legends.append('Sweep = {0}, delta = {1}'.format(i, delta))

    plt.legend(legends, loc='lower right' if aj.bias > .5 else 'upper left')
    plt.ylabel('Value Estimates')
    plt.xlabel('Capital')
    plt.title("Gambler's Problem for Prob(Head)={0}".format(aj.bias))
    plt.show()

    figure, axis = plt.subplots()
    policy = aj.policy()
    axis.bar(list(range(len(policy))), policy, width=1)
    plt.ylabel('Final Policy')
    plt.xlabel('Capital')
    plt.title("Gambler's Problem for Prob(Head)={0}".format(aj.bias))
    plt.show()

    x, y = aj.policy_family()
    plt.scatter(x, y)
    plt.ylabel('Optimal Policies')
    plt.xlabel('Capital')
    plt.title("Gambler's Problem for Prob(Head)={0}".format(aj.bias))
    plt.show()


if __name__ == '__main__':
    main()
