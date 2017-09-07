import sys
sys.path.append('../x64/Release')

# using cpp implementation for around 15x speed-up
import RLcpp as rl
from matplotlib import pyplot as plt

def experiment(n_runs, n_machines, n_steps, method='softmax', param=.1):
    if method == 'softmax':
        return rl.softmax(n_runs, n_machines, n_steps, param)
    return rl.e_greedy(n_runs, n_machines, n_steps, param)


def main():
    n_runs = 10000
    n_machines = 10
    n_steps = 1000
    parameters = [('softmax', .2), ('softmax', .15), ('softmax', .3), ('e_greedy', 0), ('e_greedy', .1)]
    avg_rewards, optimal_actions = [], []
    legends = []

    for m, p in parameters:
        r, a = experiment(n_runs, n_machines, n_steps, m, p)
        avg_rewards.append(r)
        optimal_actions.append(a)
        legends.append('{0} param= {1}'.format(m, p))

    for avg_reward in avg_rewards:
        plt.plot(avg_reward)
    plt.legend(legends, loc='upper left')
    plt.ylabel('Average Reward')
    plt.xlabel('Steps')
    plt.title('Machines = {0}, Runs = {1}'.format(n_machines, n_runs))
    plt.show()

    for optimal_action in optimal_actions:
        plt.plot(optimal_action)
    plt.gca().set_ylim([0,100])
    plt.legend(legends, loc='upper left')
    plt.ylabel('% Optimal Action')
    plt.xlabel('Steps')
    plt.title('Machines = {0}, Runs = {1}'.format(n_machines, n_runs))
    plt.show()


if __name__ == '__main__':
    main()