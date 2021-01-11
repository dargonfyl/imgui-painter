# IMGUI Painter
A simple imgui painter, made for edutainment purposes! The goal of this project is to learn about OpenGL, gain hands-on software development experience, and to figure out how Photoshop/Clip Studio Paint/Paint.NET/Krita/other painting tools do their thing.

This project's development progress is sometimes inconsistent, and its user interface in its current state is lackluster. There is no todo list, but a user interface similar to MS Paint and some basic brushes are the current goal. 

## Requirements
You will need:
- OpenGL version 3.3+ capable system
- CMake 3.0+ installed

## Building
Run the following:
```
$ git clone --recursive https://github.com/dargonfyl/imgui-painter.git
$ cd imgui-painter
$ mkdir build
$ cd build
$ cmake .. [-G your-target-generator]
$ cmake --build .
```
Depending on your generator, you may also build from your IDE.

## License
Licensed under The Unlicense, that is, you're free to do whatever you want with this code, but there is no warranty/liability.
