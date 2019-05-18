import numpy as np

import matplotlib.pyplot as plt

from fastpy.utils import sort_two_lists_based_on_first
from fastpy.visualization import viz_utils
from fastpy.io.output_loader import OutputParser
from fastpy.evaluation.performance_calculations import FlopCounter


# For display in plots.
UNITS_MAP = {'op_intensity': '[Flops/Bytes]',
             'flop_count': '',
             'mem_move_bytes': '[Bytes]',
             'mem_move_floats': '[# Floats]',
             'performance': '[Flops/Cycle]',
             'mean_runtime': '[Cycles]'}

# For nicer formatting in plots.
LABEL_MAP = {'op_intensity': 'Operational Intensity',
             'flop_count': 'Flop Count',
             'mem_move_bytes': 'Memory Movement',
             'mem_move_floats': 'Memory Movement',
             'performance': 'Performance',
             'n_iter': 'Number of Iterations',
             'population': 'Population Size',
             'n_rep': 'N Repetitions',
             'dimension': 'Search Space Dimension',
             'algorithm': 'Algorithm',
             'obj_func': 'Objective Function',
             'mean_runtime': 'Runtime'}


def plot_mean_runtime_vs_input_size(out_parser: OutputParser, plot_type='performance'):
    """For all algorithms present in the out_parser data, plot mean runtime vs input size.

    NOTE: We fix the dimension to be equal for all runs. Only works if only one dimension specified in config.
    """
    plot_types = ['performance', 'runtime']

    config = out_parser.config
    sub_configs = out_parser.sub_configs

    if len(config['dimension']) != 1:
        raise ValueError(f'We fix the dimension and vary population size. Only one dimension allowed. '
                         f'Given: {config["dimension"]}')

    assert plot_type in plot_types, f'Plot type argument needs to be one of {plot_types}.'

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

                    y_label = ' '.join([LABEL_MAP[plot_type], UNITS_MAP[plot_type]])
                    if plot_type == 'performance':
                        flop_counter = FlopCounter(sub_config)
                        flops = flop_counter.flop_count()
                        algo_quantity_vs_size[algo][obj_func][plot_type].append(flops/mean_timings[run])
                    elif plot_type == 'runtime':
                        algo_quantity_vs_size[algo][obj_func][plot_type].append(mean_timings[run])

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


def plot_perf_metric_vs_config_val(perf_metrics, x_key, y_key, ax=None, label=None):
    """Given a dict of performanc_metric dicts over runs, one dict looks like so
        {'config': {'algorithm': 'pso',
                    'obj_func': 'rosenbrock',
                    'dimension': 64,
                    'n_rep': 1,
                    'n_iter': 1832,
                    'population': 512,
                    'min_val': -100,
                    'max_val': 100},
         'op_intensity': 2947.808607796262,
         'flop_count': 1193603079,
         'mem_move_bytes': 404912,
         'mem_move_floats': 101228,
         'performance': 0.3243619732473652},
     plot values from the config, e.g. n_iter, over runs vs calculated performance metrics, e.g. performance.
     """
    assert x_key in list(perf_metrics.values())[0]['config'], 'x axis values should be keys in run config.'
    assert y_key in [item for item in list(perf_metrics.values())[0].keys() if item != 'config'], \
        'y axis values should be calculated performance quantities.'

    if label is None:
        label = ' '.join([LABEL_MAP[y_key], UNITS_MAP[y_key]])

    if ax is None:
        fig, ax = viz_utils.setup_figure_1ax(x_label=LABEL_MAP[x_key], y_label=label)

    x_val_list = []
    y_val_list = []

    for metrics in perf_metrics.values():
        x_val_list.append(metrics['config'][x_key])
        y_val_list.append(metrics[y_key])

    ax.plot(x_val_list, y_val_list, 'o', label=label)
    return ax


def plot_multiple_perf_metric_vs_config_val(perf_metrics, x_key, y_keys, ax=None):
    """Same as plot_perf_metric_vs_config_val but for multiple y values."""
    if ax is None:
        fig, ax = viz_utils.setup_figure_1ax(x_label=x_key, y_label='')
    for y_val in y_keys:
        plot_perf_metric_vs_config_val(perf_metrics, x_key, y_val, ax=ax)
    ax.legend()
