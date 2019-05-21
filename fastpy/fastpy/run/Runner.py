import os
import shutil
import itertools
import subprocess
from pprint import pprint

from fastpy.io.config import load_json_config, store_json_config
from fastpy.utils import get_date_time_tag
from common import DATA_DIR_PATH, PROJECT_ROOT_PATH

# This mapping will be used to pass command line arguments to benchmark executable
PARAM_TO_C_MAP = {'algorithm':  '-a',
                  'obj_func':   '-o',
                  'dimension':  '-d',
                  'n_iter':     '-n',
                  'n_rep':      '-m',
                  'population': '-p',
                  'min_val':    '-y',
                  'max_val':    '-z'}

BENCHMARK_BIN = 'benchmark'

TIMING_OUT_FILE = 'timings.csv'
SOLUTION_OUT_FILE = 'solution.csv'

CONFIG_FILE_NAME = 'config.json'
RUN_CONFIG_FILE_NAME = 'run_config.json'
SUB_DIR_PATTERN = 'run_{run_idx}'

RELEASE_BUILD_SCRIPT = 'build-releases.sh'
RELEASES_BUILD_DIR = 'build-releases'


class BenchmarkRunner:

    def __init__(self, config_path, data_dir=DATA_DIR_PATH, bin_dir=None, bin_name=None):
        self.config = load_json_config(config_path)
        self.data_dir = data_dir
        self.timestamp = get_date_time_tag()
        self.bin_dir = bin_dir
        self.bin_name = bin_name if bin_name else BENCHMARK_BIN

    def run_benchmarks(self):
        """Main wrapper function to loop over all possible parameter combinations. Returns output dir name."""
        os.mkdir(self._output_dir)  # main output dir for this whole run
        shutil.copy(os.path.join(self._bin_dir_path, self.bin_name), self._output_dir)
        store_json_config(self.config, self._output_dir, CONFIG_FILE_NAME)

        for run_idx, run_config in enumerate(self._build_param_sets()):

            sub_dir = os.path.join(self._output_dir, SUB_DIR_PATTERN.format(run_idx=run_idx))  # sub output dir one param combo
            os.mkdir(sub_dir)

            store_json_config(run_config, sub_dir, RUN_CONFIG_FILE_NAME)

            self._run_algorithm(run_config, sub_dir)
        return self._output_dir

    def _run_algorithm(self, run_config, sub_dir):
        """Subprocess call to run a single algorithm."""
        call_str = ' '.join([self._benchmark_bin, self._create_params_str(run_config)])
        call_str += ' -f ' + os.path.join(sub_dir, TIMING_OUT_FILE)
        call_str += ' -s ' + os.path.join(sub_dir, SOLUTION_OUT_FILE)

        try:
            subprocess.check_call(call_str, shell=True)
        except subprocess.CalledProcessError as exception:
            # TODO: This is a first step, better would be using a proper logging system.
            print(exception)
            print(f'\nThe call to the binary executable failed. Tried parameters: \n')
            pprint(run_config)
            print('\nAbort.')
            exit(1)

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
        """Given a run_config, dict-type configuration for one single execution of the benchmark executable, build
        a string which represents the parameters on the command line ready to be passed to the executable."""
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
        return os.path.join(self.bin_dir, self.bin_name)

    @property
    def _output_dir(self):
        return os.path.join(self.data_dir, f'run_{self.timestamp}')

    @property
    def _bin_dir_path(self):
        return os.path.join(PROJECT_ROOT_PATH, self.bin_dir)

    def __repr__(self):
        return 'Benchmark running wrapper class.\n' + \
                'Config: ' + str(self.config) + '\n' + \
                'Output dir: ' + DATA_DIR_PATH + '\n' + \
                'Benchmark binary: ' + self._benchmark_bin


class MultiBenchmarkRunner:
    """Lets us perform runs for a config over several releases. The releases are compiled using the build-releases.sh
    shell script in the fastcode project root folder. Specify the release tags in this file."""

    def __init__(self, config_path, data_dir=DATA_DIR_PATH):
        self.config_path = config_path
        self.data_dir = data_dir

    def perform_runs(self, force_compile=False):
        """Performs a benchmark run for every tagged executable in the RELEASES_BUILD_DIR.
        Returns
        -------
            tag_out_dict: a dictionary mapping tags to output dir names of the respective runs
        """
        if force_compile:
            self._compile_executables()
        tag_out_dir_dict = {}
        for tag in sorted(self._parse_tags()):
            print(f'Running benchmarks for tag {tag}...')
            bin_dir = os.path.join(self.fastcode_root_path, RELEASES_BUILD_DIR)
            bin_name = f'benchmark_{tag}'

            benchmark_runner = BenchmarkRunner(config_path=self.config_path, data_dir=self.data_dir,
                                               bin_dir=bin_dir, bin_name=bin_name)

            run_dir_path = benchmark_runner.run_benchmarks()
            tag_out_dir_dict[tag] = os.path.split(run_dir_path)[1]
        return tag_out_dir_dict

    def _compile_executables(self):
        subprocess.call(['sh', os.path.join(self.fastcode_root_path, RELEASE_BUILD_SCRIPT)],
                        cwd=self.fastcode_root_path)

    def _parse_tags(self):
        if not os.path.exists(os.path.join(self.fastcode_root_path, RELEASES_BUILD_DIR)):
            raise FileNotFoundError('Executable dir not found. '
                                    'You might want to run the build-releases.sh shell script or run the'
                                    'function perform_runs with force_compile=True.')
        files = os.listdir(os.path.join(self.fastcode_root_path, RELEASES_BUILD_DIR))
        executables = [file for file in files if 'benchmark_' in file]
        tags = [name.split('_')[1] for name in executables]
        return tags

    @property
    def fastcode_root_path(self):
        return os.path.dirname(PROJECT_ROOT_PATH)