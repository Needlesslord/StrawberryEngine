# StrawberryEngine: 3D Engine
Developing a 3D Engine for the subject 3D Engines taught in CITM-UPC by Marc Garrigó, developed by Tomás Carreras and Núria Lamonja




## The Engine
_Short description of the engine and links._

This is Strawberry Engine's base. It includes basic features such as:

- Model Import w/ Drag & Drop
- Texture Import w/ Drag & Drop
- GameObject hierarchy + Component Pattern
- Unity-like camera controls
- Editor tools (main menu and windows)

It is developed under the MIT License.

- _To go to the Engine's **RELEASES** click [here](https://github.com/Needlesslord/StrawberryEngine/releases)._

- _To go to the **GITHUB REPOSITORY** click [here](https://github.com/Needlesslord/StrawberryEngine)._

- _To go to the **WEB PAGE** click [here](https://needlesslord.github.io/StrawberryEngine/)._




## The Team
_List of team members and GitHub and Linkedin accounts._

This project is developed by:

- Tomás Carreras 
	+ [Linkedin](https://www.linkedin.com/in/tom%C3%A1s-carreras-a96a99177/) 
	+ [GitHub](https://github.com/tomascarreras1000)
- Núria Lamonja 
	+ [Linkedin](https://www.linkedin.com/in/needlesslord/) 
	+ [GitHub](https://github.com/Needlesslord)




## How **STRAWBERRY ENGINE** works
_How to use the engine, detailing the controls and any specific action._

- Camera: Right-click + WASD to move the camera in the X and Y axis, right-click also allows for free camera look, mouse wheel to zoom in and out, Atl + left-click orbits the selected mesh, the "F" key centers the camera's focus to the selecte mesh and SHIFT doubles the camera speed.
- Hierarchy window: The hierarchy window displays all game objects in the scene with their respective meshes, which can be selected to display its information on the inspector window.
- Inspector window: The inspector window, although open at the start of the engine, doesn't display any information unless there's at least one mesh selected, in which case it displays some basic information about the mesh and the textures.
- Console window : The console window displays very basic information for now and it does not accept input.
- Menu bar: The menu bar has 4 menus:
	- File: only has an item to exit.
	- View: offers the option to hide/show all the main windows.
	- Help: list of links to our web page, to report bugs and to open the about window with information about the software and the hardware.
	- Basic Shapes: offers the option to import 4 basic shapes (cube, cylinder, sphere and cone).
- Configuration window: The configuration window gives information about the application and hardware and has several options for render, textures and window preferences.


### Included files
_Files you will find in the .zip._

- *StrawberryEngine (main folder)*
	- Assets/Resources
		+ Baker_house.FBX
	- .dll files
	- StrawberryEngine.exe
- *README.md* (StrawberryEngine's key information)
- *LICENCE.md* (MIT License, which the project is developed under)
- *Licences* (folder with all third party libraries’ licences)




### Basic functionalities
_Base funcionalities in the engine for it to work properly._

- It allows to drag and drop FBX files from anywhere in the Hard Drive.
- It allows to drag and drop DDS/PNG files from anywhere in the Hard Drive (And it applies the texture to the selected GameObject).
- Game Object structure for each loaded mesh: available 3 display-only components: Transform, Mesh and Texture. 
- Unity-like camera controls:
	+ While Right clicking, “WASD” fps-like movement and free look around is enabled.
	+ Mouse wheel zooms in and out.
	+ Alt+Left click orbit around the object.
	+ Pressing “f” focuses the camera around the geometry.
	+ Holding SHIFT duplicates movement speed.
- Editor Windows:
	+ Console: the window LOGs the geometry loading process from ASSIMP and the external libraries initialization process.
	+ Configuration:
		- A graph for the frames per second.
		- Configuration for all variables on each module (renderer, window, input, textures).
		- Information output of memory consumption, hardware detection and software versions (SDL, OpenGL, DevIL).
	+ Hierarchy: displays a list with all GameObjects in this window. The user is able to select a GameObject through this window.
	+ Inspector: displays information about the components of the selected GameObject
		- Transform: only for display purposes. Show position, rotation and scale for the selected GameObject.
		- Mesh: information about the loaded mesh. There is an option to display its normals (per-triangle and per-face).
		- Texture: displays texture size and path. There is an option to view the selected GameObject with the checkers texture.
	+ There is a general menu with an option to quit, visit the github page and get info about the engine (“About”). It is also able to turn on/off editor windows.
	+ There is a menu to load your own basic shapes.




### Extra functionalities (innovation)
_Additional funcionalities in the engine._

You can dynamically change the texture of any meshes you select, including the default "checkers" texture (will work with ImGui::Combo() in the future)




## Take into account...
_Additional comments to understand some parts of the engine._




## Credits 

- 3D Models: [free3d.com](https://free3d.com/3d-models/fbx-textures)




# [LATEST RELEASE](https://github.com/Needlesslord/StrawberryEngine/releases/tag/v0.5)




