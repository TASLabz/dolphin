# PyCore Scripting Interface Specifications

This document outlines how to begin writing your own Python scripts to interface with Dolphin.

## Folder Hierarchy
Scripts and any user-defined helper files are located within `$DOLPHIN_USER_FOLDER/Load/Scripts`. The current folder hierarchy is defined as follows:

```
/Load/Scripts
\__ Modules (Directory)
\__ GAMEID (Directory)
    \__ SCRIPTS GO HERE
```
`GAMEID` can be one or multiple directories which are named after a game's ID without a region (e.g. `RMC` for "Mario Kart Wii (All regions)").

`Modules` is automatically created by Dolphin and is the preferred location for any Python modules that game-specific scripts may import.

## Writing Scripts
Scripts can import various optional modules from dolphin, such as:

- `memory`: Allows for scripts to read from / write to memory
- `event`: Supports listening for various events such as frame advances and savestate saves/loads
- `gui`: Grants the ability to render text and polygons to the imgui game overlay (useful for RAM Watch displays)

For a full definition of modules, see [python-stubs](../python-stubs).

### Events
Events can be listened for in a few ways. Some events may provide arguments, such as whether a savestate load was via a file or a slot.

- Callback
```python
@event.on_savestatesave
def my_callback(fromSlot : bool, slotNumber : int):
    if (fromSlot):
        print(f"Loaded from slot {slotNumber}")
    else:
        print("Loaded from file")
```
- Async
```python
while True:
    (fromSlot, slotNumber) = await event.savestatesave()
    if (fromSlot):
        print(f"Loaded from slot {slotNumber}")
    else:
        print("Loaded from file")
```

## Running Scripts
The scripts panel can be accessed either by going to `View->Scripting` or clicking on the `Scripts` toolbar button. This will open the scripting widget on the left side of the Dolphin window. This widget will show a list of all `.py` files present within `$DOLPHIN_USER_FOLDER/Load/Scripts` and its child directories.

Upon loading a game, to hide scripts unrelated to the game you are playing, the widget will navigate to the `GAMEID` folder associated with the active game. If the associated `GAMEID` folder is not present, then the root Scripts directory will be shown. Closing a game will return you to the root Scripts directory.

Scripts can be toggled by clicking the checkbox next to the script name. Even if a script is checked, it will not run until a game is booted up, and it will stop running when a game is shut down.
