import time


def debug_lists(*args):
    for l in args:
        r = list(map(lambda x: round(x, 2), l))
        print(r)
    print()


def timer(dt, f):
    """
    Calls function `f` every `dt` milliseconds.

    Args:
        dt: Time in Milliseconds
        f: Function to be called.
    """

    now = int(round(time.time() * 1000))
    past = now
    while True:
        now = int(round(time.time() * 1000))
        if now > past + dt:
            past = now
            f()
