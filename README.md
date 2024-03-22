[![Build](https://github.com/falk-werner/pe-tools/actions/workflows/build.yml/badge.svg)](https://github.com/falk-werner/pe-tools/actions/workflows/build.yml)

# PE Tools

Tools to work with Microsoft Portable Executable (PE) files:

- pe-depends: list and copy dependencies of `.exe` or `.dll` files
- dll-finder: search for a `.dll` file
- pe-info: print information of `.exe` or `.dll` files

## pe-depends

List and / or copy dependencies of `.exe` or `.dll` files.

### Usage

```
pe-depends [--list | --direct | --copy] [-n] [-w <path>] [-t <dir>]
           [-k <dll>] [-p path] <exe_or_dll>...
```

### Commands

| Long Option | Short Option | Description |
| ----------- | ------------ | ----------- |
| `--list`    | `-l`         | list dependencies recursivly |
| `--direct`  | `-d`         | list direct dependencies only |
| `--copy`    | `-c`         | copy dependencies to target directory |

### Options

| Long Option    | Short Option | Argument | Description |
| -------------- | ------------ | -------- | ----------- |
| `--work-dir`   | `-w`         | PATH     | change working directory |
| `--target`     | `-t`         | PATH     | target directory for copy command (default: .) |
| `--known`      | `-k`         | FILE     | add known DLL (can be specified multiple times) |
| `--path`       | `-p`         | PATH     | add search Path (can be specified multiple times) |
| `--no-default` | `-n`         | -        | disable default search order (see below) |

## dll-finder

Search for a `.dll` file.

### Usage

```
dll-finder [-n] [-w <path>] [-t <dir>] [-k <dll>] [-p path] <dll>...
```

### Options

| Long Option    | Short Option | Argument | Description |
| -------------- | ------------ | -------- | ----------- |
| `--work-dir`   | `-w`         | PATH     | change working directory |
| `--known`      | `-k`         | FILE     | add known DLL (can be specified multiple times) |
| `--path`       | `-p`         | PATH     | add search Path (can be specified multiple times) |
| `--no-default` | `-n`         | -        | disable default search order (see below) |

## pe-info

Print information of `.exe` or `.dll` files.

### Usage

```
pe-info <exe_or_dll>...
```

## Known DLLs

A *known DLL* is a DLL that is well known by the system and therefore not copied to the target directory. Windows manages a list of known DLLs in registry (see
`HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\KnownDLLs`).

Use the `-k` option to add extra known DLLs.

Use the `-n` option to ignore registry (extra known DLLs will still be used).

## Default Search Order

The default search order is roughly oriented at DLL search order (see
[https://learn.microsoft.com/en-us/windows/win32/dlls/dynamic-link-library-search-order](https://learn.microsoft.com/en-us/windows/win32/dlls/dynamic-link-library-search-order)). Although, some parts are ignored,
such as DLL redirection, API sets, SxS manifests, loaded module lists,
package manifests and the 16 bit folder. This results effectively in
the following search order:

- known DLLs
- current path
- system directory
- windows directory
- path variable
- extra paths specified by `-p`

Use the `-n` option to default search order and onyl hone extra paths.

## Build

```bash
cmake -B build
cmake --build build
cmake --install build
```

### Run Unit Tests

```bash
cmake -B build
cmake --build build
ctest --test-dir build
```

### Generate Source Code Documentation

```bash
doxygen
```

The generated documentation can be found in the `doc` subdirectory.

## Design Principles

All PE Tools should have a least dependencies as possible so they can be executed on every
MS Windows installation without the need of additional DLLs. Therefore they are build
- without use of any C standard library
- without use of any default libraries

## References

- [PE Format](https://learn.microsoft.com/en-us/windows/win32/debug/pe-format)
- [Dynamic-link library search order](https://learn.microsoft.com/en-us/windows/win32/dlls/dynamic-link-library-search-order)