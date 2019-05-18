from typing import Dict


class PerfCounter:
    def __init__(self, run_config: Dict) -> None:
        """Base class for FlopCounter and MemmoryMoveCounter.

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


def calc_op_intensity(run_config: Dict) -> float:
    """Given a run configuration, what is the operational intensity (FLOPS / BYTES)."""
    mem_move_bytes = MemoryMoveCounter(run_config).mem_movement_bytes()
    total_flop_cont = FlopCounter(run_config).flop_count()

    return total_flop_cont / mem_move_bytes


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
            pass
        else:
            raise NotImplementedError(f'Algorithm {self.algorithm} not implemented yet for performance plot.')
