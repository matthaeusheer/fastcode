import matplotlib
import matplotlib.pyplot as plt


def setup_figure_1ax(x_label='', y_label='', title='', size=(13, 9), shrink_ax=True):
    """Returns a (figure, ax) tuple with legend on the right hand side, no spines."""

    matplotlib.rcParams.update({'font.size': 20})
    fig, ax = plt.subplots()
    fig.set_size_inches(size)
    ax.set_xlabel(x_label)
    ax.set_ylabel(y_label)
    ax.set_title(title)
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


def get_color_from_cm(cmap, vmin, vmax, value, hex=True):
    norm = matplotlib.colors.LogNorm(vmin=vmin, vmax=vmax)
    rgba = cmap(norm(value))
    if hex:
        return matplotlib.colors.rgb2hex(rgba[:3])
    else:
        return rgba


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


def get_min_max_of_evolution_data(evolution_data):
    global_min_x = 10e6
    global_min_y = 10e6
    global_max_x = -10e6
    global_max_y = -10e6

    for step_dict in evolution_data:
        x = [pos[0] for pos in step_dict.values()]
        y = [pos[1] for pos in step_dict.values()]

        max_x = max(x)
        max_y = max(y)
        min_x = min(x)
        min_y = min(y)

        global_min_x = min(global_min_x, min_x)
        global_min_y = min(global_min_y, min_y)
        global_max_x = max(global_max_x, max_x)
        global_max_y = max(global_max_y, max_y)

    return global_min_x, global_max_x, global_min_y, global_max_y


def rgb2hex(r,g,b):
    return "#{:02x}{:02x}{:02x}".format(r,g,b)


def hex2rgb(hexcode):
    return tuple(map(ord,hexcode[1:].decode('hex')))
