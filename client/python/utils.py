import time


def debug_lists(*args):
    for l in args:
        r = list(map(lambda x: round(x, 2), l))
        print(r)
    print()


def millis():
    return int(round(time.time() * 1000))


def clamp(n, minimum, maximum):
    """ Constrain value between `minimum` and `maximum`. """
    return max(min(maximum, n), minimum)


def reset_in_range(value, other_value, start, end):
    if start < value < end:
        return other_value
    return value


def timer(dt, f):
    """
    Calls function `f` every `dt` milliseconds.

    Args:
        dt: Time in Milliseconds
        f: Function to be called.
    """

    now = millis()
    past = now
    while True:
        now = millis()
        if now > past + dt:
            past = now
            f()
