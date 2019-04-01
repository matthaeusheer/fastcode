import os


def load_input_size_benchmark_output(dir_name, file_name):

    file_path = os.path.join(dir_name, file_name)

    with open(file_path, 'r') as infile:
        return infile.readlines()
