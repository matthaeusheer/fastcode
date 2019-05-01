import numpy as np

import matplotlib.pyplot as plt

from fastpy.utils import sort_two_lists_based_on_first
from fastpy.visualization import viz_utils
from fastpy.io.output_loader import OutputParser


def plot_mean_runtime_vs_input_size(out_parser: OutputParser):
    """For all algorithms present in the out_parser data, plot mean runtime vs input size.

    NOTE: We fix the dimension to be equal for all runs. Only works if only one dimension specified in config.
    """

    config = out_parser.config
    sub_configs = out_parser.sub_configs

    if len(config['dimension']) != 1:
        raise ValueError(f'We fix the dimension and vary population size. Only one dimension allowed. '
                         f'Given: {config["dimension"]}')

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
                    algo_times_vs_size[algo][obj_func]['sizes'].append(sub_config['population'])

    fig, ax = viz_utils.setup_figure_1ax(x_label='Input size [population]', y_label='Mean cycles',
                                         title=f'Search space dimension: {config["dimension"][0]}')

    for algo, obj_func_dict in algo_times_vs_size.items():
        for obj_func, data_dict in obj_func_dict.items():
            sizes, times = sort_two_lists_based_on_first(data_dict['sizes'], data_dict['times'])

            ax.plot(sizes, times, label='_'.join([algo, obj_func]))

    ax.legend(frameon=False)
    plt.show()

    return algo_times_vs_size
