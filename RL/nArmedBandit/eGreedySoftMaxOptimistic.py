import random
from matplotlib import pyplot as plt
import numpy as np


class environment():
    def __init__(self, machines):
        self.machines = np.random.normal(size=machines)
        self.optimal_action = np.argmax(self.machines)

    def reward(self, action):
        return self.machines[action] + np.random.normal()


class policies():
    
    @staticmethod
    def e_greedy(values, e):
        best = values.argmax()
        if np.random.random_sample() < e:
            choices = list(range(len(values)))
            choices.remove(best)
            return np.random.choice(choices)
        return best

    @staticmethod
    def softmax(values, temp):
        probabilities = np.exp(values / temp)
        probabilities /= sum(probabilities)
        return np.random.choice(list(range(len(values))), p=probabilities)


class agent():
    def __init__(self, n_machines, initial_values=0.0, policy='softmax', policy_param=.1):
        self.policy_param = policy_param
        self.policy = policies.softmax if policy == 'softmax' else policies.e_greedy
        # creats an estimate of n machines
        self.machines = np.ones(n_machines) * initial_values
        self.counts = [1] * n_machines

    def action(self):
        return self.policy(self.machines, self.policy_param)
    
    def update(self, machine, reward):
        self.machines[machine] += (reward - self.machines[machine]) / self.counts[machine]
        self.counts[machine] += 1


def experiment(n_runs, n_machines, n_steps, method, param, optimism):
    rewards = np.zeros((n_runs, n_steps))
    optimals = np.zeros((n_runs, n_steps))

    for run in range(n_runs):
        env = environment(n_machines)
        aj = agent(n_machines, initial_values=optimism, policy=method, policy_param=param)
        for step in range(n_steps):
            action = aj.action()
            if action == env.optimal_action:
                optimals[run,step] = 100
            rewards[run,step] = env.reward(action)
            aj.update(action, rewards[run,step])
    
    return np.mean(rewards, 0), np.mean(optimals, 0)


def main():
    n_runs = 10000
    n_machines = 10
    n_steps = 1000
    parameters = [('uniform', .1, 0), ('uniform', .1, 5), ('softmax', .2, 0), ('softmax', .2, 5)]
    avg_rewards, optimal_actions = [], []
    legends = []

    for m, p, o in parameters:
        r, a = experiment(n_runs, n_machines, n_steps, method=m, param=p, optimism=o)
        avg_rewards.append(r)
        optimal_actions.append(a)
        legends.append('{0} param= {1} {2}'.format(m, p, '(optimistic)' if o > 0 else ''))

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