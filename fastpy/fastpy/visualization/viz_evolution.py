import numpy as np
from matplotlib import ticker, cm
import mpld3

from fastpy.visualization import viz_utils
from fastpy.obj_funcs import rosenbrock

OBJ_FUNCS = {'rosenbrock': rosenbrock}


def plot_optimization_evolution_2d(evolution_data, *args, obj_func=None, **kwargs):
    """For a given population, plot their positions in solution space in 2d over time.

    Arguments
    ---------
        evolution_data: list over iterations where each entry is a dict of population members
                        holding their 2d positions as a list
        xlims (optional): list of lower and upper x lim for plot
        ylims (optional): list of lower and upper y lim for plot
        obj_func: name of the objective function to contour plot in the background
    """
    mpld3.enable_notebook()
    fig, ax = viz_utils.setup_figure_1ax(x_label='x position', y_label='y_position', size=(8, 8), shrink_ax=False)

    if obj_func:
        if obj_func not in OBJ_FUNCS:
            raise NotImplementedError(f'{obj_func} is not implemented for plotting. '
                                      f'Only {list(OBJ_FUNCS.keys())} can be plotted.\n'
                                      f'Feel free to implement them in python :)')

        mpld3.disable_notebook()  # contour plots cannot be json serialized so we have to switch of d3 mode
        min_x, max_x, min_y, max_y = viz_utils.get_min_max_of_evolution_data(evolution_data)
        x_list = np.linspace(min_x, max_x, 100)
        y_list = np.linspace(min_y, max_y, 100)
        x_mesh, y_mesh = np.meshgrid(x_list, y_list)

        z_mesh = OBJ_FUNCS[obj_func](np.vstack([x_mesh.ravel(), y_mesh.ravel()])).reshape((100, 100))

        ax.contourf(x_mesh, y_mesh, z_mesh, [rosenbrock(np.array([k, k])) for k in np.linspace(1, 20, 50)], cmap='jet',
                    locator=ticker.LogLocator(), alpha=0.1)

    for iter_idx, step_dict in enumerate(evolution_data):

        if iter_idx == 0:
            color = 'black'
        else:
            color = viz_utils.get_color_from_cm(cm.hot, 1, len(evolution_data), iter_idx + 1)

        x = [pos[0] for pos in step_dict.values()]
        y = [pos[1] for pos in step_dict.values()]

        ax.plot(x, y, '.', color=color, alpha=0.7, markeredgewidth=0.0)

    if 'xlims' in kwargs:
        ax.set_xlim(kwargs['xlims'])
    if 'ylims' in kwargs:
        ax.set_ylim(kwargs['ylims'])

    return fig, ax


def simulate_optimization_evolution_2d(evolution_data, best_obj_values):
    """Instead of a static plot, this produces a plotly simulation of the evolution of the data."""
    iterations = range(len(evolution_data))

    # make figure
    figure = {
        'data': [],
        'layout': {},
        'frames': []
    }

    # fill in most of layout
    min_x, max_x, min_y, max_y = viz_utils.get_min_max_of_evolution_data(evolution_data)
    figure['layout']['xaxis'] = {'title': 'x', 'range': [min_x, max_x], 'zeroline': False}
    figure['layout']['yaxis'] = {'title': 'y', 'range': [min_y, max_y], 'zeroline': False}
    figure['layout']['hovermode'] = 'closest'
    figure['layout'].update(width=700, height=700, autosize=False)
    figure['layout']['sliders'] = {
        'args': [
            'transition', {
                'duration': 10,
                'easing': 'cubic-in-out'
            }
        ],
        'initialValue': 0,
        'plotlycommand': 'animate',
        'values': iterations,
        'visible': True
    }
    figure['layout']['updatemenus'] = [
        {
            'buttons': [
                {
                    'args': [None, {'frame': {'duration': 200, 'redraw': False},
                                    'fromcurrent': True,
                                    'transition': {'duration': 200, 'easing': 'quadratic-in-out'}}],
                    'label': 'Play',
                    'method': 'animate'
                },
                {
                    'args': [[None], {'frame': {'duration': 0, 'redraw': False}, 'mode': 'immediate',
                                      'transition': {'duration': 0}}],
                    'label': 'Pause',
                    'method': 'animate'
                }
            ],
            'direction': 'left',
            'pad': {'r': 10, 't': 87},
            'showactive': True,
            'type': 'buttons',
            'x': 0.1,
            'xanchor': 'right',
            'y': 0,
            'yanchor': 'top'
        }
    ]

    sliders_dict = {
        'active': 0,
        'yanchor': 'top',
        'xanchor': 'left',
        'currentvalue': {
            'font': {'size': 20},
            'prefix': 'Iteration:',
            'visible': True,
            'xanchor': 'right'
        },
        'transition': {'duration': 10, 'easing': 'cubic-in-out'},
        'pad': {'b': 10, 't': 50},
        'len': 0.9,
        'x': 0.1,
        'y': 0,
        'steps': []
    }

    # make data

    x_positions = []
    y_positions = []
    for iter_idx, step_dict in enumerate(evolution_data):
        x = [pos[0] for pos in step_dict.values()]
        y = [pos[1] for pos in step_dict.values()]
        x_positions.append(x)
        y_positions.append(y)

    data_points = {'x': x_positions[0],
                   'y': y_positions[0],
                   'mode': 'markers',
                   'marker': {'color': 'black'}}

    figure['data'].append(data_points)

    # make frames
    iter_idx = 1
    for x, y in zip(x_positions, y_positions):
        frame = {'data': [], 'name': str(iter_idx)}

        color = viz_utils.get_color_from_cm(cm.winter_r, min(best_obj_values), max(best_obj_values), best_obj_values[iter_idx-1], hex=True)
        data_dict = {
            'x': x,
            'y': y,
            'mode': 'markers',
            'marker': {'color': color}
        }
        frame['data'].append(data_dict)

        figure['frames'].append(frame)
        slider_step = {'args': [
            [str(iter_idx)],
            {'frame': {'duration': 10, 'redraw': False},
             'mode': 'immediate',
             'transition': {'duration': 10}}
        ],
            'label': str(iter_idx),
            'method': 'animate'}
        sliders_dict['steps'].append(slider_step)
        iter_idx += 1

    figure['layout']['sliders'] = [sliders_dict]

    return figure


def plot_objective_value_evolution(values):
    """Plots an array of objective values vs iteration index.
    Arguments
    --------
        values: a list of objective values
    """
    fig, ax = viz_utils.setup_figure_1ax('Iteration index', 'Average objective value', shrink_ax=False)

    ax.plot(range(len(values)), values, 'c-', linewidth=2)
    return fig, ax


def dummy_evolution_data(pop_size=10, n_iterations=5):
    """Creates dummy data in the format the evolution plotting function needs it, that is

    a list of dicts where each dicts represents the population at a particular iteration. The dict has keys
    of member names and a list which is the current state of the member in dim dimensions.
    """
    def sample_circle(npoints, ndim=2, radius=1):
        vec = np.random.randn(ndim, npoints)
        vec /= (np.linalg.norm(vec, axis=0) * radius)
        return vec

    evolution_data = []

    for iter_idx in range(n_iterations):

        step_dict = {}
        for member in range(pop_size):
            step_dict[f'm_{member}'] = sample_circle(1, radius=iter_idx + 1).transpose()[0]
        evolution_data.append(step_dict)
    return evolution_data
