import time


def debug_lists(*args):
    for l in args:
        r = list(map(lambda x: round(x, 2), l))
        print(r)
    print()


def millis():
    return int(round(time.time() * 1000))


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
