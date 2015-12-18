@echo off

echo %cd%

for /f "%%cd" %%i in(*.o) do (echo %%i)

pause
goto :eof

