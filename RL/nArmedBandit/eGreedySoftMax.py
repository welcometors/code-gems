import random
from matplotlib import pyplot as plt
import numpy as np


class agent():
    def __init__(self, machines, method='uniform', exploration=0, temperature=.1):
        self.method = method
        self.exploration = exploration
        self.temperature = temperature
        # creats an estimate of n machines
        self.machines = np.zeros(machines)
        self.counts = [0] * machines

    def action(self):
        best = self.machines.argmax()
        if np.random.random_sample() < self.exploration:
            choices = list(range(len(self.machines)))
            choices.remove(best)
            if self.method == 'uniform':
                return np.random.choice(choices)
            else:
                probabilities = np.exp(self.machines / self.temperature)
                probabilities = np.delete(probabilities, best)
                probabilities = probabilities / sum(probabilities)
                return np.random.choice(choices, p=probabilities)
        return best
    
    def update(self, machine, reward):
        self.counts[machine] += 1
        self.machines[machine] += (reward - self.machines[machine]) / self.counts[machine]


class environment():
    def __init__(self, machines):
        self.machines = np.random.normal(size=machines)
        self.optimal_action = np.argmax(self.machines)

    def reward(self, action):
        return self.machines[action] + np.random.normal()


def experiment(n_runs, n_machines, n_steps, method, exploration, temperature):
    rewards = np.zeros((n_runs, n_steps))
    optimals = np.zeros((n_runs, n_steps))

    for run in range(n_runs):
        env = environment(n_machines)
        aj = agent(n_machines, method, exploration, temperature)
        for step in range(n_steps):
            action = aj.action()
            if action == env.optimal_action:
                optimals[run,step] = 100
            rewards[run,step] = env.reward(action)
            aj.update(action, rewards[run,step])
    
    return np.mean(rewards, 0), np.mean(optimals, 0)


def main():
    n_runs = 2000
    n_machines = 10
    n_steps = 1000
    parameters = [('softmax', .1, 1), ('uniform', 0, None), ('uniform', .1, None)]
    avg_rewards, optimal_actions = [], []
    legends = []

    for m, e, t in parameters:
        r, a = experiment(n_runs, n_machines, n_steps, method=m, exploration=e, temperature=t)
        avg_rewards.append(r)
        optimal_actions.append(a)
        legends.append('{0} exploration= {1}% {2}'.format(m, 100*e, '' if t is None else 'temperature= {0}'.format(t)))

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