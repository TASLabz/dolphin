"""Module for various utilities."""

def get_script_dir() -> str:
    """
    Gets the path to the Scripts dir.

    :return: value as string
    """

def open_dir() -> str:
    """
    Promtps the user to open a file.

    :return: value as string
    """

def start_framedump() -> None:
    """Starts a framedump."""

def stop_framedump() -> None:
    """Stops a framedump."""

def is_framedumping() -> bool:
    """
    Gets if a framedump is occuring.

    :return: value as bool
    """

def start_audiodump() -> None:
    """Starts an audiodump."""

def stop_audiodump() -> None:
    """Stops an audiodump."""

def is_audiodumping() -> bool:
    """
    Gets if an audiodump is occuring.

    :return: value as bool
    """

def save_screenshot(filename: str | None = None) -> None:
    """Saves a screenshot of the running game."""
