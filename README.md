# Project DirectX/OpenGl framework 
This repository will house a graphics framework that can chose between 2 api's open_gl and directX
## Tools used 
- visual sudio 2017 
## Language 
- c++ 17 
## External libs 
- ASSIMP: https://github.com/assimp/assimp
    - include in the project  
- ImGui : https://github.com/ocornut/imgui
    - include in the project 
- DirectXTK :  https://github.com/Microsoft/DirectXTK
    - include in the project 
- GLFW : https://www.glfw.org/
    - include in the project  
- GLM : https://github.com/g-truc/glm
    - NOT include in the project 
    - path is __`$(DEVLIBS)glm`__ 
- GLEW : http://glew.sourceforge.net/ 
	- included in the project 
## Environment variable
 - DEVLIBS : `C:\c++libs\`
    - aka it in the same place where i keep my libs 
## submodule 
	- This project contains 2 submodules 
		- DxShaders : https://github.com/y1z/DxShaders
		- GlShaders : https://github.com/y1z/GlShaders   
## clone process 
just clone the project then run the `updateSubmodule.bat` located in scripts
