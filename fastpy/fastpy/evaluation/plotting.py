import numpy as np

import matplotlib.pyplot as plt

from fastpy.utils import sort_two_lists_based_on_first
from fastpy.visualization import viz_utils
from fastpy.io.output_loader import OutputParser
from fastpy.evaluation.performance_calculations import FlopCounter


PLOT_TYPES = ['performance', 'runtime']


def plot_mean_runtime_vs_input_size(out_parser: OutputParser, plot_type='performance'):
    """For all algorithms present in the out_parser data, plot mean runtime vs input size.

    NOTE: We fix the dimension to be equal for all runs. Only works if only one dimension specified in config.
    """

    config = out_parser.config
    sub_configs = out_parser.sub_configs

    if len(config['dimension']) != 1:
        raise ValueError(f'We fix the dimension and vary population size. Only one dimension allowed. '
                         f'Given: {config["dimension"]}')

    assert plot_type in PLOT_TYPES, f'Plot type argument needs to be one of {PLOT_TYPES}.'

    algos = config['algorithm']
    obj_funcs = config['obj_func']

    timings = out_parser.parse_timings(return_lists=True)

    mean_timings = {run: np.mean(times) for run, times in timings.items()}

    algo_quantity_vs_size = {algo: {} for algo in algos}  # quantity is either cycle or flop / cycle

    for algo in algos:
        for obj_func in obj_funcs:

            algo_quantity_vs_size[algo][obj_func] = {plot_type: [], 'sizes': []}

            for run, sub_config in sub_configs.items():

                if sub_config['algorithm'] == algo and sub_config['obj_func'] == obj_func:

                    if plot_type == 'performance':
                        flop_counter = FlopCounter(sub_config)
                        flops = flop_counter.flop_count()
                        algo_quantity_vs_size[algo][obj_func][plot_type].append(flops/mean_timings[run])
                        y_label = 'Performance [flops/cycle]'
                    elif plot_type == 'runtime':
                        algo_quantity_vs_size[algo][obj_func][plot_type].append(mean_timings[run])
                        y_label = 'Runtime [cycles]'

                    algo_quantity_vs_size[algo][obj_func]['sizes'].append(sub_config['population'])

    fig, ax = viz_utils.setup_figure_1ax(x_label='Input size [population]', y_label=y_label,
                                         title=f'Search space dimension: {config["dimension"][0]}')

    for algo, obj_func_dict in algo_quantity_vs_size.items():
        for obj_func, data_dict in obj_func_dict.items():
            sizes, times = sort_two_lists_based_on_first(data_dict['sizes'], data_dict[plot_type])

            ax.plot(sizes, times, label='_'.join([algo, obj_func]))

    ax.legend(frameon=False)
    plt.show()

    return algo_quantity_vs_size
