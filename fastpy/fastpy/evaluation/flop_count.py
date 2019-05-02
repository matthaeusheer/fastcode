from typing import Dict


class FlopCounter:
    def __init__(self, run_config: Dict) -> None:
        """Calculates the flop count for a particular execution of an optimisation algorithm with a given set
        of parameters specified in the run_config (obj_func, iterations, dimensions, ...).

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

        else:
            raise NotImplementedError(f'Algorithm {self.algorithm} not implemented yet for performance plot.')
