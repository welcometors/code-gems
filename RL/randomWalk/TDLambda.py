import warnings
import matplotlib.pyplot as plt
import numpy as np
from sklearn.preprocessing import normalize


class RandomWalk2D:
    ''' Class for TD-lambda random walk '''

    def __init__(self, grid_size=3, end_states=[(0,0)], rewards=[1], \
        exploration=.1, move_cost=0, alpha=.3, gamma=.9, lmbda=.7):

        self.n = grid_size
        self.alpha = alpha
        self.gamma = gamma
        self.lmbda = lmbda
        self.end_states = end_states
        self.move_cost = move_cost
        self.rewards = rewards
        self.e = exploration
        self.actions = [(-1,0), (1,0), (0,-1), (0,1)]
        # history for plot
        self.h = np.zeros((self.n, self.n, len(self.actions)), dtype=int)
        # invalid move penality for first time
        self.v = np.zeros((self.n, self.n))
        for idx, state in enumerate(end_states):
            self.v[state] = rewards[idx] / (1-lmbda)

    def valid_moves(self, state):
        moves = []
        if state[0] != 0:
            moves.append(0)
        if state[0] != self.n - 1:
            moves.append(1)
        if state[1] != 0:
            moves.append(2)
        if state[1] != self.n - 1:
            moves.append(3)
        return moves

    def choose_action(self, state):
        moves = self.valid_moves(state)
        if np.random.uniform() < self.e:
            return np.random.choice(moves)
        values = np.zeros(len(moves))
        for idx, move in enumerate(moves):
            values[idx] = self.v[self.move(state, move)]
        return moves[np.argmax(values)]

    def move(self, state, action):
        return tuple([x+y for x,y in zip(state, self.actions[action])])

    def episode(self):
        old_v = np.copy(self.v)
        z = np.zeros((self.n, self.n))
        state = self.end_states[0]
        while state in self.end_states:
            state = tuple(np.random.random_integers(0, self.n - 1, size=2))
        visited = set()
        reward, end = 0, False
        while not end:
            action = self.choose_action(state)
            reward -= self.move_cost
            state1 = self.move(state, action)
            if state1 in self.end_states:
                reward += self.rewards[self.end_states.index(state1)]
                end = True
            delta = reward + self.gamma*self.v[state1] - self.v[state]
            z[state] = 1 #replacing trace
            #z[state] += 1 #accumulative trace
            visited.add(state)
            for s in visited:
                self.v[s] += self.alpha*delta*z[s]
                z[s] *= self.gamma*self.lmbda
            visited = set([x for x in visited if z[x] < 1e-5])
            self.h[(state[0], state[1], action)] += 1
            state = state1
        return np.amax(np.abs(self.v - old_v))

    def policy(self):
        e, self.e = self.e, 0
        policy = np.zeros((self.n,self.n), dtype=int)
        for y in range(self.n):
            for x in range(self.n):
                policy[y,x] = self.choose_action((y,x))
        self.e = e
        return policy

    def arrow(self, m):
        dx, dy = 0, 0
        if m == 0:
            dx, dy = 0, -.3
        elif m == 1:
            dx, dy = 0, .3
        elif m == 2:
            dx, dy = -.3, 0
        elif m == 3:
            dx, dy = .3, 0
        return dx, dy

    def plot(self, axis):
        policy = self.policy()
        maxh = np.amax(self.h)*.2
        for y in range(self.n):
            for x in range(self.n):
                cx, cy = x + .5, y + .5
                if (y,x) in self.end_states:
                    v = self.rewards[self.end_states.index((y,x))]
                    c = 'coral' if v < 0 else 'lime'
                    axis.add_artist(plt.Circle((cx, cy), .3, color=c))
                    axis.text(cx, cy, str(v), fontsize=15, horizontalalignment='center', verticalalignment='center')
                else:
                    #v = float(self.v[y,x])
                    #c = 'coral' if v < 0 else 'lime'
                    #axis.add_artist(plt.Circle((cx, cy), .3, color=c))
                    moves = np.copy(self.h[y, x, :])
                    for m, v in np.ndenumerate(moves):
                        v = 1 - min(1, v / maxh)
                        dx, dy = self.arrow(m[0])
                        c = '#{0:02x}{0:02x}ff'.format(int(v*255))
                        plt.arrow(cx, cy, dx, dy, head_width=.2, head_length=.2, fc=c, ec=c)
                    dx, dy = self.arrow(policy[(y,x)])
                    plt.arrow(cx, cy, dx, dy, head_width=.2, head_length=.2, fc='k', ec='k')
        self.h //= 10


def main():
    exp = RandomWalk2D(grid_size=5, exploration=0.01, move_cost=0.01, \
        end_states=[(0,0), (4,4), (1,2), (2,1)], \
        rewards=[2,1,-2,-1], \
        alpha=.3, gamma=.9, lmbda=.8)

    display_interval = 100
    figure, axis = plt.subplots()
    figure.canvas.set_window_title('SARSA')
    axis.set_xlim([0,exp.n])
    axis.xaxis.tick_top()
    axis.set_ylim([exp.n, 0])
    
    for iter in range(1000):
        delta = 0
        for sub_iter in range(display_interval):
            delta = exp.episode()
        print(exp.v)
        axis.cla()
        exp.plot(axis)
        plt.title('Policy Iteration: {0}, delta = {1:.7f}' \
            .format((iter+1)*display_interval, delta), y=1.08)
        plt.xlabel('Blue: visit-frequency, Black: optimal-policy')
        axis.set_aspect('equal')
        plt.draw()
        with warnings.catch_warnings():
            warnings.simplefilter("ignore")
            plt.pause(.001)
    plt.show()


if __name__ == '__main__':
    main()
