# Build & Run Instructions

## Prerequisites
- **Visual Studio 2022** (or 2019 with v143 platform toolset)
- **Windows SDK 10.0**
- **C++17 or higher** support

## Quick Start (Visual Studio IDE)

### Option 1: Build from IDE
1. Navigate to `D:\DesignPatterns\MultiThreading\RateLimiter\`
2. Double-click `RateLimiter.sln`
3. Visual Studio will open the solution
4. Press **F5** (Run with debugging) or **Ctrl+F5** (Run without debugging)

### Option 2: Build from Developer Command Prompt
```cmd
cd D:\DesignPatterns\MultiThreading\RateLimiter
MSBuild RateLimiter.sln /p:Configuration=Debug /p:Platform=x64
.\x64\Debug\RateLimiter.exe
```

### Option 3: VS Code Build Task
```cmd
cd D:\DesignPatterns\MultiThreading\RateLimiter\RateLimiter
cl /EHsc /std:c++17 main.cpp /Fe:RateLimiter.exe
RateLimiter.exe
```

## Project Configuration

### Compiler Settings
- **Language Standard**: C++17 (`/std:c++17`)
- **Character Set**: Unicode
- **Warning Level**: Level 3
- **SDL Check**: Enabled
- **Subsystem**: Console

### Platform Support
- ✅ x64 (Debug/Release)
- ✅ Win32 (Debug/Release)

## Expected Output

```
=== Rate Limiter System - Token Bucket Implementation ===

=== EXAMPLE 1: Global rate limiting - Burst of requests ===
Request with key [global]: ✅ Allowed
Request with key [global]: ✅ Allowed
Request with key [global]: ✅ Allowed
Request with key [global]: ✅ Allowed
Request with key [global]: ✅ Allowed
Request with key [global]: ❌ Blocked
Request with key [global]: ❌ Blocked
Request with key [global]: ❌ Blocked
Request with key [global]: ❌ Blocked
Request with key [global]: ❌ Blocked
Results: 5 allowed, 5 blocked (total: 10)

=== EXAMPLE 2: Global rate limiting - After waiting for tokens refill ===
Waiting 5 seconds for tokens to refill...
Request with key [global]: ✅ Allowed
Request with key [global]: ✅ Allowed
Request with key [global]: ✅ Allowed
Request with key [global]: ✅ Allowed
Request with key [global]: ✅ Allowed
Request with key [global]: ❌ Blocked
Request with key [global]: ❌ Blocked
Request with key [global]: ❌ Blocked
Request with key [global]: ❌ Blocked
Request with key [global]: ❌ Blocked
Results: 5 allowed, 5 blocked (total: 10)

Requests for user1:
Request with key [user1]: ✅ Allowed
Request with key [user1]: ✅ Allowed
Request with key [user1]: ✅ Allowed
Request with key [user1]: ✅ Allowed
Request with key [user1]: ✅ Allowed
Request with key [user1]: ❌ Blocked
Request with key [user1]: ❌ Blocked
Results: 5 allowed, 2 blocked (total: 7)

Requests for user2:
Request with key [user2]: ✅ Allowed
Request with key [user2]: ✅ Allowed
Request with key [user2]: ✅ Allowed
Request with key [user2]: ✅ Allowed
Request with key [user2]: ✅ Allowed
Request with key [user2]: ❌ Blocked
Request with key [user2]: ❌ Blocked
Results: 5 allowed, 2 blocked (total: 7)

Requests for user3:
Request with key [user3]: ✅ Allowed
Request with key [user3]: ✅ Allowed
Request with key [user3]: ✅ Allowed
Request with key [user3]: ✅ Allowed
Request with key [user3]: ✅ Allowed
Request with key [user3]: ❌ Blocked
Request with key [user3]: ❌ Blocked
Results: 5 allowed, 2 blocked (total: 7)

=== EXAMPLE 4: High concurrency scenario ===
Request with key [global]: ✅ Allowed
Request with key [global]: ✅ Allowed
Request with key [global]: ✅ Allowed
Request with key [global]: ✅ Allowed
Request with key [global]: ✅ Allowed
Request with key [global]: ❌ Blocked
Request with key [global]: ❌ Blocked
Request with key [global]: ❌ Blocked
Request with key [global]: ❌ Blocked
Request with key [global]: ❌ Blocked
Request with key [global]: ❌ Blocked
Request with key [global]: ❌ Blocked
Request with key [global]: ❌ Blocked
Request with key [global]: ❌ Blocked
Request with key [global]: ❌ Blocked
Request with key [global]: ❌ Blocked
Request with key [global]: ❌ Blocked
Request with key [global]: ❌ Blocked
Request with key [global]: ❌ Blocked
Request with key [global]: ❌ Blocked
High concurrency results: 5 allowed, 15 blocked

=== Demo Complete ===
```

## Troubleshooting

### Issue: "Cannot open include file: '<header>'"
**Solution**: Ensure C++17 standard library is available. Update Visual Studio to latest version.

### Issue: "LNK2019: unresolved external symbol"
**Solution**: Rebuild solution (Ctrl+Shift+B). This is header-only implementation, so no linking issues should occur.

### Issue: Build succeeds but exe crashes
**Solution**: 
- Check thread count (default 10 workers should be safe)
- Ensure proper shutdown logic runs (RAII handles cleanup)
- Run with debugger (F5) to see exception details

### Issue: "MSBuild not found"
**Solution**: Run from **Developer Command Prompt for VS 2022**:
```
Start Menu → Visual Studio 2022 → Developer Command Prompt for VS 2022
```

## File Structure After Build

```
RateLimiter/
├── RateLimiter.sln              # Solution file (double-click to open)
├── RateLimiter/
│   ├── main.cpp                 # Entry point
│   ├── IRateLimiter.h           # Interface
│   ├── TokenBucketStrategy.h    # Token Bucket impl
│   ├── RateLimiterType.h        # Enum
│   ├── RateLimiterFactory.h     # Factory pattern
│   ├── RateLimiterController.h  # Controller with thread pool
│   ├── RateLimiter.vcxproj      # Project file
│   ├── RateLimiter.vcxproj.filters
│   ├── README.md                # Overview
│   ├── INTERVIEW_GUIDE.md       # Interview prep
│   └── BUILD_INSTRUCTIONS.md    # This file
└── x64/Debug/                   # Build output (after build)
    └── RateLimiter.exe          # Executable
```

## Configuration Options

Edit `main.cpp` to adjust:

```cpp
std::unordered_map<std::string, int> config;
config["capacity"] = 5;        // Max tokens per bucket
config["refreshRate"] = 1;     // Tokens added per second

// Thread pool size
RateLimiterController controller(
    RateLimiterType::TOKEN_BUCKET, 
    config, 
    10  // <-- Number of worker threads
);
```

## Testing Different Scenarios

### Test 1: Higher Capacity
```cpp
config["capacity"] = 100;
config["refreshRate"] = 10;
```

### Test 2: Stricter Rate Limiting
```cpp
config["capacity"] = 2;
config["refreshRate"] = 1;
```

### Test 3: More Users
```cpp
std::vector<std::string> users = {"user1", "user2", ..., "user100"};
```

## Performance Notes

- **Thread Pool**: Default 10 workers handles ~1000 req/sec
- **Token Refill**: 1 second interval (configurable in `TokenBucketStrategy.h`)
- **Mutex Contention**: Per-bucket locks minimize contention
- **Memory**: ~64 bytes per user bucket + map overhead

## Clean Build

```cmd
# Visual Studio
Build → Clean Solution
Build → Rebuild Solution

# Command Line
MSBuild RateLimiter.sln /t:Clean
MSBuild RateLimiter.sln /t:Rebuild /p:Configuration=Debug /p:Platform=x64
```

## Running Tests

The `main.cpp` includes 4 test scenarios:
1. ✅ Global rate limiting (burst)
2. ✅ Token refill verification
3. ✅ Per-user rate limiting
4. ✅ High concurrency stress test

All tests run automatically on `main()` execution.

## Next Steps

1. ✅ Build and run the default demo
2. Modify `main.cpp` to test custom scenarios
3. Review `INTERVIEW_GUIDE.md` for discussion points
4. Extend with new algorithms (Fixed Window, etc.)

## Support

For issues or questions:
- Check `README.md` for architecture overview
- Review `INTERVIEW_GUIDE.md` for implementation details
- Examine header files for inline documentation

