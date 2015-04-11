# LightBikes3d
3-dimensional version of the classic game Lightbikes. The 3rd dimension is created by having many levels with ramps between them.

## Building on Windows
To build LightBikes3d on Windows, you will need:
* Some sort of Visual Studio, Express or Professional. Visual Studio Professional 2013 is known to work, but other versions and editions are likely to work too.
* [Microsoft DirectX Software Development Kit (June 2010)](http://www.microsoft.com/en-gb/download/details.aspx?id=6812)

To do the build:
* Open a Visual Studio x86 Native Tools Command Prompt.
* Change into the `src` directory of LightBikes3d.
* Run `Make-VC.bat`.

To run the game:
* Run `lb3d.exe` from the `bin` directory (double-click in Explorer works).

## Playing the game
The game currently uses text commands, which you enter simply by typing with the game focused. The text will appear at the bottom left of the screen. Anything not starting with `/` or `\` is sent as chat to all other connected players.

The game starts in the lobby, where you can set your handle (`/hand`) and either start a server (`/startserver`) or connect to an existing one (`/connect`). Once people are connected, you may exchange chat messages and the server may start the game (`/newgame`).

### Commands
* `/hand` sets your in-game handle.
* `/startserver` starts a server within the current copy of the program (you need to do this even to play locally).
* `/connect [host]` connects to a running server (defaults to local machine).
* `/leave` _very buggy_ but is meant to disconnect you from the server.
* `/quit` exits the game.

### Server Commands
* `/newgame` starts a new game, abandoning the current one if necessary.
* `/endgame` stops the current game and returns to lobby mode.
* `/newround` _not implemented_
* `/newmap` _not implemented_

### Other Inputs
* Hold `Tab` to display the player list and current scores when in game.
* `F1`, `F2`, `F3` and `F4` select following cameras with increasing distance from the player (`F3` is the default).
* `F6` and `F7` select side-on and top-down cameras.