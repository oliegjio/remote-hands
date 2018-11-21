import math


def solve_3dof_kinematics(x, y, l1, l2, l3):
    """
    Solve inverse kinematics equations for 3 DOF planar arm.

    Args:
        x (float): Target X position.
        y (float): Target Y position.
        l1 (float): Length of the first limb.
        l2 (float): Length of the second limb.
        l3 (float): Length of the third limb.

    Returns:
        list: List of angles (in radians) for manipulator joins.

    Raises:
        ValueError: If could not solve kinematics equations for given input.
    """

    phi = math.atan2(y, x)
    xw = x - l3 * math.cos(phi)
    yw = y - l3 * math.sin(phi)
    r = xw ** 2 + yw ** 2
    beta = math.acos((l1 ** 2 + l2 ** 2 - r) / (2 * l1 * l2))
    gamma = math.acos((r + l1 ** 2 - l2 ** 2) / (2 * (r ** 0.5) * l1))
    alpha = math.atan2(yw, xw)

    r1 = alpha - gamma
    r2 = math.pi - beta
    r3 = phi - r1 - r2

    return [r1, r2, r3]