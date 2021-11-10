@echo off

ctime -begin timing.ctm

set opts=-FC -EHa- -nologo -Zi -DASSERTIONS_ENABLED=1 -DUNIFORM_ASSERTIONS_ENABLED=1 /EHsc -DVISION_ENABLED

set external_code_paths=/I C:\Library\glfw-3.3.4\include /I C:\Library\glfw-3.3.4\src
set common_libs=kernel32.lib gdi32.lib user32.lib opengl32.lib
set renderer_libs=/LIBPATH:..\lib glew32.lib /LIBPATH:C:\Library\glfw-3.3.4\build\src\Debug glfw3dll.exp glfw3dll.lib

set code=%cd%

pushd build
cl %opts% %external_code_paths% %code%\glfw_main.cpp -Ferenderer %common_libs% /link %renderer_libs%
popd

ctime -end timing.ctm %last_error%

