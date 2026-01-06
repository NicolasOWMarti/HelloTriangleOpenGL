# HelloTriangleOpenGL
## my first hello triangle made using openGL. 

i'm still working on this project so not every thing is finished. 
i don't use windows or linux so i don't know how to make the code work on those operating systems

## what you need to run the code on mac
you need to install GLEW, GLFW and CMake.
to instal glew using HomeBrew type the following into the terminal:
```bash
brew install glew
```
same goes for GLFW and CMake:
```bash
brew install glfw
```
```bash
brew install cmake
```


## how to build and run the code on mac
of you want to edit the code in VSCode then you will have to go to `C/C++ configurations (UI) -> advanced settings -> compile commands` and add:
```bash
${workspaceFolder}/build/compile_commands.json
```

build command (make sure that the curent directory is the projectfolder and not build):

```bash
cd "${fileDirname}"                                                        
rm -rf build
mkdir build
cd build
cmake ..
make
```

run command (make sure that the curent directory is build and not the projectfolder):

```bash
./OpenGLApp  
```
