#!/usr/bin/env python3
import os
import argparse
import shutil

from common import DATA_DIR_PATH


def parse_args():
    parser = argparse.ArgumentParser(description='Wipes the data directory such that you can start clean again.')

    return parser.parse_args()


def main():
    sub_folders = [f.path for f in os.scandir(DATA_DIR_PATH) if f.is_dir()]

    if len(sub_folders) == 0:
        print('\nDude, there is no output data. Run some and come back.\n')
        exit(0)

    print(f'\nThis will clean the following {len(sub_folders)} output directories...\n')
    for folder in sub_folders:
        print(f'\t{folder}')

    answer = input('\nDo you REALLY want to wipe all folders in the data directory? [y/n]\n')

    if answer == 'y':
        for folder in sub_folders:
            shutil.rmtree(folder)
        print('\nDeleted.\n')
    else:
        print('\nPhu... that was close. Nothing will happen to your precious data, mate.\n')


if __name__ == '__main__':

    main()