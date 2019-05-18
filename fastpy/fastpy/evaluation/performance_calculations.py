import numpy as np

from fastpy.io.output_loader import OutputParser

from typing import Dict


class PerfCounter:
    def __init__(self, run_config: Dict) -> None:
        """Base class for FlopCounter and MemoryMoveCounter.

        Arguments
        ---------
            run_config: dict with algorithm parameter and corresponding value
        """
        self.config = run_config

    @property
    def dimension(self):
        return self.config['dimension']

    @property
    def pop_size(self):
        return self.config['population']

    @property
    def obj_func(self):
        return self.config['obj_func']

    @property
    def n_iter(self):
        return self.config['n_iter']

    @property
    def algorithm(self):
        return self.config['algorithm']


class MemoryMoveCounter(PerfCounter):
    def __init__(self, run_config: Dict) -> None:
        """Calculates memory movement for a particular execution of an optimisation algorithm with a given set
        of parameters specified in the run_config (obj_func, iterations, dimensions, ...).

        Arguments
        ---------
            run_config: dict with algorithm parameter and corresponding value
        """
        super().__init__(run_config)

    def mem_movement_bytes(self) -> int:
        dim = self.dimension
        pop = self.pop_size
        n_iter = self.n_iter

        if self.algorithm == 'pso':
            return 16 + 12 * dim * pop + 8 * pop + 4 * (n_iter + dim)
        else:
            raise NotImplementedError(f'No memory usage function for {self.algorithm} yet...')

    def mem_movement_floats(self) -> int:
        dim = self.dimension
        pop = self.pop_size
        n_iter = self.n_iter

        if self.algorithm == 'pso':
            return 4 + 3 * dim * pop + 2 * pop + n_iter + dim
        else:
            raise NotImplementedError(f'No memory usage function for {self.algorithm} yet...')


class FlopCounter(PerfCounter):
    def __init__(self, run_config: Dict) -> None:
        """Calculates the flop count for a particular execution of an optimisation algorithm with a given set
        of parameters specified in the run_config (obj_func, iterations, dimensions, ...).

        Arguments
        ---------
            run_config: dict with algorithm parameter and corresponding value
        """
        super().__init__(run_config)

    def obj_func_flops(self) -> int:
        """Returns the flop count for the particular objective function specified in the run config."""
        if self.obj_func == 'sum_of_squares':
            return self.dimension * 2

        elif self.obj_func == 'sum':
            return self.dimension * 1

        elif self.obj_func == 'rastigrin':
            return self.dimension * 7

        elif self.obj_func == 'rosenbrock':
            return (self.dimension - 1) * 8

        elif self.obj_func == 'sphere':
            return self.dimension * 2

        elif self.obj_func == 'griewank':
            return self.dimension * 6

    def flop_count(self) -> int:
        """Calculates total flop counts for different algorithms depending on the run configuration."""
        # for better readability inside the expressions, have local variables
        pop_size = self.pop_size
        dimension = self.dimension
        obj_func_flops = self.obj_func_flops()
        n_iter = self.n_iter

        if self.algorithm == 'hgwosca':
            flop_count = sum([
                4 * pop_size * dimension,
                pop_size * obj_func_flops,
                31 * pop_size * n_iter * dimension,
                n_iter * pop_size * obj_func_flops
            ])
            return flop_count

        elif self.algorithm == 'squirrel':
            flop_count = sum([
                4 * dimension * pop_size,
                pop_size * obj_func_flops + 5,
                n_iter * (pop_size * obj_func_flops + 5 + 9 * dimension +
                          ((pop_size - 1) * 0.2 - 4) * dimension * 26 +
                          2 * (1 + 0.9 * 11 * dimension * (3 + 0.1 * pop_size) +
                               0.1 * 4 * (3 + 0.1 * pop_size) * dimension
                               )
                          )
            ])
            return flop_count

        elif self.algorithm == 'pso':
            flop_count = sum([7 + 7 * pop_size * dimension,
                              pop_size * obj_func_flops,
                              n_iter * (12 * pop_size * dimension + pop_size * obj_func_flops)])
            return flop_count
        else:
            raise NotImplementedError(f'Algorithm {self.algorithm} not implemented yet for performance plot.')


def calc_op_intensity(run_config: Dict) -> float:
    """Given a run configuration, what is the operational intensity [FLOPS / BYTES]."""
    mem_move_bytes = MemoryMoveCounter(run_config).mem_movement_bytes()
    total_flop_cont = FlopCounter(run_config).flop_count()
    return total_flop_cont / mem_move_bytes


def performance_metrics(output_parser: OutputParser) -> Dict:
    """For all sub runs loaded by an OutputParser, calculate the
    operational intensity, flop counts and memory movement.

    Returns
    -------
        results: a dict with run_name keys and dict value which holds the config and performance metrics fields
    """
    timings = output_parser.parse_timings(return_lists=True)
    results = {}
    for run_name, sub_config in output_parser.sub_configs.items():

        flop_count = FlopCounter(sub_config).flop_count()
        results[run_name] = {'config': sub_config,
                             'op_intensity': calc_op_intensity(sub_config),
                             'flop_count': flop_count,
                             'mem_move_bytes': MemoryMoveCounter(sub_config).mem_movement_bytes(),
                             'mem_move_floats': MemoryMoveCounter(sub_config).mem_movement_floats(),
                             'performance': flop_count / np.mean(timings[run_name])}
    print(f'Calculated performance metrics for {len(output_parser.sub_configs)} run(s).')
    return results
