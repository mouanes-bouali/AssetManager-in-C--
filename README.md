# Asset Mangaer
A subsystem that handles the game assets from the assets folder.It fetches exisitng files, loads needed assets in cache and remove non needed ones for performance. It works in Async/background so it doesn' freeze when loading large files.


## Repository Structure

```
cpp-template/
├── .gitignore
├── README.md
├── build.bat              # Windows build script
├── build.sh               # Linux/macOS build script
├── snippets/
│   └── cpp.json           # VS Code C++ snippets
└── AssetManager/
    ├── CMakeLists.txt     # Root CMake (C++17)
    ├── src/
    │   ├── CMakeLists.txt # Library + app targets
    │   ├── main.cpp       # Application entry point
    │   └── core/
    │       ├── mylib.h
    │       └── mylib.cpp
    ├── test/
    │   ├── CMakeLists.txt # Catch2 auto-download
    │   └── test_mylib.cpp
    └── .vscode/
        ├── settings.json
        └── extensions.json
```
## Documentation

## Quick Start

### 1. Clone This Repo

```bash
git clone https://github.com/yourusername/cpp-template.git
cd cpp-template


# Run
./build/bin/myapp        # Linux/macOS
build\bin\Release\myapp.exe  # Windows
```

### 3. Symlink Snippets to VS Code

**Windows (PowerShell as Admin):**
```powershell
# Remove existing C++ snippets if any
Remove-Item "$env:APPDATA\Code\User\snippets\cpp.json" -ErrorAction SilentlyContinue

# Create symlink
New-Item -ItemType SymbolicLink `
    -Path "$env:APPDATA\Code\User\snippets\cpp.json" `
    -Target "C:\Users\ACER\Desktop\C++ Files\cpp-template\snippets\cpp.json"
```

**Linux/macOS:**
```bash
# Remove existing C++ snippets if any
rm -f ~/.config/Code/User/snippets/cpp.json

# Create symlink
ln -s ~/cpp-template/snippets/cpp.json ~/.config/Code/User/snippets/cpp.json
```

### 4. Sync to a New PC

```bash
# Clone the template repo on the new machine
git clone https://github.com/yourusername/cpp-template.git

# Symlink snippets (see step 3 for your OS)
# Start new projects from the template (see step 2)
```

## Snippets Included

| Prefix       | Description                          |
|-------------|--------------------------------------|
| `cppclass`  | C++ class with rule of five           |
| `cppmain`   | C++ main function                     |
| `guard`     | Header include guard                  |
| `cpptest`   | Catch2 test case                      |
| `cmake_exe` | CMake add_executable target           |
| `cmake_lib` | CMake add_library target              |
| `cppinterface` | C++ interface (pure virtual)       |
| `cppabstract` | C++ abstract class                  |
| `cppns`     | C++ namespace block                   |
| `singleton` | Meyers' singleton pattern             |
| `cppfor`    | Range-based for loop                  |
| `cppunique` | std::unique_ptr with make_unique      |

## VS Code Extensions

The template recommends these extensions (see `.vscode/extensions.json`):

- **C/C++** (`ms-vscode.cpptools`) — IntelliSense, debugging
- **CMake Tools** (`ms-vscode.cmake-tools`) — CMake integration
- **clangd** (`llvm-vs-code-extensions.vscode-clangd`) — Code completion
- **GitLens** (`eamodio.gitlens`) — Git supercharged
- **Error Lens** (`usernamehw.errorlens`) — Inline error highlighting
- **CMake** (`twxs.cmake`) — CMake language support

## Build Options

### Using CMake Presets (Recommended)
```bash
# Configure and build (debug)
cmake --preset debug
cmake --build --preset debug

# Configure and build (release)
cmake --preset release
cmake --build --preset release

# Run tests
ctest --preset default
```

### Manual CMake
```bash
# Build with tests (default)
cmake -DBUILD_TESTS=ON ..

# Build without tests
cmake -DBUILD_TESTS=OFF ..

# Run tests
cd build && ctest --output-on-failure
```

## Customizing the Template

1. **Rename the project**: Edit `project-template/CMakeLists.txt` — change `MyProject`
2. **Rename the library**: Rename `mylib/` folder and update `src/CMakeLists.txt`
3. **Rename the app**: Change `myapp` in `src/CMakeLists.txt`
4. **Add more libraries**: Create new folders under `src/` and add `add_subdirectory()` calls
5. **Change C++ standard**: Edit `set(CMAKE_CXX_STANDARD 17)` in root `CMakeLists.txt`

