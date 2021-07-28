@echo off

ctime -begin timing.ctm

set opts=-FC -GR- -EHa- -nologo -Zi -DASSERTIONS_ENABLED=1 /std:c++17
set code=%cd%
pushd build
cl %opts% %code%\glfw_main.cpp -Ferenderer kernel32.lib gdi32.lib user32.lib opengl32.lib  /link /LIBPATH:..\lib glfw3dll.lib glfw3dll.exp glew32.lib
REM cl %opts% /EHsc %code%\render_test.cpp -Ferenderer_test
REM cl %opts% /EHsc %code%\camera_test.cpp -Fecamera_test
popd

ctime -end timing.ctm %last_error%
