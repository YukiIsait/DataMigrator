# Directory Migrator

The Directory Migrator is a practical tool for conveniently and quickly migrating data files.

## Usage

1. Save `DirectoryMigrator.exe` and `RegisterToContextMenu.bat` to a directory of your choice.

2. Right-click on `RegisterToContextMenu.bat` and select `Run as administrator`.

3. Save the list of directories that need to be migrated as a text file and rename the extension to `.mil`. To demonstrate the file format for migrating Adobe files.

```
:Destinations              | Sources                                | Keeping Rules
.\AppData\Roaming          | <AppData>\Adobe                        | KeepSource
.\AppData\Local            | <LocalAppData>\Adobe                   | KeepSource
.\ProgramData              | <ProgramData>\Adobe                    | KeepSource
.\ProgramFiles\CommonFiles | <ProgramFiles(x86)>\Common Files\Adobe | KeepSource
.\ProgramFiles\x64         | <ProgramFiles>\Adobe                   | KeepSource
```

4. Double-click on your `.mil` file and enjoy! If the directories you are migrating require elevated permissions, such as `%ProgramFiles%`, you will need to right-click and select `Run as administrator` to ensure the necessary access privileges.

## Format

## Comments

Use the colon `:` at the beginning of a line to add comments.

```
: This line is commented out.
```

### Destinations and Sources

Use angle brackets `<>` in the path to access environment variables. The following two paths are equivalent.

```
<ProgramFiles(x86)>\Common Files\Adobe
C:\Program Files (x86)\Common Files\Adobe
```

### Keeping Rules

- **KeepDestination**: Deletes the source directory to the recycle bin and replaces it with a link pointing to the target directory.
- **KeepSource**: Moves the source to the target directory and creates a link at the original source path, pointing to the target directory.
- **KeepNothing**: Deletes the content of both the source and target directories. Then, creates a link at the original source path, pointing to the already empty target directory.

### Delimiters

The destination path, source path, and keeping rules are separated by the pipe `|` symbol.

```
<SystemDrive>\Programs\Postman | <LocalAppData>\Postman | KeepDestination
```
