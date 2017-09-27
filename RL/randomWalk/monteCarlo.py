import warnings
import matplotlib.pyplot as plt
import numpy as np


class RandomWalk2D:
    def __init__(self, grid_size=3, end_states=[(0,0)], rewards=[1], exploration=.1, move_cost=0):
        self.n = grid_size
        self.end_states = end_states
        self.move_cost = move_cost
        self.rewards = rewards
        self.e = exploration
        self.n_actions = 4
        self.actions = [(-1,0), (1,0), (0,-1), (0,1)]
        # invalid move penality for first time
        self.q = np.ones((self.n, self.n, self.n_actions)) * -99999
        self.c = np.zeros((self.n, self.n, self.n_actions), dtype=int)
        self.policy = np.zeros((self.n,self.n), dtype=int)
        for state, value in np.ndenumerate(self.policy):
            self.policy[state] = np.random.choice(self.valid_moves(state))

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

    def move(self, state):
        action = self.policy[state]
        if np.random.uniform() < self.e:
            action = np.random.choice(self.valid_moves(state))
        return tuple([x+y for x,y in zip(state, self.actions[action])]), action

    def episode(self):
        state = tuple(np.random.random_integers(0, self.n - 1, size=2))
        states_actions = set()
        while state not in self.end_states:
            next_state, action = self.move(state)
            states_actions.add((state, action))
            state = next_state
        return states_actions, self.rewards[self.end_states.index(state)] - self.move_cost*len(states_actions)

    def update(self):
        states_actions, reward = self.episode()
        for s, a in states_actions:
            s_a = (s[0], s[1], a)
            self.c[s_a] += 1
            self.q[s_a] += (reward - self.q[s_a]) / self.c[s_a]
        for state,_ in states_actions:
            self.policy[state] = np.argmax(self.q[state[0], state[1],:])

    def plot(self, axis):
        d = 1/(2*self.n)
        for y in range(self.n):
            for x in range(self.n):
                cx, cy = x + .5, y + .5
                if (y,x) in self.end_states:
                    v = self.rewards[self.end_states.index((y,x))]
                    c = 'coral' if v < 0 else 'lime'
                    axis.add_artist(plt.Circle((cx, cy), .3, color=c))
                    axis.text(cx, cy, str(v), fontsize=15, horizontalalignment='center', verticalalignment='center')
                else:
                    dx, dy = 0, 0
                    m = self.policy[(y,x)]
                    if m == 0:
                        dx, dy = 0, -.3
                    elif m == 1:
                        dx, dy = 0, .3
                    elif m == 2:
                        dx, dy = -.3, 0
                    elif m == 3:
                        dx, dy = .3, 0
                    plt.arrow(cx, cy, dx, dy, head_width=.2, head_length=.2, fc="k", ec="k")


def main():
    exp = RandomWalk2D(grid_size=5, exploration=.15, move_cost=.05, \
       end_states=[(0,0), (4,4), (1,2), (2,1)], rewards=[2,1,-1,-1])

    display_interval = 1000
    figure, axis = plt.subplots()
    figure.canvas.set_window_title('Monte-Carlo')
    axis.set_xlim([0,exp.n])
    axis.xaxis.tick_top()
    axis.set_ylim([exp.n, 0])
    
    for iter in range(1000):
        for sub_iter in range(display_interval):
            exp.update()
        
        axis.cla()
        exp.plot(axis)
        plt.title('Policy Iteration: {0}'.format((iter+1)*display_interval), y=1.08)
        axis.set_aspect('equal')
        plt.draw()
        with warnings.catch_warnings():
            warnings.simplefilter("ignore")
            plt.pause(.001)
    plt.show()


if __name__ == '__main__':
    main()
