import numpy as np
from matplotlib import cm

from fastpy.visualization import viz_utils


def plot_optimization_evolution_2d(evolution_data, *args, init_pop=True, **kwargs):
    """For a given population, plot their positions in solution space in 2d over time.

    Arguments
    ---------
        evolution_data: list over iterations where each entry is a dict of population members
                        holding their 2d positions as a list
        xlims (optional): list of lower and upper x lim for plot
        ylims (optional): list of lower and upper y lim for plot
    """

    fig, ax = viz_utils.setup_figure_1ax(x_label='x position', y_label='y_position', size=(8, 8), shrink_ax=False)

    for iter_idx, step_dict in enumerate(evolution_data):

        if iter_idx == 0:
            color = 'black'
        else:
            color = viz_utils.get_color_cm_jet(cm.hot, 0, len(evolution_data), iter_idx - 1)

        x = [pos[0] for pos in step_dict.values()]
        y = [pos[1] for pos in step_dict.values()]

        ax.plot(x, y, '.', color=color, alpha=0.7, markeredgewidth=0.0)

    if 'xlims' in kwargs:
        ax.set_xlim(kwargs['xlims'])
    if 'ylims' in kwargs:
        ax.set_ylim(kwargs['ylims'])

    return fig, ax


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
