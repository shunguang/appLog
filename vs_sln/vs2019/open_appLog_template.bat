@echo off

REM define environment variables, do not change the variable name, just it's value!
set APPLOG_HOME=C:\Users\wus1\Projects\swu-personal
set APPLOG_BUILD=C:\Users\wus1\Projects\swu-personal\appLog\build

"C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\Common7\IDE\devenv.exe" %APPLOG_HOME%\appLog\vs_sln\vs2019\appLog.sln

REM ---eof---
