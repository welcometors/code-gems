import warnings
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from cppLib.bellman import JacksCarRental


def data_formatter(data):
    def format_coord(x, y):
        ix, iy = int(x), int(y)
        v = 'N/A'
        try:
            v = data[iy][ix]
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
    a, b = min(min(values)), max(max(values))
    ticks = []
    for i in np.linspace(a, b, n_ticks):
        ticks.append(i)
    return ticks


def main():
    # whether to animate plots or not
    animate = True

    experiment = JacksCarRental(max_cars=20, max_transfer=5, rent=10, transfer_cost=2, \
        avg_request_return_A=(3,3), avg_request_return_B=(4,2), gamma=.9)

    for iter, (policy, values, stats) in enumerate(experiment.run()):
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
            .format(iter + 1, stats[0], stats[1]), y=.9)
       
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