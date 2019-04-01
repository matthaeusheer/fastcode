#!/usr/bin/env python3
import os
import sys
import argparse

from pprint import pprint

from fastpy.run.Runner import BenchmarkRunner


def parse_args():
    parser = argparse.ArgumentParser(description='Runs benchmark script on all possible '
                                                 'parameter combinations in a given config_template.json'
                                                 'file.')

    parser.add_argument('-c', '--config', default='config.json', help='output file')

    return parser.parse_args()


def main(args):

    assert os.path.exists(args.config), f'Config file {args.config} does not exist. Abort. See README.md.'
    runner = BenchmarkRunner(args.config)
    runner.run_benchmarks()


if __name__ == '__main__':

    main(parse_args())


