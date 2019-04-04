from datetime import datetime


def get_date_time_tag():
    """Returns a datetime tag in the following example format '2019-04-01_20-24-13-535342'."""
    curr_time = datetime.now()
    return curr_time.strftime('%Y-%m-%d_%H-%M-%S-%f')