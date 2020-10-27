# StrawberryEngine: 3D Engine
Developing a 3D Engine for the subject 3D Engines taught in CITM-UPC by Marc Garrigó, developed by Tomás Carreras and Núria Lamonja




## The Engine
_Short description of the engine and links._

This is Strawberry Engine's base. It includes basic features such as:

- Model Import + Drag & Drop
- Texture Import
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
- It allows to drag and drop DDS/PNG files from anywhere in the Hard Drive (apply the texture to the selected GameObject).
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

*(under construction)*




## Take into account...
_Additional comments to understand some parts of the engine._



