"""Module for interacting with the emulated machine's registers."""


def read_gpr(index: int, /) -> int:
    """
    Returns the value contained in general purpose register at index.
    
    :param index: index of the gpr to read from (0-31)
    :return: value as integer
    """


def read_fpr(index: int, /) -> float:
    """
    Returns the value contained in floating point register at index.
    
    :param index: index of the fpr to read from (0-31)
    :return: value as float
    """


def write_gpr(index: int, value: int, /) -> None:
    """
    Writes value to general purpose register at index.
    
    :param index: index of the gpr to write to (0-31)
    :param value: the value to write
    """


def write_fpr(index: int, value: float, /) -> None:
    """
    Writes value to floating point register at index.
    
    :param index: index of the fpr to write to (0-31)
    :param value: the value to write
    """