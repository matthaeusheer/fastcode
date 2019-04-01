import numpy as np

import matplotlib
import matplotlib.pyplot as plt
from fastpy.io.output import OutputParser


def plot_mean_runtime_vs_input_size(out_parser: OutputParser):
    """For all algorithms present in the out_parser data, plot mean runtime vs input size."""

    config = out_parser.config
    sub_configs = out_parser.sub_configs

    algos = config['algorithm']
    obj_funcs = config['obj_func']

    timings = out_parser.parse_timings(return_lists=True)

    mean_timings = {run: np.mean(times) for run, times in timings.items()}

    algo_times_vs_size = {algo: {} for algo in algos}

    for algo in algos:
        for obj_func in obj_funcs:

            algo_times_vs_size[algo][obj_func] = {'times': [], 'sizes': []}

            for run, sub_config in sub_configs.items():

                if sub_config['algorithm'] == algo and sub_config['obj_func'] == obj_func:

                    algo_times_vs_size[algo][obj_func]['times'].append(mean_timings[run])
                    algo_times_vs_size[algo][obj_func]['sizes'].append(sub_config['dimension'])

    fig, ax = setup_figure_1ax(x_label='Input size', y_label='Mean cycles')

    for algo, obj_func_dict in algo_times_vs_size.items():
        for obj_func, data_dict in obj_func_dict.items():
            sizes, times = sort_two_lists_based_on_first(data_dict['sizes'], data_dict['times'])

            ax.plot(sizes, times, label='_'.join([algo, obj_func]))

    ax.legend(frameon=False)
    plt.show()

    return algo_times_vs_size


def setup_figure_1ax(x_label='', y_label='', size=(13, 9), shrink_ax=True):
    """Returns a figure and ax with legend on the right hand side, no spines."""

    matplotlib.rcParams.update({'font.size': 20})
    fig, ax = plt.subplots()
    fig.set_size_inches(size)
    ax.set_xlabel(x_label)
    ax.set_ylabel(y_label)
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.spines['bottom'].set_visible(False)
    ax.spines['left'].set_visible(False)
    # Shrink current axis by 20%
    if shrink_ax:
        box = ax.get_position()
        ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])

    return fig, ax


def sort_two_lists_based_on_first(first, second):

    return sorted(first), [x for _, x in sorted(zip(first, second))]
