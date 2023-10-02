"""Module for interacting with Dolphin's debugger."""

def set_breakpoint(addr: int, /) -> None:
    """
    Sets an instruction breakpoint at addr.
    
    :param addr:  address to set the breakpoint at
    """
    
    
def remove_breakpoint(addr: int, /) -> None:
    """
    Removes the breakpoint at addr, if present.
    
    :param addr: address of the breakpoint to remove
    """
    
    
def set_memory_breakpoint(params: dict, /) -> None:
    """
    Sets a memory breakpoint with
    parameters specified by params.
    
    :param params: must have one of the following set of keys:
        - "At": u32 - the address to set the memory breakpoint at
        - "Start": u32, "End": u32 - the range of addresses to breakpoint within
        Followed by any of the following optional breakpoints:
        - "BreakOnRoad": bool - whether the bp should trigger on memory read
        - "BreakOnWrite": bool - whether the bp should trigger on memory write
        - "LogOnHit": bool - whether Dolphin should log when the bp is hit
        - "BreakOnHit": bool - whether execution should pause when the bp is hit
        - "Condition": str - set a condition for the breakpoint to be hit.
                             GPRs can be referenced by r0-r31 and FPRs by f0-f31.
                             You can also use lr, ctr, and pc.
    
    """


def remove_memory_breakpoint(addr: int, /) -> None:
    """
    Remove the memory breakpoint at the specified addr.
    
    :param addr: address of the breakpoint to remove
    """


def read_u8(addr: int, /) -> int:
    """
    Reads 1 byte as an unsigned integer.

    :param addr: memory address to read from
    :return: value as integer
    """


def read_u16(addr: int, /) -> int:
    """
    Reads 2 bytes as an unsigned integer.

    :param addr: memory address to read from
    :return: value as integer
    """


def read_u32(addr: int, /) -> int:
    """
    Reads 4 bytes as an unsigned integer.

    :param addr: memory address to read from
    :return: value as integer
    """


def read_u64(addr: int, /) -> int:
    """
    Reads 8 bytes as an unsigned integer.

    :param addr: memory address to read from
    :return: value as integer
    """


def read_s8(addr: int, /) -> int:
    """
    Reads 1 byte as a signed integer.

    :param addr: memory address to read from
    :return: value as integer
    """


def read_s16(addr: int, /) -> int:
    """
    Reads 2 bytes as a signed integer.

    :param addr: memory address to read from
    :return: value as integer
    """


def read_s32(addr: int, /) -> int:
    """
    Reads 4 bytes as a signed integer.

    :param addr: memory address to read from
    :return: value as integer
    """


def read_s64(addr: int, /) -> int:
    """
    Reads 8 bytes as a signed integer.

    :param addr: memory address to read from
    :return: value as integer
    """


def read_f32(addr: int, /) -> float:
    """
    Reads 4 bytes as a floating point number.

    :param addr: memory address to read from
    :return: value as floating point number
    """


def read_f64(addr: int, /) -> float:
    """
    Reads 8 bytes as a floating point number.

    :param addr: memory address to read from
    :return: value as floating point number
    """


def read_bytes(addr: int, size: int, /) -> bytearray:
    """
    Reads size bytes and outputs a bytearray of length size.
    
    :param addr: memory address to start reading from
    :param size: number of bytes to read
    :return: bytearray containing the read bytes
    """


def invalidate_icache(addr: int, size: int, /) -> None:
    """
    Invalidates JIT cached code between the address and address + size,
    forcing the JIT to refetch instructions instead of executing from its cache.

    :param addr: memory address to start invalidation at
    :param size: size of the cache as integer
    """


def write_u8(addr: int, value: int, /) -> None:
    """
    Writes an unsigned integer to 1 byte.
    Overflowing values are truncated.

    :param addr: memory address to read from
    :param value: value as integer
    """


def write_u16(addr: int, value: int, /) -> None:
    """
    Writes an unsigned integer to 2 bytes.
    Overflowing values are truncated.

    :param addr: memory address to read from
    :param value: value as integer
    """


def write_u32(addr: int, value: int, /) -> None:
    """
    Writes an unsigned integer to 4 bytes.
    Overflowing values are truncated.

    :param addr: memory address to read from
    :param value: value as integer
    """


def write_u64(addr: int, value: int, /) -> None:
    """
    Writes an unsigned integer to 8 bytes.
    Overflowing values are truncated.

    :param addr: memory address to read from
    :param value: value as integer
    """


def write_s8(addr: int, value: int, /) -> None:
    """
    Writes a signed integer to 1 byte.
    Overflowing values are truncated.

    :param addr: memory address to read from
    :param value: value as integer
    """


def write_s16(addr: int, value: int, /) -> None:
    """
    Writes a signed integer to 2 bytes.
    Overflowing values are truncated.

    :param addr: memory address to read from
    :param value: value as integer
    """


def write_s32(addr: int, value: int, /) -> None:
    """
    Writes a signed integer to 4 bytes.
    Overflowing values are truncated.

    :param addr: memory address to read from
    :param value: value as integer
    """


def write_s64(addr: int, value: int, /) -> None:
    """
    Writes a signed integer to 8 bytes.
    Overflowing values are truncated.

    :param addr: memory address to read from
    :param value: value as integer
    """


def write_f32(addr: int, value: float, /) -> None:
    """
    Writes a floating point number to 4 bytes.
    Overflowing values are truncated.

    :param addr: memory address to read from
    :param value: value as floating point number
    """


def write_f64(addr: int, value: float, /) -> None:
    """
    Writes a floating point number to 8 bytes.
    Overflowing values are truncated.

    :param addr: memory address to read from
    :param value: value as floating point number
    """
    
    
def write_bytes(addr: int, bytes: bytearray, /) -> None:
    """
    Writes each byte from the provided bytearray,
    starting from addr.
    
    :param addr: memory address to start writing to
    :param bytes: bytearray of bytes to write
    """
