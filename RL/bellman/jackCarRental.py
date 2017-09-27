# python

import copy
import math
import random
import warnings
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
from decimal import *
from collections import namedtuple
from _collections import defaultdict


def poisson(l, n):
    s, p = 0, math.exp(-l)
    for k in range(n):
        yield p
        s += p
        p *= l
        p /= k + 1
    yield 1 - s


class Station:
    def __init__(self, rent, avg_request, avg_return, max_capacity, max_transfer):
        max_limit = max_capacity + max_transfer
        self.reward = np.zeros(max_limit + 1)
        self.probability = np.zeros((max_limit + 1, max_capacity + 1))
        for cars in range(max_limit + 1):
            for n_request, p_request in enumerate(poisson(avg_request, max_limit)):
                for n_return, p_return in enumerate(poisson(avg_return, max_limit)):
                    #old bugged version
                    #n_rented = min(cars, n_request)
                    n_rented = min(cars + n_return, n_request)
                    self.reward[cars] += rent*n_rented*p_request*p_return
                    gone = min(cars, n_request)
                    cars_eod = min(cars + n_return - gone, max_capacity)
                    self.probability[cars,cars_eod] += p_request*p_return
        self.max_capacity = max_capacity
        self.max_limit = max_limit


class Environment():
    def __init__(self, station_a, station_b, transfer_cost):
        self.station_a = station_a
        self.station_b = station_b
        self.transfer_cost = transfer_cost

    def next(self, state, values, moved, discount):
        cars_a, cars_b = state
        cars_a -= moved
        cars_b += moved
        if cars_a < 0 or cars_b < 0:
            return -9999999
        else:
            new_value = -math.fabs(moved)*self.transfer_cost
            next_states = np.outer(self.station_a.probability[cars_a,:], self.station_b.probability[cars_b,:])
            for next_state, probability in np.ndenumerate(next_states):
                new_value += probability * ( \
                    + self.station_a.reward[cars_a] \
                    + self.station_b.reward[cars_b] \
                    + discount*values[next_state])
            return new_value


def evaluate_policy(environment, states, policy, values, discount, max_iterations=100):
    for iter in range(max_iterations):
        max_delta = 0
        for state in states:
            old_value = values[state]
            values[state] = environment.next(state, values, policy[state], discount)
            max_delta = max(max_delta, math.fabs(values[state] - old_value))
        if max_delta < .001:
            return iter + 1
    return max_iterations


def improve_policy(environment, states, policy, values, actions, discount):
    policy_changes = 0
    for state in states:
        best_action = actions[np.argmax( \
            [environment.next(state, values, action, discount) for action in actions])]
        if best_action != policy[state]:
            policy[state] = best_action
            policy_changes += 1
    return policy_changes


def experiment(environment, actions, discount):
    a_max = environment.station_a.max_capacity
    b_max = environment.station_b.max_capacity
    states = [(x, y) for x in range(a_max + 1) for y in range(b_max + 1)]
    policy = np.zeros((a_max + 1,b_max + 1), dtype=np.int8)
    values = np.zeros((a_max + 1,b_max + 1))
    policy_changes = -1
    while policy_changes != 0:
        iterations = evaluate_policy(environment, states, policy, values, discount)
        policy_changes = improve_policy(environment, states, policy, values, actions, discount)
        yield policy, values, (iterations, policy_changes)


def data_formatter(data):
    def format_coord(x, y):
        ix, iy = int(x), int(y)
        v = 'N/A'
        try:
            v = data[iy, ix]
        except IndexError:
            pass
        return 'x={0}, y={1}, v={2}'.format(ix, iy, v)
    return format_coord


def maximize_plot(manager):
    backend = plt.get_backend().lower()
    if 'tk' in backend:
        manager.window.wm_geometry("+0+0")
        manager.window.state('zoomed')
    elif 'wx' in backend:
        manager.frame.SetPosition((0, 0))
        manager.frame.Maximize(True)
    else:
        manager.window.SetPosition((0, 0))
        manager.window.showMaximized()


def get_ticks(values, n_ticks=10):
    a, b = np.amin(values), np.amax(values)
    ticks = []
    for i in np.linspace(a, b, n_ticks):
        ticks.append(i)
    return ticks


def main():
    # whether to animate plots or not
    animate = True
    n_cars = 20
    rent = 10
    transfer_cost = 2
    max_transfer = 5
    actions = list(range(-max_transfer, max_transfer+1))
    discount = .9
    
    station_a = Station(rent, 3, 3, n_cars, max_transfer)
    station_b = Station(rent, 4, 2, n_cars, max_transfer)
    environment = Environment(station_a, station_b, transfer_cost)

    for iter, (policy, values, stats) in enumerate(experiment(environment, actions, discount)):
        if not animate or iter == 0:
            sns.set()
            figure, axis = plt.subplots(ncols=2)
            cbar_ax = figure.add_axes([.91, .15, .01, .70])
            maximize_plot(plt.get_current_fig_manager())
            policy_color = sns.diverging_palette(255, 133, l=60, n=11, center="dark")

        axis[0].cla()
        sns.heatmap(policy, annot=True, cmap=policy_color, cbar=False, ax=axis[0], vmin=-5, vmax=5).invert_yaxis()
        axis[0].format_coord = data_formatter(policy)
        axis[0].set_title('Policy')

        axis[1].cla()
        cbar_ax.cla()
        cbar_kws = {'ticks': get_ticks(values, 10)}
        sns.heatmap(values, cbar=True, cbar_ax=cbar_ax, cbar_kws=cbar_kws, linewidths=.5, ax=axis[1]).invert_yaxis()
        axis[1].format_coord = data_formatter(values)
        axis[1].set_title('Value Function')
        
        for x in axis:
            x.set_aspect('equal')
            x.set(xlabel='Station B', ylabel='Station A')
        
        plt.xticks(rotation=0) 
        plt.suptitle('Policy Iteration: {0} (Value steps={1}, changes={2})' \
            .format(iter + 1, stats[0], stats[1]))
       
        if animate:
            plt.draw()
            with warnings.catch_warnings():
                warnings.simplefilter("ignore")
                plt.pause(.01)
        else:
            plt.show()

        # plt.savefig('./bellman/plots/jacksCarRental_{0}.png'.format(iter+1))

    # stop at the end of animation to hold plot
    if animate:
        plt.show()


if __name__ == '__main__':
    main()