# Build Instructions for Elevator System

## 🛠️ Prerequisites

### Required Software
- **Visual Studio 2022** (Community, Professional, or Enterprise)
- **Windows 10/11**
- **C++17 or later** compiler support

### Visual Studio Components
Ensure you have installed:
- Desktop development with C++
- Windows 10 SDK (or later)
- MSVC v143 - VS 2022 C++ x64/x86 build tools

---

## 🚀 Method 1: Visual Studio IDE (Recommended)

### Step 1: Open the Solution
1. Navigate to: `D:\DesignPatterns\MultiThreading\Elevator\`
2. Double-click `Elevator.sln`
3. Visual Studio will open the solution

### Step 2: Configure Build
1. At the top, select configuration:
   - **Configuration**: Debug or Release
   - **Platform**: x64 (recommended) or Win32

### Step 3: Build the Project
**Option A**: Menu
- Go to `Build` → `Build Solution` (or press `Ctrl+Shift+B`)

**Option B**: Toolbar
- Click the green "Build" button

### Step 4: Run the Application
**Option A**: With Debugging
- Press `F5` or click "Local Windows Debugger"

**Option B**: Without Debugging (Recommended)
- Press `Ctrl+F5` or `Debug` → `Start Without Debugging`

### Expected Output
```
========================================
   Elevator System Simulation
========================================
Building: Office Tower
Floors: 10
Elevators: 3
========================================

--- Select an option ---
1. Request elevator (external)
2. Request floor (internal)
3. Simulate next step
4. Change scheduling strategy
5. Exit simulation
Choice:
```

---

## 🖥️ Method 2: Developer Command Prompt

### Step 1: Open Developer Command Prompt
1. Press `Win` key
2. Type "Developer Command Prompt for VS 2022"
3. Run as Administrator (optional, but recommended)

### Step 2: Navigate to Project Directory
```cmd
cd D:\DesignPatterns\MultiThreading\Elevator
```

### Step 3: Build with MSBuild
**For Debug build (x64)**:
```cmd
msbuild Elevator.sln /p:Configuration=Debug /p:Platform=x64
```

**For Release build (x64)**:
```cmd
msbuild Elevator.sln /p:Configuration=Release /p:Platform=x64
```

**For Win32 platform**:
```cmd
msbuild Elevator.sln /p:Configuration=Debug /p:Platform=Win32
```

### Step 4: Run the Executable
**For x64 Debug**:
```cmd
cd x64\Debug
Elevator.exe
```

**For x64 Release**:
```cmd
cd x64\Release
Elevator.exe
```

**For Win32**:
```cmd
cd Debug
Elevator.exe
```

---

## 🔧 Method 3: PowerShell (Alternative)

### Step 1: Open PowerShell
1. Navigate to project directory
2. Ensure MSBuild is in PATH

### Step 2: Build
```powershell
cd D:\DesignPatterns\MultiThreading\Elevator
& "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" Elevator.sln /p:Configuration=Debug /p:Platform=x64
```

### Step 3: Run
```powershell
cd x64\Debug
.\Elevator.exe
```

---

## 🧪 Verify Build Success

### Check for Output Files
After successful build, you should see:

**For x64 Debug**:
```
D:\DesignPatterns\MultiThreading\Elevator\x64\Debug\
  ├── Elevator.exe
  ├── Elevator.pdb
  └── Elevator.ilk
```

**For x64 Release**:
```
D:\DesignPatterns\MultiThreading\Elevator\x64\Release\
  ├── Elevator.exe
  └── Elevator.pdb
```

---

## 🐛 Troubleshooting

### Issue 1: "MSBuild is not recognized"
**Solution**:
- Use "Developer Command Prompt for VS 2022" instead of regular command prompt
- OR add MSBuild to PATH:
  ```cmd
  set PATH=%PATH%;C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin
  ```

### Issue 2: "Cannot open include file"
**Symptom**: Errors like `fatal error C1083: Cannot open include file: 'iostream'`

**Solution**:
- Ensure "Desktop development with C++" is installed in Visual Studio
- Repair Visual Studio installation
- Check Windows SDK is installed

### Issue 3: "LNK2019: unresolved external symbol"
**Symptom**: Linker errors about undefined symbols

**Solution**:
- Clean and rebuild: `Build` → `Clean Solution`, then `Build Solution`
- Ensure all `.cpp` files are included in project
- Check `Elevator.vcxproj` has all source files listed

### Issue 4: "Platform 'x64' not found"
**Solution**:
- Install x64 build tools in Visual Studio Installer
- OR use Win32 platform instead:
  ```cmd
  msbuild Elevator.sln /p:Configuration=Debug /p:Platform=Win32
  ```

### Issue 5: Build succeeds but .exe not found
**Solution**:
- Check the output directory:
  - x64 builds: `x64\Debug\` or `x64\Release\`
  - Win32 builds: `Debug\` or `Release\`
- Look in Visual Studio Output window for actual path

---

## 🧹 Clean Build

### Visual Studio IDE
1. `Build` → `Clean Solution`
2. `Build` → `Rebuild Solution`

### Command Line
```cmd
msbuild Elevator.sln /t:Clean
msbuild Elevator.sln /t:Rebuild /p:Configuration=Debug /p:Platform=x64
```

---

## 📊 Build Configurations

### Debug vs Release

| Feature | Debug | Release |
|---------|-------|---------|
| Optimization | None | Full |
| Debug Symbols | Yes | Minimal |
| Size | Larger | Smaller |
| Speed | Slower | Faster |
| Use Case | Development | Production |

**Recommendation**: Use Debug for development and testing, Release for final demo.

---

## 🔍 Verify Project Structure

### Expected Files in Elevator/ Directory
```
Elevator/
├── Building.cpp
├── Building.h
├── CommonEnums.h
├── Elevator.cpp
├── Elevator.h
├── ElevatorCommand.h
├── ElevatorController.cpp
├── ElevatorController.h
├── ElevatorDisplay.cpp
├── ElevatorDisplay.h
├── ElevatorObserver.h
├── ElevatorRequest.cpp
├── ElevatorRequest.h
├── Elevator.vcxproj
├── Elevator.vcxproj.filters
├── Elevator.vcxproj.user
├── FCFSSchedulingStrategy.cpp
├── FCFSSchedulingStrategy.h
├── Floor.cpp
├── Floor.h
├── LookSchedulingStrategy.cpp
├── LookSchedulingStrategy.h
├── main.cpp
├── ScanSchedulingStrategy.cpp
├── ScanSchedulingStrategy.h
└── SchedulingStrategy.h
```

### Verify with PowerShell
```powershell
cd Elevator
Get-ChildItem -Name *.h,*.cpp | Sort-Object
```

### Expected Output
```
Building.cpp
Building.h
CommonEnums.h
Elevator.cpp
Elevator.h
ElevatorCommand.h
ElevatorController.cpp
ElevatorController.h
ElevatorDisplay.cpp
ElevatorDisplay.h
ElevatorObserver.h
ElevatorRequest.cpp
ElevatorRequest.h
FCFSSchedulingStrategy.cpp
FCFSSchedulingStrategy.h
Floor.cpp
Floor.h
LookSchedulingStrategy.cpp
LookSchedulingStrategy.h
main.cpp
ScanSchedulingStrategy.cpp
ScanSchedulingStrategy.h
SchedulingStrategy.h
```

---

## ⚙️ Advanced Build Options

### Parallel Build (Faster)
```cmd
msbuild Elevator.sln /p:Configuration=Debug /p:Platform=x64 /m
```
The `/m` flag enables multi-processor compilation.

### Verbose Output
```cmd
msbuild Elevator.sln /p:Configuration=Debug /p:Platform=x64 /v:detailed
```

### Build Specific Project Only
```cmd
msbuild Elevator\Elevator.vcxproj /p:Configuration=Debug /p:Platform=x64
```

---

## 📝 Build Output Interpretation

### Successful Build
```
Build started...
1>------ Build started: Project: Elevator, Configuration: Debug x64 ------
1>Building...
1>Compiling...
1>  main.cpp
1>  Building.cpp
1>  Elevator.cpp
1>  ElevatorController.cpp
1>  ... (more files)
1>Linking...
1>Elevator.vcxproj -> D:\...\x64\Debug\Elevator.exe
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========
```

### Failed Build
```
Build started...
1>------ Build started: Project: Elevator, Configuration: Debug x64 ------
1>Building...
1>Compiling...
1>  Elevator.cpp
1>D:\...\Elevator.cpp(25,10): error C2065: 'undefinedVariable' : undeclared identifier
========== Build: 0 succeeded, 1 failed, 0 up-to-date, 0 skipped ==========
```

---

## 🎯 Quick Start Commands

### One-Line Build and Run (PowerShell)
```powershell
cd D:\DesignPatterns\MultiThreading\Elevator; msbuild Elevator.sln /p:Configuration=Debug /p:Platform=x64 /m; cd x64\Debug; .\Elevator.exe
```

### One-Line Clean, Build, Run (PowerShell)
```powershell
cd D:\DesignPatterns\MultiThreading\Elevator; msbuild Elevator.sln /t:Clean; msbuild Elevator.sln /p:Configuration=Debug /p:Platform=x64 /m; cd x64\Debug; .\Elevator.exe
```

---

## 📚 Additional Resources

### Visual Studio Documentation
- [MSBuild Command-Line Reference](https://docs.microsoft.com/en-us/visualstudio/msbuild/msbuild-command-line-reference)
- [C++ Project Properties](https://docs.microsoft.com/en-us/cpp/build/reference/c-cpp-prop-page)

### Keyboard Shortcuts
| Action | Shortcut |
|--------|----------|
| Build Solution | Ctrl+Shift+B |
| Start Debugging | F5 |
| Start Without Debugging | Ctrl+F5 |
| Stop Debugging | Shift+F5 |
| Clean Solution | - |
| Rebuild Solution | - |

---

## ✅ Post-Build Checklist

- [ ] Build completed without errors
- [ ] Build completed without warnings (optional)
- [ ] Executable file created in output directory
- [ ] Application runs and displays menu
- [ ] Can create elevator requests
- [ ] Can simulate elevator movement
- [ ] Can switch scheduling strategies
- [ ] Application exits cleanly

---

## 🎓 For Interview Preparation

### Before the Interview
1. Build the project in **Release** mode for best performance
2. Test all menu options to ensure they work
3. Prepare 2-3 test scenarios to demonstrate
4. Have the code open in Visual Studio for quick navigation

### During the Interview
1. Show the running application first
2. Demonstrate a simple scenario (external + internal request)
3. Switch scheduling strategies to show flexibility
4. Walk through the code starting from `main.cpp`
5. Explain design patterns with code examples

---

**Happy Building! 🚀**

If you encounter any issues not covered here, check the Visual Studio Output window for detailed error messages.

