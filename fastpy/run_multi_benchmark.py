#!/usr/bin/env python3
import os
import argparse

from fastpy.run.Runner import MultiBenchmarkRunner


def parse_args():
    parser = argparse.ArgumentParser(description='Runs benchmark script on all possible '
                                                 'parameter combinations in a given config_template.json'
                                                 'file for all releases in the ')
    parser.add_argument('-c', '--config', default='config.json', help='config file name in fastpy root folder, '
                                                                      'defaults config.son')
    parser.add_argument('-f', '--force-compile', default=False, help='whether to force recompilation of executables'
                                                                     'for all releases')

    return parser.parse_args()


def main(args):

    assert os.path.exists(args.config), 'Config file {} does not exist. ' \
                                        'Abort. See README.md.'.format(args.config)
    runner = MultiBenchmarkRunner(args.config)
    runner.perform_runs(args.force_compile)


if __name__ == '__main__':

    main(parse_args())
