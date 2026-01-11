# Build Instructions - Cache System

Step-by-step guide to build and run the Cache System in Visual Studio.

---

## 📋 Prerequisites

### Required
- **Visual Studio 2022** (or Visual Studio 2019)
  - With "Desktop development with C++" workload
  - Windows 10 SDK
- **C++17 Standard** (configured in project)
- **Windows 10** or later

### Optional
- **Git** (for version control)
- **CMake** (if you want to create a cross-platform build)

---

## 🚀 Quick Start (Visual Studio)

### Step 1: Open the Solution

1. Navigate to: `D:\DesignPatterns\MultiThreading\Cache\`
2. Double-click `Cache.sln`
3. Visual Studio will open the solution

### Step 2: Verify Configuration

1. In **Solution Explorer**, right-click on the `Cache` project → **Properties**
2. Verify the following settings:

```
Configuration Properties → General:
  - Configuration Type: Application (.exe)
  - C++ Language Standard: ISO C++17 Standard (/std:c++17)
  - Windows SDK Version: 10.0 (or latest installed)

Configuration Properties → C/C++ → General:
  - Warning Level: Level3 (/W3)

Configuration Properties → C/C++ → Language:
  - C++ Language Standard: ISO C++17 Standard (/std:c++17)
  - Conformance mode: Yes (/permissive-)
```

### Step 3: Build the Project

**Method 1: Menu**
- Go to **Build** → **Build Solution** (or press `Ctrl+Shift+B`)

**Method 2: Toolbar**
- Click the green **Start** button (or press `F5` for debug, `Ctrl+F5` for release)

**Method 3: Command Line**
```powershell
cd D:\DesignPatterns\MultiThreading\Cache
msbuild Cache.sln /p:Configuration=Debug /p:Platform=x64
```

### Step 4: Run the Application

**Method 1: Visual Studio**
- Press `F5` (Debug mode) or `Ctrl+F5` (Release mode without debugging)

**Method 2: Command Line**
```powershell
.\x64\Debug\Cache.exe
```

---

## 📦 Project Structure

```
Cache/
├── Cache.sln                          # Visual Studio solution file
├── Cache/
│   ├── Cache.vcxproj                  # Visual Studio project file
│   ├── Cache.vcxproj.filters          # Project filters (file organization)
│   │
│   ├── main.cpp                       # Entry point / demo program
│   │
│   ├── Interfaces/                    # (logical grouping in filters)
│   │   ├── CacheStorage.h
│   │   ├── DBStorage.h
│   │   ├── WritePolicy.h
│   │   └── EvictionAlgorithm.h
│   │
│   ├── Implementations/               # (logical grouping in filters)
│   │   ├── InMemoryCacheStorage.h
│   │   ├── SimpleDBStorage.h
│   │   ├── WriteThroughPolicy.h
│   │   └── LRUEvictionAlgorithm.h
│   │
│   ├── Utilities/                     # (logical grouping in filters)
│   │   ├── DoublyLinkedList.h
│   │   └── KeyBasedExecutor.h
│   │
│   └── Cache.h                        # Core cache class
│
└── Documentation/
    ├── README.md
    ├── ARCHITECTURE.md
    ├── INTERVIEW_GUIDE.md
    └── QUICK_REFERENCE.md
```

---

## 🔧 Build Configurations

### Debug Configuration

**Purpose**: Development and debugging

**Settings**:
- Optimization: Disabled (`/Od`)
- Debug Info: Full (`/Zi`)
- Runtime Library: Multi-threaded Debug DLL (`/MDd`)
- Preprocessor: `_DEBUG`, `_CONSOLE`

**Output**: `x64\Debug\Cache.exe`

**Build Command**:
```powershell
msbuild Cache.sln /p:Configuration=Debug /p:Platform=x64
```

### Release Configuration

**Purpose**: Performance testing and deployment

**Settings**:
- Optimization: Maximize Speed (`/O2`)
- Inline Function Expansion: Any Suitable (`/Ob2`)
- Runtime Library: Multi-threaded DLL (`/MD`)
- Preprocessor: `NDEBUG`, `_CONSOLE`
- Link Time Code Generation: Use LTCG (`/GL`)

**Output**: `x64\Release\Cache.exe`

**Build Command**:
```powershell
msbuild Cache.sln /p:Configuration=Release /p:Platform=x64
```

---

## 🔍 Troubleshooting

### Issue 1: "Cannot open include file: 'Cache.h'"

**Cause**: Incorrect include paths

**Solution**:
1. Verify all `.h` files are in `Cache\Cache\` directory
2. In project properties:
   - C/C++ → General → Additional Include Directories: `$(ProjectDir);%(AdditionalIncludeDirectories)`

### Issue 2: "C++17 features not supported"

**Cause**: Wrong C++ standard setting

**Solution**:
1. Right-click project → Properties
2. C/C++ → Language → C++ Language Standard → Select **ISO C++17 Standard (/std:c++17)**

### Issue 3: "LNK2019: unresolved external symbol"

**Cause**: Missing source files or template instantiation issues

**Solution**:
- All implementations are header-only (templates)
- Verify `main.cpp` is included in the project
- Check `Cache.vcxproj` has `<ClCompile Include="main.cpp" />`

### Issue 4: Build succeeds but crashes on run

**Cause**: Potential threading issues or exceptions

**Solution**:
1. Run in Debug mode (F5)
2. Check Output window for exceptions
3. Verify cache capacity > 0
4. Check all pointers are valid before `cache.shutdown()`

### Issue 5: "std::future" or "std::async" errors

**Cause**: Missing `<future>` include or linking issues

**Solution**:
- Verify `#include <future>` in files using async operations
- No additional libraries needed (std::future is in standard library)

---

## 🧪 Verification Steps

After successful build, run the program and verify output:

### Expected Output

```
=== Cache System Demo ===

Writing 5 items to cache (capacity=5)...
Cache is now full.

Writing 6th item (will evict LRU key 'A')...
Key 'F' added, 'A' was evicted.

Attempting to read evicted key 'A'...
A is evicted or not found in cache.

Reading key 'F'...
F: Fig

Updating key 'B' and reading it back...
B: Blueberry

Testing thread affinity with multiple operations on 'C'...
C: Cranberry

=== Demo Complete ===
Key Design Points Demonstrated:
1. LRU Eviction: 'A' was evicted when 'F' was added
2. Write-Through: Data written to both cache and DB concurrently
3. Read-Your-Own-Writes: Updated 'B' immediately visible
4. Thread Affinity: Same key operations on same thread
5. Strategy Pattern: Pluggable eviction & write policies
```

### Exit Code
- **0**: Success
- **1**: Error occurred (check stderr)

---

## 🛠️ Alternative Build Methods

### Using Developer Command Prompt

```powershell
# Open "Developer Command Prompt for VS 2022"

# Navigate to project
cd D:\DesignPatterns\MultiThreading\Cache

# Clean previous builds
msbuild Cache.sln /t:Clean

# Build Debug
msbuild Cache.sln /p:Configuration=Debug /p:Platform=x64

# Build Release
msbuild Cache.sln /p:Configuration=Release /p:Platform=x64

# Run
.\x64\Debug\Cache.exe
```

### Using CMake (Cross-platform)

Create `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.10)
project(Cache)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB HEADERS "Cache/*.h")
add_executable(Cache Cache/main.cpp ${HEADERS})

# For Windows
if(WIN32)
    target_compile_options(Cache PRIVATE /W3)
endif()

# For Linux/Mac
if(UNIX)
    target_link_libraries(Cache pthread)
endif()
```

Build with CMake:

```bash
mkdir build && cd build
cmake ..
cmake --build .
./Cache
```

---

## 📊 Build Performance

| Configuration | Build Time | Binary Size | Startup Time |
|---------------|------------|-------------|--------------|
| Debug | ~5-10 sec | ~150 KB | ~50 ms |
| Release | ~10-15 sec | ~50 KB | ~10 ms |

*Times are approximate on a typical developer machine*

---

## 🚦 CI/CD Integration

### GitHub Actions Example

```yaml
name: Build Cache System

on: [push, pull_request]

jobs:
  build:
    runs-on: windows-latest
    
    steps:
    - uses: actions/checkout@v2
    
    - name: Setup MSBuild
      uses: microsoft/setup-msbuild@v1
    
    - name: Build Debug
      run: msbuild Cache.sln /p:Configuration=Debug /p:Platform=x64
    
    - name: Build Release
      run: msbuild Cache.sln /p:Configuration=Release /p:Platform=x64
    
    - name: Run Tests
      run: .\x64\Debug\Cache.exe
```

---

## 🎯 Next Steps After Building

1. **Read the Output**: Verify all demo scenarios pass
2. **Modify main.cpp**: Experiment with different cache sizes, thread counts
3. **Review Code**: Open header files in Visual Studio, explore implementation
4. **Interview Prep**: Use `INTERVIEW_GUIDE.md` for discussion points
5. **Extend**: Add LFU eviction or Write-Back policy (see `ARCHITECTURE.md`)

---

## 📞 Support

**Issues**:
- Verify Prerequisites are installed
- Check Visual Studio version (2019 or 2022 recommended)
- Ensure Windows SDK is installed
- Try cleaning solution: Build → Clean Solution

**Performance Issues**:
- Build in Release mode for performance testing
- Debug mode has significant overhead for multithreading

**Documentation**:
- `README.md` - Overview
- `ARCHITECTURE.md` - Technical details
- `INTERVIEW_GUIDE.md` - Discussion points
- `QUICK_REFERENCE.md` - Cheat sheet

---

## ✅ Build Checklist

- [ ] Visual Studio 2022 installed with C++ workload
- [ ] Project opens without errors
- [ ] All header files visible in Solution Explorer
- [ ] C++17 standard is configured
- [ ] Build succeeds (0 errors, 0 warnings)
- [ ] Program runs and produces expected output
- [ ] Exit code is 0 (success)

---

**Happy Building!** 🚀

**Build Time**: ~10 seconds  
**Complexity**: Simple (header-only templates)  
**Platform**: Windows (Visual Studio)

