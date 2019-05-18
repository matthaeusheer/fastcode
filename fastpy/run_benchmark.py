#!/usr/bin/env python3
import os
import argparse


from fastpy.run.Runner import BenchmarkRunner


def parse_args():
    parser = argparse.ArgumentParser(description='Runs benchmark script on all possible '
                                                 'parameter combinations in a given config_template.json'
                                                 'file.')
    parser.add_argument('-c', '--config', default='config.json', help='config file name in fastpy root folder, '
                                                                      'defaults config.son')
    parser.add_argument('-b', '--bin_dir', default='../build', help='name of the build/bin directory, e.g.: ../build')

    return parser.parse_args()


def main(args):

    assert os.path.exists(args.config), 'Config file {} does not exist. ' \
                                        'Abort. See README.md.'.format(args.config)
    runner = BenchmarkRunner(args.config, bin_dir=args.bin_dir)
    runner.run_benchmarks()


if __name__ == '__main__':

    main(parse_args())


