# PushbackFlorian

`Pushback Florian` allows for additional information regarding pushback and ground handling to be tracked in EuroScope.

This plugin stores and synchronises additional ground and pushback states of aircraft between all controllers, allowing for more fine-grained clearances and ensuring all users with the plugin loaded receive all information available.  
`Pushback Florian` does not modify flightplans in a (user) noticable way, allowing for it to be utilised in combination with controllers not using the plugin.

## Table of Contents

- [Getting started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Usage](#usage)
  - [Basics](#basics)
  - [Tag items](#tag-items)
  - [Tag functions](#tag-functions)
  - [Chat commands](#chat-commands)
- [Contributing](#contributing)
  - [Development setup](#development-setup)
- [License](#license)

## Getting started

### Prerequisites

Since `Pushback Florian` was developed as an EuroScope plugin, it requires a working installation [EuroScope](https://euroscope.hu/). The initial development was started using EuroScope version [`v3.2.1.26`](https://www.euroscope.hu/wp/2021/02/07/v3-2-1-26/), although the plugin should most likely also work fine with previous and later versions. As development continues, compatibility to the latest **beta** versions of EuroScope will be maintained as long as possible and feasible.

### Installation

1. Download the latest release (`PushbackFlorian.zip`) of `Pushback Florian` from the [**Releases**](https://github.com/MorpheusXAUT/PushbackFlorian/releases/latest) section of this repository
2. Extract `PushbackFlorian.dll` and place it somewhere as desired (most likely somewhere inside your EuroScope sectorfile/profile setup, where other plugins are already set up)
3. Start EuroScope and open the **Plug-ins** dialog in the settings menu (**OTHER SET**)
![Plug-ins dialog](https://i.imgur.com/SrVtRp9.png)
4. **Load** the plugin by selecting the `PushbackFlorian.dll` you extracted and ensure the proper version is displayed
![Load plugin](https://i.imgur.com/EXSEfLl.png)
`Pushback Florian` will also confirm successful initialisation by logging its version to the **Messages** chat:
`[08:34:10] Pushback Florian: Version 1.0.0 loaded.`
5. Close the plugin dialog and open the departure list columns setup dialog (small **S** at the left side of your departure list)
![Departure list columns setup dialog](https://i.imgur.com/VrbgB8H.png)
6. Add the **Direction** column to your departure list by clicking **Add Item** and selecting the `Pushback Florian / Direction` **Tag Item type**. Pick a **Header name** and set a **Width** of 3 or greater
7. Add the **Line** column to your departure list by clicking **Add Item** and selecting the `Pushback Florian / Line` **Tag Item type**. Pick a **Header name** and set a **Width** of 4 or greater
8. Assign the `Pushback Florian / Direction Menu` action as the **Left button** action of your **Direction** tag item. Triggering this function opens the direction menu, allowing you to pick the desired pushback (facing) direction
9. Assign the `Pushback Florian / Direction Clear` action as the **Right button** action of your **Direction** tag item. Triggering this function clears the pushback (facing) direction of an aircraft, resetting its direction state
10. Assign the `Pushback Florian / Line Menu` action as the **Left button** action of your **Line** tag item. Triggering this function opens the line menu, allowing you to pick the desired (taxiway) line for pushback
11. Assign the `Pushback Florian / Line Clear` action as the **Right button** action of your **Line** tag item. Triggering this function clears the assigned (taxiway) line for pushback of an aircraft, resetting its line state
12. Close the departure list settings by clicking **OK**

## Usage

### Basics

At the moment, `Pushback Florian` supports the following additional pushback states:
- Direction: allows for pushback direction ("facing") to be tracked and shared with other controllers.
- Line: allows for (taxiway) line assigned for pushback to be tracked and shared with other controllers.

If enabled (see [Toggle setting of pushback state](#toggle-setting-of-pushback-state)), `Pushback Florian` will automatically set the `PUSH` ground state for an aircraft once a direction or line has been assigned. Note that the ground state will only be updated if the aircraft currently has no state assigned or has been cleared for startup (`ST-UP`).

### Tag items

Tag items are used to display information about flightplans in aircraft lists such as the departure list.  
At the moment, `Pushback Florian` adds two tag items to EuroScope:

#### Direction

The `Direction` tag item displays the selected pushback (facing) cardinal direction, abbreviated as a single letter.

#### Line

The `Line` tag item displays the selected (taxiway) line assigned for pushback as a colored "line" (represented as `===`).

### Tag functions

Tag functions are used to trigger plugin functionality via a flightplan tag in aircraft lists such as the departure list.  
At the moment, `Pushback Florian` adds several functions for assigning directions and lines which can be added as an action to any tag item desirable (although using them with the [`Direction`](#direction) and [`Line`](#line) tag items is recommended):

#### Direction Menu

Opens a selection menu containing all four pushback cardinal directions as well as an option to clear the current selection.  
This actions is suggested the default left-click action for the [`Direction`](#direction) tag item as it provides the most flexible direction management.

#### Direction Clear

Removes the currently selected pushback direction and clears its tracked state.  
This action is also available in the [`Direction Menu`](#direction-menu) tag function by default and allows for a shortcut to remove the direction state, making it the suggested right-click action for the [`Direction`](#direction) tag item.

#### Direction north

Sets the desired pushback direction as north (`N`).  
This action is also available in the [`Direction Menu`](#direction-menu) tag function by default.

#### Direction east

Sets the desired pushback direction as east (`E`).  
This action is also available in the [`Direction Menu`](#direction-menu) tag function by default.

#### Direction south

Sets the desired pushback direction as north (`S`).  
This action is also available in the [`Direction Menu`](#direction-menu) tag function by default.

#### Direction west

Sets the desired pushback direction as north (`W`).  
This action is also available in the [`Direction Menu`](#direction-menu) tag function by default.

#### Line Menu

Opens a selection menu containing all pre-defined (taxiway) lines assignable for pushback as well as an option to clear the current selection.  
This actions is suggested the default left-click action for the [`Line`](#line) tag item as it provides the most flexible line management.

#### Line Clear

Removes the currently selected pushback line and clears its tracked state.  
This action is also available in the [`Line Menu`](#line-menu) tag function by default and allows for a shortcut to remove the line state, making it the suggested right-click action for the [`Line`](#line) tag item.

#### Line blue

Assigns the blue line for pushback.  
This action is also available in the [`Line Menu`](#line-menu) tag function by default.

#### Line orange

Assigns the orange line for pushback.  
This action is also available in the [`Line Menu`](#line-menu) tag function by default.

### Chat commands

Chat commands allow more fine-grained control of `Pushback Florian`'s behavior and settings not available via UI elements. Every chat command is prefixed with `.pbf` and can be entered in every chat channel available. Executing `.pbf` without any additional commands prints the version loaded and a list of commands available.

#### Toggle debug logging

`.pbf debug`

Toggles debug logging, displaying more messages about the internal state.

This setting will be saved to the EuroScope settings upon exit.

#### Toggle setting of pushback state

`.pbf push`

Toggles automatic setting of pushback ground state.

If enabled (default), `Pushback Florian` will automatically set the `PUSH` ground state for an aircraft after a direction or line has been assigned.

This setting will be saved to the EuroScope settings upon exit.

#### Reset aircraft states

`.pbf reset`

Clears the plugin's list of tracked aircraft and their respective ground handling states.

## Contributing

If you have a suggestion for the project or encountered an error, please open an [issue](https://github.com/MorpheusXAUT/PushbackFlorian/issues) on GitHub. Please provide a summary of your idea or problem, optionally with some logs or screenshots and ways to replicate for the latter.  

[Pull requests](https://github.com/MorpheusXAUT/PushbackFlorian/pulls) are highly welcome, feel free to extend the plugin's functionality as you see fit and submit a request to this repository to make your changes available to everyone. Please keep in mind this plugin attempts to provide features in a relatively generic way so it can be used by vACCs with different needs - try refraining from "hard-coding" any features that might just apply to a specific airport or vACC.

### Development setup

`Pushback Florian` currently has no external development dependencies aside [Visual Studio](https://visualstudio.microsoft.com/vs/). Initial development started using Visual Studio 2019, although later versions should most likely remain compatible.

To allow for debugging, the project has been configured to launch EuroScope as its debug command. Since your installation path of EuroScope will most likely be different, you **must** set an environment variable `EUROSCOPE_ROOT` to the **directory** EuroScope is installed in (**not** the actual `EuroScope.exe` executable), for instance `E:\EuroScope`.  
Note: triggering a breakpoint seems to cause both EuroScope and Visual Studio to freak out, resulting in high resource usage and slugging mouse movements, thus only being of limited usefulnes. **NEVER** debug your EuroScope plugin using a live connection as halting EuroScope apparently messes with the VATSIM data feed under certain circumstances.

`Pushback Florian` is compiled using Windows SDK Version 10.0 with a platform toolset for Visual Studio 2019 (v142) using the ISO C++17 Standard.

This repository contains all external dependencies used by the project in their respective `include` and `lib` folders:

- `EuroScope`: EuroScope plugin library

## License

[MIT License](LICENSE)