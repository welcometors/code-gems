import sys
sys.path.append('../x64/Release')

# using cpp implementation for around 15x speed-up
import RLcpp as rl
from matplotlib import pyplot as plt


def main():
    n_runs = 10000
    n_machines = 10
    n_steps = 1000
    parameters = [.0, .01, .10]
    avg_rewards, optimal_actions = [], []
    legends = []

    for e in parameters:
        r, a = rl.e_greedy(n_runs, n_machines, n_steps, e)
        avg_rewards.append(r)
        optimal_actions.append(a)
        legends.append('explore {0}%'.format(e*100))

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