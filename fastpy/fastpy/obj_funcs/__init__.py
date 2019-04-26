from scipy import optimize


def rosenbrock(input_vec):
    """SciPy implementation of n dimensional rosenbrock function."""
    return optimize.rosen(input_vec)
