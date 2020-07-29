# Interfaces
kenan_api_graphics.image_create
kenan_api_graphics.image_destroy
kenan_api_graphics.image_set_src
kenan_api_graphics.image_get_src
kenan_api_graphics.image_get_width
kenan_api_graphics.image_get_height
kenan_api_graphics.image_set_image_data
kenan_api_graphics.image_get_image_data
kenan_api_graphics.image_get_texture
kenan_api_script.run
kenan_api_script.include
kenan_api_task.start
kenan_api_task.stop
kenan_api_task.has
kenan_api_task.send_message
kenan_api_task.broadcast_message

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

# Threads
```
|  Non-JS                                    |
|  Resource Operation Threads Pool(Fixed)    |
|-----------------------------------------   |
                                             |
|  JS Context-1-N                            |
|  Runtime Threads(Creatable)               | <<==>> Structure Sharing Interface
|-----------------------------------------   |
                                             |
|  JS Context-0                              |
|  Engine Thread(Main)                     |
|-----------------------------------------   |

```


## Kenan modules
```
kenan-engine2: 2d graphic speed engine based on openGLES
kenan-graphics: Image(pixel) operations
kenan-system: Log, Timer, IO, Network
kenan-runtime: v8bindings and runtime apis
```

# C++ layer Modules list(TODO)
## Script             [OK, Lack of HTTP]
## ResourcePool
## Timer
## Sound
## Animation
## ImageFileLoader    [OK, Lack of HTTP]

# Refactor(TODO)
## V8Kenan2d
## Image
## V8Image
## V8EventService