@echo off

set ResultFileName=SimpleHydrologyByNickMcDonald

FOR %%f IN ("%CD%\Result_*.tiff") DO (

FOR /f "tokens=2 delims=_" %%g IN ("%%~nf") DO (

echo %%f : %%g
gm.exe convert "%%f" "..\(Documentation)\%ResultFileName%_grayscale_%%g.png"

)
)

FOR %%f IN ("%CD%\Colors_*.tiff") DO (

FOR /f "tokens=2 delims=_" %%g IN ("%%~nf") DO (

echo %%f : %%g
gm.exe convert "%%f" "..\(Documentation)\%ResultFileName%_colors_%%g.png"

)
)

cd..
cd..
cd..

doxygen.exe

pause
