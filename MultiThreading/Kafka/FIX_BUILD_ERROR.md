# Fix C1090 PDB API Call Failed Error

## Error Message
```
Error C1090: PDB API call failed, error code '12': 
D:\DesignPatterns\MultiThreading\Kafka\Kafka\x64\Debug\vc143.pdb
```

## Cause
The PDB (Program Database) file is locked by another process or corrupted.

## Solutions (Try in Order)

### Solution 1: Clean and Rebuild (Quickest)

1. **In Visual Studio:**
   - Menu: `Build` → `Clean Solution`
   - Wait for it to complete
   - Menu: `Build` → `Rebuild Solution`

### Solution 2: Close All and Delete Build Files

1. **Close Visual Studio completely**
2. **Close any running instances** of the program (check Task Manager)
3. **Delete these folders:**
   ```
   D:\DesignPatterns\MultiThreading\Kafka\Kafka\x64\
   D:\DesignPatterns\MultiThreading\Kafka\Kafka\Debug\
   D:\DesignPatterns\MultiThreading\Kafka\.vs\
   ```
4. **Reopen Visual Studio**
5. **Build Solution** (Ctrl+Shift+B)

### Solution 3: PowerShell Script (Automated)

Run this in PowerShell (as Administrator):

```powershell
# Navigate to project directory
cd "D:\DesignPatterns\MultiThreading\Kafka"

# Stop any running processes
Get-Process | Where-Object {$_.Path -like "*Kafka*"} | Stop-Process -Force

# Delete build artifacts
Remove-Item -Recurse -Force "Kafka\x64" -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force "Kafka\Debug" -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force ".vs" -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force "x64" -ErrorAction SilentlyContinue

Write-Host "Build artifacts cleaned. Reopen Visual Studio and rebuild." -ForegroundColor Green
```

### Solution 4: Manual File Deletion

If Visual Studio is still open and you can't close it:

1. Open File Explorer
2. Navigate to: `D:\DesignPatterns\MultiThreading\Kafka\Kafka\x64\Debug\`
3. **Delete `vc143.pdb`** file manually
4. Back in Visual Studio: `Build` → `Rebuild Solution`

### Solution 5: Disable Minimal Rebuild (Permanent Fix)

This prevents the issue in the future:

1. In Visual Studio: **Project → Properties**
2. **Configuration Properties → C/C++ → Code Generation**
3. Set **"Enable Minimal Rebuild"** to **No**
4. Click **OK**
5. **Rebuild Solution**

## Quick Command Line Build

If Visual Studio keeps failing, try building from command line:

```cmd
# Open "Developer Command Prompt for VS 2022"
cd D:\DesignPatterns\MultiThreading\Kafka
msbuild Kafka.sln /t:Clean
msbuild Kafka.sln /t:Rebuild /p:Configuration=Debug /p:Platform=x64
```

## Prevention

Add this to your `.gitignore` (if using Git):

```gitignore
*.pdb
x64/
Debug/
.vs/
```

## Still Not Working?

### Check Anti-Virus
Some anti-virus software locks PDB files. Try:
1. Temporarily disable anti-virus
2. Add project folder to exclusions
3. Rebuild

### Check Disk Space
Ensure you have at least 500MB free space on D: drive

### Restart Computer
If all else fails, restart Windows to release all file locks.
```

## ✅ Expected Success

After cleaning and rebuilding, you should see:

```
Build started...
1>------ Build started: Project: Kafka, Configuration: Debug x64 ------
1>Main.cpp
1>KafkaController.cpp
1>SimplePublisher.cpp
1>SimpleSubscriber.cpp
1>TopicSubscriberController.cpp
1>Kafka.vcxproj -> D:\DesignPatterns\MultiThreading\Kafka\x64\Debug\Kafka.exe
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========
```

