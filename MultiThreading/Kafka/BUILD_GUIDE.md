# Build & Run Guide

## Visual Studio (Recommended)

1. **Open Solution**
   - Double-click `Kafka.sln` to open in Visual Studio 2022
   - OR Open Visual Studio → File → Open → Project/Solution → Select `Kafka.sln`

2. **Build**
   - Press `Ctrl+Shift+B` or
   - Menu: Build → Build Solution

3. **Run**
   - Press `F5` (with debugging) or `Ctrl+F5` (without debugging)
   - OR Menu: Debug → Start Debugging

## Visual Studio Code (Alternative)

If using VS Code with C++ extension:

1. **Install Required Extensions**
   - C/C++ Extension Pack (by Microsoft)
   - CMake Tools (optional)

2. **Configure Build Task**
   - Open VS Code in the project folder
   - Terminal → Configure Default Build Task
   - Select "MSBuild" or "C++ Compiler"

3. **Build from Developer Command Prompt**
   ```cmd
   # Open "Developer Command Prompt for VS 2022"
   cd D:\DesignPatterns\MultiThreading\Kafka
   msbuild Kafka.sln /p:Configuration=Debug /p:Platform=x64
   ```

4. **Run**
   ```cmd
   .\x64\Debug\Kafka.exe
   ```

## Troubleshooting

### Issue: "Cannot open include file"
**Solution**: Make sure all .h files are in the same directory as .cpp files

### Issue: "Unresolved external symbol"
**Solution**: Verify all .cpp files are listed in Kafka.vcxproj:
- KafkaController.cpp
- SimplePublisher.cpp
- SimpleSubscriber.cpp
- TopicSubscriberController.cpp
- Main.cpp

### Issue: Build fails with threading errors
**Solution**: Ensure you're building for x64, not Win32 (x64 has better C++11 thread support)

## Expected Output

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

Publisher Publisher1 published: Message m2 to topic 1
Message "Message m2" published to topic: Topic1
Subscriber Subscriber1 received: Message m2
Subscriber Subscriber2 received: Message m2

...

--- Resetting Offset (Re-processing) ---

Offset for subscriber Subscriber1 on topic Topic1 reset to 0
Subscriber Subscriber1 received: Message m1
Subscriber Subscriber1 received: Message m2
Subscriber Subscriber1 received: Message m5

--- Shutting Down ---

KafkaController shutdown complete

Demo completed successfully!
```

## Quick Test

To verify the system works correctly, look for:
1. ✅ Topics created with unique IDs
2. ✅ Subscribers registered to topics
3. ✅ Messages published and received in order
4. ✅ Multiple subscribers receive same message (broadcast)
5. ✅ Offset reset causes message replay
6. ✅ Clean shutdown without crashes

## Performance Notes

- Each subscriber runs in its own thread
- Messages are processed with 500ms simulated delay
- Multiple subscribers process in parallel
- Thread-safe with proper synchronization

## Next Steps for Interview

Be prepared to discuss:
- Why pull model vs push model?
- How does offset tracking work?
- What threading primitives are used?
- How would you add partitioning?
- How to handle consumer failure?
- Scaling strategies?

