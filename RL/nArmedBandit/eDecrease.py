import random
from matplotlib import pyplot as plt
import numpy as np


class agent():
    def __init__(self, machines, exploration, decay_rate):
        self.exploration = exploration
        # creats an estimate of n machines
        self.machines = np.zeros(machines)
        self.counts = [0] * machines
        self.steps = 0
        self.decay = decay_rate

    def action(self):
        best = self.machines.argmax()
        # exponen decay
        e = self.exploration * np.exp(self.steps*self.decay)
        if np.random.random_sample() < e:
            chosen = np.random.random_integers(0, len(self.machines) - 2)
            return chosen if chosen < best else chosen + 1
        return best

    def update(self, machine, reward):
        self.steps += 1
        self.counts[machine] += 1
        self.machines[machine] += (reward - self.machines[machine]) / self.counts[machine]


class environment():
    def __init__(self, machines):
        self.machines = np.random.normal(size=machines)

    def optimal_action(self):
        return np.argmax(self.machines)

    def reward(self, action):
        return self.machines[action] + np.random.normal()


def experiment(n_runs, n_machines, n_steps, exploration, decay_rate):
    rewards = np.zeros((n_runs, n_steps))
    optimals = np.zeros((n_runs, n_steps))

    for run in range(n_runs):
        env = environment(n_machines)
        aj = agent(n_machines, exploration, decay_rate)
        for step in range(n_steps):
            action = aj.action()
            if action == env.optimal_action():
                optimals[run,step] = 100
            rewards[run,step] = env.reward(action)
            aj.update(action, rewards[run,step])
    
    return np.mean(rewards, 0), np.mean(optimals, 0)


def main():
    parameters = [(.1,0), (.25,.00001)]
    avg_rewards, optimal_actions = [], []
    legends = []

    for e,r in parameters:
        ar, oa = experiment(n_runs=1000, n_machines=10, n_steps=1000, exploration=e, decay_rate=r)
        avg_rewards.append(ar)
        optimal_actions.append(oa)
        legends.append('explore {0}%, rate = {1}'.format(e*100, r))

    for avg_reward in avg_rewards:
        plt.plot(avg_reward)
    plt.legend(legends, loc='upper left')
    plt.ylabel('Average Reward')
    plt.xlabel('Steps')
    plt.show()

    for optimal_action in optimal_actions:
        plt.plot(optimal_action)
    plt.gca().set_ylim([0,100])
    plt.legend(legends, loc='upper left')
    plt.ylabel('% Optimal Action')
    plt.xlabel('Steps')
    plt.show()


if __name__ == '__main__':
    main()