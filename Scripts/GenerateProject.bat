@echo off
pushd %~dp0\..\
call Vendor\premake\premake5.exe vs2022
popd
PAUSE