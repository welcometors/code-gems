from cppLib.libImporter import RLcpp as rl


class e_greedy():
    @staticmethod
    def run(n_runs=1, n_machines=10, n_steps=100, exploration=.1):
        return rl.nArmedBandit_eGreedy(n_runs, n_machines, n_steps, exploration)


class softmax():
    @staticmethod
    def run(n_runs=1, n_machines=10, n_steps=100, temperature=.1):
        return rl.nArmedBandit_softmax(n_runs, n_machines, n_steps, temperature)

