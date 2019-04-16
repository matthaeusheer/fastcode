import matplotlib
import matplotlib.pyplot as plt


def setup_figure_1ax(x_label='', y_label='', size=(13, 9), shrink_ax=True):
    """Returns a (figure, ax) tuple with legend on the right hand side, no spines."""

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
    ax.grid()
    return fig, ax


def get_color_cm_jet(cmap, vmin, vmax, value):
    norm = matplotlib.colors.Normalize(vmin=vmin, vmax=vmax)
    return [val / 255 for val in cmap(norm(value), bytes=True)]


def get_n_until_first_repeat(input_list, exclude_start='#'):
    """Get the number of elements until a repeat comes up. Assumes that this repeat will actually happen.
    Lines starting with # are ignored by default.
    """
    start = input_list[0]

    counter = 1
    for item in input_list[1:]:
        if item is not start:
            if exclude_start and item.startswith(exclude_start):
                continue
            counter += 1
        else:
            return counter
