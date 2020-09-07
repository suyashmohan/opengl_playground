# Opengl Playground
Learning OpenGL for no apparent reason 

# Dependencies

* https://www.glfw.org/
* https://glad.dav1d.de/
* https://github.com/nothings/stb
* https://github.com/datenwolf/linmath.h

# How to run

```
vcpkg install glfw3
vcpkg install glad
vcpkg install stb
mkdir build
cd build
VCPKG_ROOT=/path/to/vcpkg cmake ..
cmake --build .
cd ..
./build/main
```