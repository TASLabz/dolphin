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
        - "BreakOnRead": bool - whether the bp should trigger on memory read
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
