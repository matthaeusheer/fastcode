import os
from glob import glob


def get_files_by_template(dir_path, file_name_template):
    """Returns a list of file paths for files in dir_path which the file_name_template fits.

    Arguments
    ---------
        dir_path: path to directory in which to search for files
        file_name_template: a string template with one argument position, e.g. file_name_{}.txt
    Returns
    -------
        list of file paths
    """
    return glob(os.path.join(dir_path, file_name_template.format('*')))

