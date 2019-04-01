import os
import sys

"""
This context file enables uf to to

    from context import fastpy

inside the notebooks in this notebooks directory without dealing with paths and stuff inside the notebook.
"""

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

import fastpy