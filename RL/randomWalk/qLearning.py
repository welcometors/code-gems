import warnings
import matplotlib.pyplot as plt
import numpy as np
from sklearn.preprocessing import normalize


class RandomWalk2D:
    ''' Class for q-learning random walk '''

    def __init__(self, grid_size=3, end_states=[(0,0)], rewards=[1], \
        exploration=.1, move_cost=0, alpha=.3, gamma=.9):

        self.n = grid_size
        self.alpha = alpha
        self.gamma = gamma
        self.end_states = end_states
        self.move_cost = move_cost
        self.rewards = rewards
        self.e = exploration
        self.actions = [(-1,0), (1,0), (0,-1), (0,1)]
        # history for plot
        self.h = np.zeros((self.n, self.n, len(self.actions)), dtype=int)
        # invalid move penality for first time
        self.q = np.ones((self.n, self.n, len(self.actions))) * -99999
        for y in range(self.n):
            for x in range(self.n):
                for move in self.valid_moves((y,x)):
                    self.q[y, x, move] = 0

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
        if np.random.uniform() < self.e:
            return np.random.choice(self.valid_moves(state))
        return np.argmax(self.q[state[0], state[1], :])

    def move(self, state, action):
        return tuple([x+y for x,y in zip(state, self.actions[action])])

    def episode(self):
        old_q = np.copy(self.q)
        state = self.end_states[0]
        while state in self.end_states:
            state = tuple(np.random.random_integers(0, self.n - 1, size=2))
        reward, end = 0, False
        while not end:
            reward -= self.move_cost
            action = self.choose_action(state)
            state1 = self.move(state, action)
            if state1 in self.end_states:
                reward += self.rewards[self.end_states.index(state1)]
                end = True
            Maction = np.argmax(self.q[state1[0], state1[1], :])
            s_a = (state[0], state[1], action)
            s1_a1 = (state1[0], state1[1], Maction)
            self.q[s_a] += self.alpha*(reward + self.gamma*self.q[s1_a1] - self.q[s_a])
            self.h[s_a] += 1
            state = state1
        return np.amax(np.abs(self.q - old_q))

    def policy(self):
        policy = np.zeros((self.n,self.n), dtype=int)
        for y in range(self.n):
            for x in range(self.n):
                policy[y,x] = np.argmax(self.q[y, x, :])
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
    exp = RandomWalk2D(grid_size=5, exploration=.05, move_cost=0.01, \
        end_states=[(0,0), (4,4), (1,2), (2,1)], \
        alpha=.1, gamma=1, rewards=[2,1,-2,-1])

    display_interval = 1000
    figure, axis = plt.subplots()
    figure.canvas.set_window_title('Q-Learning')
    axis.set_xlim([0,exp.n])
    axis.xaxis.tick_top()
    axis.set_ylim([exp.n, 0])
    
    for iter in range(1000):
        delta = 0
        for sub_iter in range(display_interval):
            delta = exp.episode()
        # print(exp.q)
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
