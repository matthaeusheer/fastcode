import unittest

from fastpy.evaluation.performance_calculations import FlopCounter


class TestFlopCount(unittest.TestCase):

    def test_hgwosca_flop_count(self):
        run_config = {
            "algorithm": "hgwosca",
            "obj_func": "rastigrin",
            "dimension": 10,
            "n_iter": 3,
            "n_rep": 1,
            "population": 100,
            "min_val": -10,
            "max_val": 10
        }
        flop_counter = FlopCounter(run_config)
        expected = 4 * 100 * 10 + 100 * 7 * 10 + 31 * 100 * 3 * 10 + 3 * 100 * 7 * 10
        self.assertEqual(flop_counter.flop_count(), expected)

    def test_squirrel_flop_count(self):
        run_config = {
            "algorithm": "squirrel",
            "obj_func": "rastigrin",
            "dimension": 10,
            "n_iter": 3,
            "n_rep": 1,
            "population": 100,
            "min_val": -10,
            "max_val": 10
        }
        flop_counter = FlopCounter(run_config)
        expected = 52654
        self.assertEqual(flop_counter.flop_count(), expected)
