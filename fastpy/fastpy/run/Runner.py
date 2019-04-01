import os
import itertools
from subprocess import call

from fastpy.io.config import load_json_config, store_json_config
from fastpy.utils import get_date_time_tag
from common import DATA_DIR_PATH, PROJECT_ROOT_PATH

# This mapping will be used to pass command line arguments to benchmark executable
PARAM_TO_C_MAP = {'algorithm':  '-a',
                  'obj_func':   '-o',
                  'dimension':  '-d',
                  'n_iter':     '-n',
                  'population': '-p',
                  'min_val':    '-y',
                  'max_val':    '-z'}

BENCHMARK_BIN_DIR = os.path.join(PROJECT_ROOT_PATH, '../benchmark')
BENCHMARK_BIN = 'benchmark'

TIMING_OUT_FILE = 'timings.csv'
SOLUTION_OUT_FILE = 'solution.csv'


class BenchmarkRunner:

    def __init__(self, config_path, data_dir=DATA_DIR_PATH):
        self.config = load_json_config(config_path)
        self.data_dir = data_dir
        self.timestamp = get_date_time_tag()

    def run_benchmarks(self):
        """Main wrapper function to loop over all possible parameter combinations."""

        os.mkdir(self._output_dir)  # main output dir for this whole run

        for run_idx, run_config in enumerate(self._build_param_sets()):

            sub_dir = os.path.join(self._output_dir, f'run_{run_idx}')  # sub output dir for one param combo
            os.mkdir(sub_dir)

            store_json_config(run_config, sub_dir, 'run_config.json')

            self._run_algorithm(run_config, sub_dir)

    def _run_algorithm(self, run_config, sub_dir):
        """Subprocess call to run a single algorithm."""

        call_str = ' '.join([self._benchmark_bin, self._create_params_str(run_config)])
        call_str += ' -f ' + os.path.join(sub_dir, TIMING_OUT_FILE)
        call_str += ' -s ' + os.path.join(sub_dir, SOLUTION_OUT_FILE)

        call(call_str, shell=True)

    def _build_param_sets(self):
        """Returns a list of dictionaries, where the dictionaries are a set of parameters for a single run."""
        param_keys = self.config.keys()
        param_values = self.config.values()

        param_combos = list(itertools.product(*param_values))

        param_sets = []
        for combo in param_combos:
            param_sets.append({param: value for param, value in zip(param_keys, combo)})

        return param_sets

    @staticmethod
    def _create_params_str(run_config):

        param_str = ''
        for param, value in run_config.items():
            param_str += PARAM_TO_C_MAP[param]
            param_str += ' '
            param_str += str(value)
            param_str += ' '

        return param_str

    def _check_bin_exists(self):
        """Returns True if benchmark binary exists, otherwise False."""
        assert os.path.exists(self._benchmark_bin)

    @property
    def _benchmark_bin(self):
        return os.path.join(BENCHMARK_BIN_DIR, BENCHMARK_BIN)

    @property
    def _output_dir(self):
        return os.path.join(self.data_dir, f'run_{self.timestamp}')

    def __repr__(self):
        return 'Benchmark running wrapper class.\n' + \
                'Config: ' + str(self.config) + '\n' + \
                'Output dir: ' + DATA_DIR_PATH + '\n' + \
                'Benchmark binary: ' + self._benchmark_bin