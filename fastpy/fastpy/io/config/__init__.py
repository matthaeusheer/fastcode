import os
import json


def load_json_config(config_path):
    with open(config_path, 'r') as infile:
        return json.load(infile)


def store_json_config(config, dir_path, file_name):
    with open(os.path.join(dir_path, file_name), 'w') as outfile:
        json.dump(config, outfile, indent=4)


