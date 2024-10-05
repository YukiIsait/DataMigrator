# 数据迁移工具

🌍 **[English](README.md) | [简体中文](README-CN.md)**

一款方便快捷的数据迁移工具。

## 使用方法

1. 将 `DataMigrator.exe` 和 `RegisterOrUnregister.bat` 保存到你想要的目录中。

2. 右键点击 `RegisterOrUnregister.bat` 并选择“**以管理员身份运行**”。

3. 将需要迁移的目录列表保存为 `.ini` 文件，并将扩展名重命名为 `.mil`。以下示例展示了迁移 SSH 和 VSCode 数据的文件格式。

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

4. 双击 `.mil` 文件以迁移。如果你迁移的目录需要提升权限，例如 `%ProgramFiles%`，则需要右键并选择“**以管理员身份运行**”以确保必要的访问权限。

## 配置格式

配置文件遵循 INI 格式。

### MappingDirectory 和 StorageDirectory

在路径中使用尖括号 `<>` 来访问环境变量。以下两个路径是等效的。

```
<ProgramFiles(x86)>\Common Files\Adobe
C:\Program Files (x86)\Common Files\Adobe
```

### Operation

- **Move**: 将 **MappingDirectory** 迁移到 **StorageDirectory** 并链接，然后在首次运行后将 **Operation** 更改为 **Map**。
- **Map**: 将 **MappingDirectory** 链接到 **StorageDirectory**，如果 **MappingDirectory** 非空则清空。
- **Erase**: 将 **MappingDirectory** 链接到 **StorageDirectory**，并清空 **MappingDirectory** 和 **StorageDirectory**。

## 许可证

此项目根据 MIT 许可证授权，详见 [LICENSE](LICENSE.md) 文件。
