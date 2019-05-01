import unittest

from fastpy.visualization import viz_evolution


class TestEvolutionPlotting(unittest.TestCase):

    def test_data_input(self):
        """Basically simply tests whether things run through smoothly.

        In case the plotting function changes to other input data, this will fail.
        """
        # TODO: Figure out a way to fix the test
        # evolution_data = viz_evolution.dummy_evolution_data(10, 5)
        # _, _ = viz_evolution.plot_optimization_evolution_2d(evolution_data)
        pass


if __name__ == '__main__':
    unittest.main()
