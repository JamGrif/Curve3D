:: Used to refresh res and config files in the final folder. Ran everytime Curve3DLauncher is launched (only when developing)
:: Automatically ran by Curve3DLauncher when launched through Visual Studio and will not be ran when on users machines
:: DO NOT MOVE from solution directory

@echo off

:: Ensure this bat file is ran in the correct place
if not exist res ( exit )
if not exist config ( exit )

:: Copy over any UPDATED ONLY files from res and config file from project to final
xcopy /s /y /d ..\..\..\Curve3D\res res
xcopy /s /y /d ..\..\..\Curve3D\config config