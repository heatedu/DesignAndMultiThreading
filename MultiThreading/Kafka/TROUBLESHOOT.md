# Troubleshooting Guide - Unable to Run Solution

## Step-by-Step Diagnosis

### Step 1: Check Build Status

**In Visual Studio Output Window:**

✅ **SUCCESS looks like:**

```
Build started...
1>------ Build started: Project: Kafka, Configuration: Debug x64 ------
1>Main.cpp
1>KafkaController.cpp
...
1>Kafka.vcxproj -> D:\DesignPatterns\MultiThreading\Kafka\x64\Debug\Kafka.exe
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========
```

❌ **FAILURE looks like:**

```
========== Build: 0 succeeded, 1 failed, 0 up-to-date, 0 skipped ==========
```

### Step 2: Common Issues & Fixes

#### Issue A: "LNK1120: unresolved externals" or "LNK2019"

**Symptoms:** Linker errors about undefined symbols

**Fix:**

1. Check all `.cpp` files are in the project
2. In Visual Studio Solution Explorer:
   - Right-click project → Add → Existing Item
   - Add any missing `.cpp` files

**Verify these files exist:**

- ✅ KafkaController.cpp
- ✅ SimplePublisher.cpp
- ✅ SimpleSubscriber.cpp
- ✅ TopicSubscriberController.cpp
- ✅ Main.cpp

#### Issue B: "Cannot open include file"

**Symptoms:**

```
fatal error C1083: Cannot open include file: 'KafkaController.h'
```

**Fix:**

1. Ensure all `.h` files are in: `D:\DesignPatterns\MultiThreading\Kafka\Kafka\`
2. Right-click project → Properties → C/C++ → General
3. Check "Additional Include Directories" is empty or has `$(ProjectDir)`

#### Issue C: "Build succeeded but nothing happens when running"

**Fix:**

1. Press `Ctrl+F5` (Run Without Debugging) instead of `F5`
2. Check if console window closes immediately
3. Set project as Startup Project:
   - Right-click "Kafka" project → Set as Startup Project
   - Should appear in **bold**

#### Issue D: Platform Mismatch (x64 vs x86)

**Fix:**

1. Check toolbar at top of Visual Studio
2. Change from "Win32" to "**x64**"
3. Rebuild solution

#### Issue E: Missing .exe file

**Check if file exists:**

```
D:\DesignPatterns\MultiThreading\Kafka\x64\Debug\Kafka.exe
```

**If missing:**

1. Build → Clean Solution
2. Build → Rebuild Solution
3. Check Output window for errors

### Step 3: Fresh Start (Nuclear Option)

If nothing works, do a complete reset:

**PowerShell Commands:**

```powershell
cd D:\DesignPatterns\MultiThreading\Kafka

# Stop any processes
Get-Process | Where-Object {$_.Path -like "*Kafka*"} | Stop-Process -Force -ErrorAction SilentlyContinue

# Delete ALL build artifacts
Remove-Item -Recurse -Force ".vs" -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force "Kafka\x64" -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force "Kafka\Debug" -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force "x64" -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force "Debug" -ErrorAction SilentlyContinue

Write-Host "Clean complete. Reopen Visual Studio." -ForegroundColor Green
```

**Then:**

1. Reopen Visual Studio
2. Open `Kafka.sln`
3. Build → Rebuild Solution
4. Press `Ctrl+F5` to run

### Step 4: Build from Command Line (Bypass Visual Studio)

Open **Developer Command Prompt for VS 2022**:

```cmd
cd D:\DesignPatterns\MultiThreading\Kafka
msbuild Kafka.sln /t:Clean
msbuild Kafka.sln /t:Rebuild /p:Configuration=Debug /p:Platform=x64
```

**If successful, run:**

```cmd
x64\Debug\Kafka.exe
```

### Step 5: Verify All Files Exist

Check this folder: `D:\DesignPatterns\MultiThreading\Kafka\Kafka\`

**Required files:**

```
✅ Main.cpp
✅ Message.h
✅ Topic.h
✅ IPublisher.h
✅ ISubscriber.h
✅ TopicSubscriber.h
✅ TopicSubscriberController.h
✅ TopicSubscriberController.cpp
✅ KafkaController.h
✅ KafkaController.cpp
✅ SimplePublisher.h
✅ SimplePublisher.cpp
✅ SimpleSubscriber.h
✅ SimpleSubscriber.cpp
✅ Kafka.vcxproj
✅ Kafka.vcxproj.filters
```

### Step 6: Check Visual Studio Configuration

1. **Menu: Tools → Options → Projects and Solutions → Build and Run**
2. Set "MSBuild project build output verbosity" to **Detailed**
3. Rebuild and check Output window for errors

### Step 7: Verify C++ Toolset

1. **Visual Studio Installer** → Modify
2. Ensure these are installed:
   - ✅ Desktop development with C++
   - ✅ MSVC v143 - VS 2022 C++ x64/x86 build tools
   - ✅ Windows 10 SDK

## Quick Diagnostics Checklist

Run through this checklist:

- [ ] Visual Studio 2022 is installed
- [ ] Opened `Kafka.sln` (not individual files)
- [ ] Platform is set to **x64** (not Win32)
- [ ] Configuration is **Debug**
- [ ] "Kafka" project is in bold (startup project)
- [ ] Build shows "1 succeeded"
- [ ] All 14 source files are present
- [ ] No antivirus blocking

## Expected Successful Output

When you press `Ctrl+F5`, you should see:

```
========================================
  KAFKA PUB-SUB SYSTEM DEMO
========================================

Created topic: Topic1 with id: 1
Created topic: Topic2 with id: 2

Subscriber Subscriber1 subscribed to topic: Topic1
Subscriber Subscriber1 subscribed to topic: Topic2
Subscriber Subscriber2 subscribed to topic: Topic1
Subscriber Subscriber3 subscribed to topic: Topic2

--- Publishing Messages ---

Publisher Publisher1 published: Message m1 to topic 1
Message "Message m1" published to topic: Topic1
Subscriber Subscriber1 received: Message m1
Subscriber Subscriber2 received: Message m1
...

--- Shutting Down ---

KafkaController shutdown complete

Demo completed successfully!
```

## Still Not Working?

**Tell me:**

1. What's the exact error message?
2. Does build say "succeeded" or "failed"?
3. What happens when you press F5 or Ctrl+F5?
4. Are you using Visual Studio 2022 or an older version?
5. Screenshot of the Error List window?

I'll help you fix it! 🔧
