# AC_Cheat

This is a basic cheat trainer for **Assault Cube**, built using C++ and the Windows API. It allows you to manipulate in-game variables such as health and ammo by accessing and modifying the game's memory in real-time. This program uses dynamic pointers and predefined offsets to locate specific memory addresses and write new values.

## Features

- Modify **Health** value in-game.
- Change **Ammo** value in-game.

## Code Overview

- **GetProcessID**: Retrieves the Process ID (PID) of `ac_client.exe` by taking a snapshot of all running processes and matching the name.
- **OpenProcess**: Opens the process for reading and writing memory.
- **ReadProcessMemory**: Reads the base pointer address to locate specific variables in memory.
- **WriteProcessMemory**: Writes new values (for health, ammo, etc.) to the corresponding memory address.

## Legal Disclaimer

This code is provided for **educational purposes only**. Altering the memory of running processes may cause crashes, instability, or violate game policies. Using this software in multiplayer games may result in account bans or other penalties. Use at your own risk.