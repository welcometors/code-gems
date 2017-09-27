import time
from matplotlib import pyplot as plt
from cppLib.nArmedBandit import e_greedy, softmax


def experiment(n_runs, n_machines, n_steps, method='softmax', param=.1):
    if method == 'softmax':
        return softmax.run(n_runs, n_machines, n_steps, param)
    return e_greedy.run(n_runs, n_machines, n_steps, param)


def main():
    n_runs = 10000
    n_machines = 10
    n_steps = 1000
    parameters = [('e_greedy', 0), ('e_greedy', .01), ('softmax', .2), ('e_greedy', .1)]
    avg_rewards, optimal_actions = [], []
    legends = []

    start_time = time.time()
    for i, (m, p) in enumerate(parameters):
        print('\r {0:03.2f}%'.format(100*i/len(parameters)), end='')
        r, a = experiment(n_runs, n_machines, n_steps, m, p)
        avg_rewards.append(r)
        optimal_actions.append(a)
        legends.append('{0} param= {1}'.format(m, p))
    print("\r{0} seconds.".format(time.time() - start_time))

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