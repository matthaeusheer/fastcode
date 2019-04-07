"""Functionality to load and process information which has been put out by the algorithms in #DEBUG mode."""

import numpy as np
import pandas as pd
from fastpy.io import io_utils


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

