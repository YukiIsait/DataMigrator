@echo off

:: Check if the registry key exists
reg query "HKEY_CLASSES_ROOT\.mil" >nul 2>&1
if %errorlevel%==0 (
    echo Unregistering...
    :: Unregister the entries
    reg delete "HKEY_CLASSES_ROOT\.mil" /f
    reg delete "HKEY_CLASSES_ROOT\DataMigrator.mil" /f
) else (
    echo Registering...
    :: Register the entries
    reg add "HKEY_CLASSES_ROOT\.mil" /ve /d "DataMigrator.mil" /f
    reg add "HKEY_CLASSES_ROOT\DataMigrator.mil" /ve /d "Migration Item List" /f
    reg add "HKEY_CLASSES_ROOT\DataMigrator.mil\DefaultIcon" /ve /d "%~dp0DataMigrator.exe" /f
    reg add "HKEY_CLASSES_ROOT\DataMigrator.mil\Shell\Open\Command" /ve /d "\"%~dp0DataMigrator.exe\" \"%%L\"" /f
    reg add "HKEY_CLASSES_ROOT\DataMigrator.mil\Shell\RunAs\Command" /ve /d "\"%~dp0DataMigrator.exe\" \"%%L\"" /f
)

pause
