@echo off
echo 'adding the submodules to the project ' 

REM this variable give the path to the current batch file
cd %~dp0

REM going 1 directory to later download the repos for the shaders 
cd .. 

echo 'downloadin glsl repo'
git clone https://github.com/y1z/GlShaders

echo 'downloadin hlsl repo'
git clone https://github.com/y1z/DxShaders

SET currentPath=%CD% 

cd  %~dp0
echo 'finished press whatever key you feel like pressing'
pause 
