# Layers of Architecture
```
        Javascript game code / resource
------------------------------------------------
            Javascript Game Runtime
------------------------------------------------
   Android App GLView | Gnome Window | Windows
------------------------------------------------
 **Kenan** | **V8bindings** | Third-party Libs | V8 Lib
------------------------------------------------
            System Adaptation Layer
------------------------------------------------
        System Impl(Android/Linux/Windows)
```

## Kenan modules
```
kenan-engine2: 2d graphic speed engine based on openGLES
kenan-graphics: Image(pixel) operations
kenan-system: Log, Timer, IO, Network
kenan-media: Audio, Video, Sound
```

## v8bindings


# C++ layer Modules list(TODO)
## Script
## ResourcePool
## Timer
## Sound
## Animation
## ImageFileLoader

# Refactor(TODO)
## V8Orc2d
## Image
## V8Image
## V8EventService