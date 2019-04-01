import os

"""
Helper file to setup common structure in the repo such as default PATHS or other global variables which are 
used throughout the project.
"""

DATA_FOLDER_NAME = 'data'
PROJECT_ROOT_PATH = os.path.abspath(os.path.dirname(__file__))
DATA_DIR_PATH = os.path.join(PROJECT_ROOT_PATH, DATA_FOLDER_NAME)