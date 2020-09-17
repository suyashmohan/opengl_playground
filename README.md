# Opengl Playground
Learning OpenGL for no apparent reason 

# Dependencies

* https://www.glfw.org/
* https://glad.dav1d.de/
* https://github.com/nothings/stb
* https://github.com/recp/cglm

# How to run

```
vcpkg install glfw3
vcpkg install glad
vcpkg install stb
vcpkg install cgflm [PR](https://github.com/microsoft/vcpkg/pull/13510)
mkdir build
cd build
VCPKG_ROOT=/path/to/vcpkg cmake ..
cmake --build .
cd ..
./build/main
```