@echo off
echo 'update the submodules' 
cd %~dp0
git submodule update --init 
git submodule update 
SET currentPath=%CD% 
cd ..
cd GlShaders
git fetch origin 
cd .. 
cd DxShaders
git fetch origin 
cd  %~dp0
echo 'finished'
pause 
