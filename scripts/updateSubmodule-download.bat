@echo off
echo 'adding the submodules to the project ' 

REM this variable give the path to the current batch file
cd %~dp0

REM here are the names of the directory for the shaders.

SET glDir=GlShaders
SET dxDir=DxShaders

REM creating the directorys for the project

cd .. 
REM access the variable
mkdir %glDir%
cd %glDir%
echo 'downloadin glsl repo'
REM dowloadin the glsl shaders
git clone https://github.com/y1z/GlShaders

cd ..
mkdir %dxDir%
cd %dxDir%
echo 'downloadin hlsl repo'
git clone https://github.com/y1z/DxShaders

SET currentPath=%CD% 

cd  %~dp0
echo 'finished press whatever key you feel like pressing'
pause 
