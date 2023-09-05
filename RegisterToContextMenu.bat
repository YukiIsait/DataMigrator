@echo off

reg add "HKEY_CLASSES_ROOT\.mil" /ve /d "DirectoryMigrator.mil" /f
reg add "HKEY_CLASSES_ROOT\DirectoryMigrator.mil" /ve /d "Migration Item List" /f
reg add "HKEY_CLASSES_ROOT\DirectoryMigrator.mil\DefaultIcon" /ve /d "%~dp0DirectoryMigrator.exe" /f
reg add "HKEY_CLASSES_ROOT\DirectoryMigrator.mil\Shell\Open\Command" /ve /d "\"%~dp0DirectoryMigrator.exe\" \"%%L\"" /f
reg add "HKEY_CLASSES_ROOT\DirectoryMigrator.mil\Shell\RunAs\Command" /ve /d "\"%~dp0DirectoryMigrator.exe\" \"%%L\"" /f
pause
