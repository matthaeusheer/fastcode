"""Functionality to load and process information which has been put out by the algorithms in #DEBUG mode."""

import numpy as np
import pandas as pd
from fastpy.io import io_utils
from fastpy.visualization import viz_utils


def load_solution_data(dir_path, file_name_template):
    """Loads all solution files named by the file_name_template in dir path and puts the data into a pandas df.

    Returns
    -------
        df: rows are iteration indices, columns are dimensions, values represent the solution values
    """
    output_files = io_utils.get_files_by_template(dir_path, file_name_template)
    iter_idx_to_dim_solutions_dict = {idx: np.loadtxt(output_file, delimiter=',', skiprows=1)
                                      for idx, output_file in enumerate(output_files)}

    data_for_df = {}
    for iter_idx, solution in iter_idx_to_dim_solutions_dict.items():
        data_for_df[iter_idx] = solution[:, 1]

    return pd.DataFrame(data_for_df).transpose()


def parse_pen_print_pop_output(file_path, skiprows=10, skipfooter=1):
    """This function parses the output of the penguin print population function such that we can plot the evolution.

    The plotting function is fastpy/visualization/evolution/plot_optimization_evolution_2d.
    This function exists to parse and format the output in a way such that the plotting function can handle it.

    Since printing might change over time we might have to change this function as well.

    Arguments
    ---------
        file_path: absolute path to the file which contains the piped output of the population over iterations
    Returns
    -------
        evolution_data: data in the shape such that the plotting function can handle it
    """
    print(f'Loading printed population output...')
    # the garbage column exist since the C output puts a comma after the last value
    df = pd.read_csv(file_path, skiprows=skiprows, skipfooter=skipfooter,
                     index_col=0, names=['x', 'y', 'garbage'], engine='python')
    df = df[['x', 'y']]

    population_size = viz_utils.get_n_until_first_repeat(df.index)

    evolution_data = []

    counter = 0
    iter_counter = 0
    step_dict = {}
    for pengu, row in df.iterrows():
        step_dict[pengu] = list(row)
        if counter == (population_size - 1):
            evolution_data.append(step_dict)
            iter_counter += 1
            step_dict = {}
            counter = 0
        else:
            counter += 1

    print(f'Detected population size: {population_size}.\n'
          f'{iter_counter} iterations plus initial population.\n'
          f'Data loaded (hopefully you didn\'t forget to print the initial population).')

    return evolution_data


def parse_lines_with_start(file_path, start_string, convert_to_float=True):
    """Parses the values on lines which start with a particular string.
    The value is supposed to follow the start_string, separated by a whitespace.
    """
    parsed_values = []
    with open(file_path, 'r') as infile:
        for line in infile.readlines():
            if line.startswith(start_string):
                value = line.split(' ')[-1].strip()
                if convert_to_float:
                    parsed_values.append(float(value))
                else:
                    parsed_values.append(value)
    return parsed_values






