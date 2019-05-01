import os
from glob import glob

import pandas as pd

from common import DATA_DIR_PATH
from fastpy.io.config import load_json_config
from fastpy import utils
from fastpy.run.Runner import SUB_DIR_PATTERN, CONFIG_FILE_NAME, RUN_CONFIG_FILE_NAME, TIMING_OUT_FILE, \
    SOLUTION_OUT_FILE


class OutputParser:
    """Parse the output which has been created by the fastpy/run/Runner.py module."""

    def __init__(self, run_name=None, use_most_recent=True, data_dir=DATA_DIR_PATH):
        if use_most_recent:
            run_name = OutputParser._most_recent_run()
        else:
            if run_name:
                print(f'Loading USER SPECIFIED run: {run_name}')
                run_name = run_name
            else:
                raise ValueError('Need to specify either a run_name or use_most_recent=True')
        self.out_dir = os.path.join(data_dir, run_name)

    @staticmethod
    def _most_recent_run():
        runs = os.listdir(DATA_DIR_PATH)
        if 'data_readme.txt' in runs:
            runs.remove('data_readme.txt')
        assert len(runs) != 0, 'There are no runs to load. fastpy data folder is empty.'
        tags = [run.split('_', 1)[1] for run in runs]
        date_times = [utils.load_datetime_from_string_tag(tag) for tag in tags]
        sorted_runs, sorted_date_times = utils.sort_two_lists_based_on_first(runs, date_times)
        most_recent_run = sorted_runs[-1]
        print(f'Loading MOST RECENT run: {most_recent_run}')
        return most_recent_run

    def _get_sub_folders_list(self):
        """Get a list of sub run folder names, e.g. ['run_0', 'run_1', ...]."""
        paths = glob(os.path.join(self.out_dir, SUB_DIR_PATTERN.format(run_idx='*')))
        return [os.path.split(path)[-1] for path in paths]

    @property
    def config(self):
        """Config dictionary holding information about all runs performed."""
        return load_json_config(os.path.join(self.out_dir, CONFIG_FILE_NAME))

    @property
    def sub_configs(self):
        """Dictionary with sub_run_name keys (e.g. run_1) and run_config values (dict)."""
        sub_configs = {}
        for sub_dir_name in self._get_sub_folders_list():
            sub_configs[sub_dir_name] = load_json_config(os.path.join(self.out_dir, sub_dir_name, RUN_CONFIG_FILE_NAME))

        return sub_configs

    def parse_timings(self, file_name=TIMING_OUT_FILE, return_lists=False):
        """For all sub runs, load a list of timing measurements for the performed number of iterations.

        Returns
        -------
        timings: dict of pandas data frames if return_lists=False, else same dict with lists values

        """
        timings = {}

        for sub_dir_name in self._get_sub_folders_list():
            with open(os.path.join(self.out_dir, sub_dir_name, file_name), 'r') as infile:
                timings[sub_dir_name] = pd.read_csv(infile)

        if return_lists:
            return {sub_run: list(time_df.loc[:, 'cycles']) for sub_run, time_df in timings.items()}

        return timings

    def parse_solutions(self, file_name=SOLUTION_OUT_FILE, return_lists=False):
        """For all sub runs, load a list of timing measurements for the performed number of iterations.

        Returns
        -------
        solutions: dict of pandas data frames if return_lists=False, else same dict with lists values
        """
        solutions = {}

        for sub_dir_name in self._get_sub_folders_list():
            with open(os.path.join(self.out_dir, sub_dir_name, file_name), 'r') as infile:
                solutions[sub_dir_name] = pd.read_csv(infile)

        if return_lists:
            return {sub_run: list(time_df.loc[:, 'value']) for sub_run, time_df in solutions.items()}

        return solutions