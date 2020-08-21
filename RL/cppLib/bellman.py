from cppLib.libImporter import RLcpp as rl

    
class JacksCarRental:
    def __init__(self, max_cars=20, max_transfer=5, \
        avg_request_return_A=(3,3), avg_request_return_B=(3,3), \
        rent=10, transfer_cost=2, gamma=.9):
        self.id = rl.bellman_jcr_addExperiment(max_cars, max_transfer, \
            avg_request_return_A, avg_request_return_B, \
            rent, transfer_cost, gamma)

    def evaluate_policy(self):
        return rl.bellman_jcr_evaluatePolicy(self.id)

    def improve_policy(self):
        return rl.bellman_jcr_improvePolicy(self.id)

    def run(self):
        s = 1
        while s != 0:
            v, i = self.evaluate_policy()
            p, s = self.improve_policy()
            yield p, v, (i, s)

    def __del__(self):
        return rl.bellman_jcr_endExperiment(self.id)
