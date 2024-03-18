# MapleEzorsia

This DLL modifies the memory of v83 MapleStory clients to change the experience of the game.

[Screen Shots](/readme-screenshots/README.md)

### Libraries
- [Detours](https://github.com/microsoft/Detours) - Intercepts windows api and in-game function calls
- [discord-rpc](https://github.com/discord/discord-rpc) - For discord rich presence interaction

### Features
- Modify game window and canvas resolution
- Center the game window on start-up
- Position of some in-game components are modified to scale with any resolution
- Enable the minimize button
- Skip the start-up animations
- Hide login screen book border automatically on higher resolutions
- Increase chat log history from 64 to 127
- Fix fear effect (LimitedView fieldType) for any resolution

### How To Use
In your v83 MapleStory directory,  
1. Rename file `nmconew.dll` to `nmconew2.dll`  
2. Insert [this DLL file](https://github.com/izarooni/MapleEzorsia/releases/latest) in that directory  
3. Run your client!  

### Configure
The default `config.ini` file isn't included because it's optional.  
The default resolution is 1024x768 if no config is present.  

Make a file called `config.ini` in your game folder and the program will try to use it.  
All settings are optional and can be ommitted (in case you don't want players knowing/using it).  
```
[general]
width=1280
height=720
discord=false
debug=false
```

- `discord`=`true/false` Enables to disables discord presence
- `debug`=`true/false` Enables or disables the output console

## Developing

**This code can only be compiled on \[Release\] \[x86\] configuration**

### Discord Presence
1. Create a [Discord Application](https://discord.com/developers/applications)
2. `Discord.cpp`: Copy your `General Information > Application ID` value to this variable  
```cpp
LPCSTR Discord::m_sApplicationID = "";
```
3. `Discord.cpp`: Edit the following variables to your liking. Upload images to `Rich Presence > Art Assets`. Use the name of those images for your imageKey and imageText values.
```cpp
rp.largeImageKey = "";
rp.largeImageText = "";
rp.smallImageKey = "";
rp.smallImageText = "";
rp.details = "";
rp.state = "";
```