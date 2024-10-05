# Directory Migrator

🌍 **[简体中文](README-CN.md) | [English](README.md)**

The Directory Migrator is a practical tool for conveniently and quickly migrating data files.

## Usage

1. Save `DirectoryMigrator.exe` and `RegisterOrUnregister.bat` to a directory of your choice.

2. Right-click on `RegisterOrUnregister.bat` and select "**Run as administrator**".

3. Save the list of directories that need to be migrated as `.ini` file and rename the extension to `.mil`. The following demonstrate the file format for migrating SSH and VSCode data.

```
[SSH]
MappingDirectory=<UserProfile>\.ssh
StorageDirectory=SSH
Operation=Move

[VSCodeExtensions]
MappingDirectory=<UserProfile>\.vscode
StorageDirectory=VSCode\ExtensionsDir
Operation=Map

[VSCodeUserData]
MappingDirectory=<AppData>\Code
StorageDirectory=VSCode\UserDataDir
Operation=Map
```

4. Double-click on your `.mil` file and enjoy! If the directories you are migrating require elevated permissions, such as `%ProgramFiles%`, you will need to right-click and select "**Run as administrator**" to ensure the necessary access privileges.

## Profile Format

The profile file follows the INI format.

### MappingDirectory and StorageDirectory

Use angle brackets `<>` in the path to access environment variables. The following two paths are equivalent.

```
<ProgramFiles(x86)>\Common Files\Adobe
C:\Program Files (x86)\Common Files\Adobe
```

### Operation

- **Move**: Migrates the `MappingDirectory` to the `StorageDirectory` and links it, then changes the `Operation` to `Map` after the first run.
- **Map**: Links the `MappingDirectory` to the `StorageDirectory`, and clears the `MappingDirectory` if it is not empty.
- **Erase**: Links the `MappingDirectory` to the `StorageDirectory` and clears both the `MappingDirectory` and `StorageDirectory`.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE.md) file for details.
