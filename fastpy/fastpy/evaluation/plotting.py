import numpy as np

import matplotlib

from fastpy.utils import sort_two_lists_based_on_first
from fastpy.visualization import viz_utils
from fastpy.io.output_loader import OutputParser
from fastpy.evaluation import performance_calculations

# For display in plots.
UNITS_MAP = {'op_intensity':    '[Flops/Bytes]',
             'flop_count':      '',
             'mem_move_bytes':  '[Bytes]',
             'mem_move_floats': '[# Floats]',
             'performance':     '[Flops/Cycle]',
             'mean_runtime':    '[Cycles]'}

# For nicer formatting in plots.
LABEL_MAP = {'op_intensity':    'Operational Intensity',
             'flop_count':      'Flop Count',
             'mem_move_bytes':  'Memory Movement',
             'mem_move_floats': 'Memory Movement',
             'performance':     'Performance',
             'n_iter':          'Number of Iterations',
             'population':      'Population Size',
             'n_rep':           'N Repetitions',
             'dimension':       'Search Space Dimension',
             'algorithm':       'Algorithm',
             'obj_func':        'Objective Function',
             'mean_runtime':    'Mean Runtime'}

MEMORY_BANDWIDTH = 13.1  # Bytes/Cycle
PEAK_PERFORMANCE = 32  # Flops/Cycle

COLORMAPS = ['Reds', 'Oranges', 'Purples', 'Blues', 'Greens']


def prepare_multiple_out_parsers(run_dict):
    """Prepare the out_parser_dict as needed by mult_plot_runtime_performance.

    Arguments
    ---------
        run_dict: dict with run_label keys and run_name values, e.g. {'v0.0.1': 'run_2019-05-18_19-54-14-517664', ...}
    Returns
    -------
        out_parser_dict: dict with run label keys and out_parser values corresponding to run_dict value run names
    """
    output_parser_dict = {}
    for run_label, run_name in run_dict.items():
        output_parser_dict[run_label] = OutputParser(run_name, use_most_recent=False)
    return output_parser_dict


def mult_plot_runtime_performance(out_parser_dict, plot_type='performance', colormap='jet', reverse_legend=False,
                                  plot_over='population', **kwargs):
    """Plot multiple runs (e.g. over different releases) with same configurations in one performance plot."""
    fig, ax = viz_utils.setup_figure_1ax(x_label='Input size [population]',
                                         y_label=' '.join([LABEL_MAP[plot_type], UNITS_MAP[plot_type]]))

    # Check that all runs have same configuration and only one algo / obj func
    first_config = list(out_parser_dict.values())[0].config
    for parser in list(out_parser_dict.values())[1:]:
        if parser.config.keys() != first_config.keys():
            raise AssertionError('For multiple runtime / performance plots, need equal config keys..')
        if parser.config.items() != first_config.items():
            raise AssertionError('For multiple runtime / performance plots, need equal configs.')
    for parser in out_parser_dict.values():
        assert len(parser.config['algorithm']) == 1, 'Only one algorithm over different runs per plot.'
        assert len(parser.config['obj_func']) == 1, 'Only one objective func over different runs per plot.'

    cmap_norm, cmap = norm_cmap(colormap, vmin=0, vmax=len(out_parser_dict))

    idx = 0
    for run_label, out_parser in out_parser_dict.items():
        plot_mean_runtime_vs_input_size(out_parser, plot_type, plot_over=plot_over, ax=ax,
                                        color=cmap(cmap_norm(idx)), label=run_label,
                                        reverse_legend=reverse_legend, **kwargs)
        idx += 1
    ax.set_ylim(bottom=0.0)


def plot_mean_runtime_vs_input_size(out_parser: OutputParser, plot_type='performance', ax=None, color=None, label=None,
                                    reverse_legend=False, plot_over='population', **kwargs):
    """For all algorithms present in the out_parser data, plot mean runtime vs input size.

    NOTE: We fix the dimension to be equal for all runs. Only works if only one dimension specified in config.
    """
    plot_types = ['performance', 'mean_runtime']
    plot_over_choices = ['population', 'dimension']

    assert plot_type in plot_types, f'plot_type argument needs to be one of {plot_types}.'
    assert plot_over in plot_over_choices, f'plot_over argument needs to be one of {plot_over_choices}.'

    config = out_parser.config
    sub_configs = out_parser.sub_configs
<<<<<<< HEAD

    if plot_over == 'population':
        if len(config['dimension']) != 1:
            raise ValueError(f'When plotting over population, fix dimension and vary population size. '
                             f'Only one dimension allowed. '
                             f'Given dimensions: {config["dimension"]}')
=======
    # 
    # if len(config['dimension']) != 1:
    #     raise ValueError(f'We fix the dimension and vary population size. Only one dimension allowed. '
    #                      f'Given: {config["dimension"]}')

    assert plot_type in plot_types, f'Plot type argument needs to be one of {plot_types}.'
>>>>>>> update plot script to increase input size in powers of 2's

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
                        flop_counter = performance_calculations.FlopCounter(sub_config)
                        flops = flop_counter.flop_count()
                        algo_quantity_vs_size[algo][obj_func][plot_type].append(flops/mean_timings[run])
                    elif plot_type == 'mean_runtime':
                        algo_quantity_vs_size[algo][obj_func][plot_type].append(mean_timings[run])

                    algo_quantity_vs_size[algo][obj_func]['sizes'].append(sub_config[plot_over])

    if ax is None:
        if plot_over == 'dimension':
            title = f'Population size: {config["population"][0]}'
        if plot_over == 'population':
            title = f'Search space dimension: {config["dimension"][0]}'
        _, ax = viz_utils.setup_figure_1ax(x_label=f'Input size [{plot_over}]', y_label=y_label,
                                           title=title)

    if color is None:
        cmap_norm, cmap = norm_cmap('jet', vmin=0, vmax=len(sub_configs))
        print('No colormap provided, using jet as default.')
    idx = 1
    for algo, obj_func_dict in algo_quantity_vs_size.items():
        for obj_func, data_dict in obj_func_dict.items():
            sizes, times = sort_two_lists_based_on_first(data_dict['sizes'], data_dict[plot_type])

            if color is None:
                color = cmap(cmap_norm(idx))
            if not label:
                label = '_'.join([algo, obj_func])
            ax.plot(sizes, times, label=label, color=color, linewidth=1.8)
            ax.plot(sizes, times, 'o', label='', color=color)
            idx += 1

    if reverse_legend:
        handles, labels = ax.get_legend_handles_labels()
        ax.legend(reversed(handles), reversed(labels), frameon=False)
    else:
        ax.legend(frameon=False)
    if 'log_xaxis' in kwargs:
        ax.set_xscale("log", nonposx='clip')
    if 'log_yaxis' in kwargs:
        ax.set_yscale("log")

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


def roofline_plot():
    """Produces the bare bone roofline plot for a given memory bandwidth and peak performance of a system."""

    def attainable_performance(operational_intensity):
        return min(PEAK_PERFORMANCE, MEMORY_BANDWIDTH * operational_intensity)

    oi_values = np.logspace(-4, 7, 1000, base=2)
    perf_values = [attainable_performance(oi) for oi in oi_values]
    fig, ax = viz_utils.setup_figure_1ax(x_label='Operational Intensity [Flops/Bytes]',
                                         y_label='Performance [Flops/Cycle]')
    ax.set_xscale("log", basex=2)
    ax.set_yscale("log", basey=2)
    ax.plot(oi_values, perf_values, linewidth=2.0, alpha=0.7)
    ax.set_aspect('equal', adjustable='datalim')

    ridge_point = PEAK_PERFORMANCE / MEMORY_BANDWIDTH
    ax.annotate(f'{{{ridge_point:0.1f}, {PEAK_PERFORMANCE:0.1f}}}',
                xy=(ridge_point, PEAK_PERFORMANCE),
                xytext=(-70, 15), textcoords='offset points',)
    return fig, ax


def perf_in_roofline_plot(perf_metrics, label, color, fig=None, ax=None):
    """Takes all performance metrics from a run and plots the performance over input sizes in the roofline plot."""
    perf_values = []
    input_size_values = []
    op_intensity_values = []
    for metrics in perf_metrics.values():
        perf_values.append(metrics['performance'])
        op_intensity_values.append(metrics['op_intensity'])
        input_size_values.append(metrics['config']['population'])

    if not fig and not ax:
        fig, ax = roofline_plot()
    ax.plot(op_intensity_values, perf_values, color=color, markersize=8, marker='o', label=label)
    ax.plot(op_intensity_values, perf_values, color=color, linewidth=1.5, alpha=0.8)


def prepare_multiple_perf_metrics(run_dict):
    """Returns the multiple_perf_metrics dict which is needed for multiple roofline plotting."""
    multiple_perf_metrics = {}
    for run_label, run_name in run_dict.items():
        output_parser = OutputParser(run_name, use_most_recent=False)
        perf_metrics = performance_calculations.performance_metrics(output_parser)
        multiple_perf_metrics[run_label] = perf_metrics
    return multiple_perf_metrics


def multiple_perf_in_roofline_plot(multiple_perf_metrics, colormap):
    """Plot multiple runs (e.g. for same configuration set but different releases) in one roofline plot."""
    fig, ax = roofline_plot()

    norm = matplotlib.colors.Normalize(vmin=0, vmax=len(multiple_perf_metrics))
    cmap = matplotlib.cm.get_cmap(colormap)

    idx = 0
    for run_label, perf_metrics in multiple_perf_metrics.items():
        print(f'Plotting roofline for {run_label}')
        perf_in_roofline_plot(perf_metrics, run_label, cmap(norm(idx)), fig, ax)
        idx += 1
    ax.legend()


def norm_cmap(cmap_name, vmin, vmax):
    cmap_norm = matplotlib.colors.Normalize(vmin=vmin, vmax=vmax)
    cmap = matplotlib.cm.get_cmap(cmap_name)
    return cmap_norm, cmap
