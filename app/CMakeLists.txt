cmake_minimum_required(VERSION 3.4.1)

set(APP_NAME kenan)


add_library( v8_base STATIC IMPORTED )
set_target_properties( v8_base PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libv8_base.a )

add_library( v8_snapshot STATIC IMPORTED )
set_target_properties( v8_snapshot PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libv8_snapshot.a )

add_library( png STATIC IMPORTED )
set_target_properties( png PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libpng.a )

file(GLOB kenan_engine2_cpp
    "src/main/cpp/kenan-engine2/*.h"
    "src/main/cpp/kenan-engine2/*.cpp")

file(GLOB kenan_graphics_cpp
    "src/main/cpp/kenan-graphics/*.h"
    "src/main/cpp/kenan-graphics/*.cpp")

file(GLOB kenan_system_cpp
    "src/main/cpp/kenan-system/*.h"
    "src/main/cpp/kenan-system/*.cpp")

file(GLOB kenan_platform_cpp
    "src/main/cpp/kenan-platform/*.h"
    "src/main/cpp/kenan-platform/*.cpp"
    "src/main/cpp/kenan-platform/loader/*.cpp"
    "src/main/cpp/kenan-platform/loader/*.h"
    "src/main/cpp/kenan-platform/controller/*.cpp"
    "src/main/cpp/kenan-platform/controller/*.h")

file(GLOB kenan_v8bindings_cpp
    "src/main/cpp/kenan-v8bindings/ObjectWrap.h"
    "src/main/cpp/kenan-v8bindings/ObjectWrap.cpp"
    "src/main/cpp/kenan-v8bindings/V8Main.h"
    "src/main/cpp/kenan-v8bindings/V8Main.cpp"
    "src/main/cpp/kenan-v8bindings/V8Log.h"
    "src/main/cpp/kenan-v8bindings/V8Log.cpp"
    "src/main/cpp/kenan-v8bindings/V8Orc2d.cpp"
    "src/main/cpp/kenan-v8bindings/V8Orc2d.h"
    "src/main/cpp/kenan-v8bindings/V8Sprite.cpp"
    "src/main/cpp/kenan-v8bindings/V8Sprite.h"
    "src/main/cpp/kenan-v8bindings/V8Image.cpp"
    "src/main/cpp/kenan-v8bindings/V8Image.h"
    "src/main/cpp/kenan-v8bindings/V8EventService.cpp"
    "src/main/cpp/kenan-v8bindings/V8EventService.h"
    "src/main/cpp/kenan-v8bindings/V8ResourceManager.cpp"
    "src/main/cpp/kenan-v8bindings/V8ResourceManager.h")

file(GLOB thirdparty_png_c
	src/main/cpp/thirdparty/libpng/png.c
	src/main/cpp/thirdparty/libpng/pngerror.c
	src/main/cpp/thirdparty/libpng/pnggccrd.c
	src/main/cpp/thirdparty/libpng/pngget.c
	src/main/cpp/thirdparty/libpng/pngmem.c
	src/main/cpp/thirdparty/libpng/pngpread.c
	src/main/cpp/thirdparty/libpng/pngread.c
	src/main/cpp/thirdparty/libpng/pngrio.c
	src/main/cpp/thirdparty/libpng/pngrtran.c
	src/main/cpp/thirdparty/libpng/pngrutil.c
	src/main/cpp/thirdparty/libpng/pngset.c
	src/main/cpp/thirdparty/libpng/pngtrans.c
	src/main/cpp/thirdparty/libpng/pngvcrd.c
	src/main/cpp/thirdparty/libpng/pngwio.c
	src/main/cpp/thirdparty/libpng/pngwrite.c
	src/main/cpp/thirdparty/libpng/pngwtran.c
	src/main/cpp/thirdparty/libpng/pngwutil.c
	"src/main/cpp/thirdparty/libpng/*.h"
	"src/main/cpp/thirdparty/libnhr/*.c"
	"src/main/cpp/thirdparty/libnhr/*.h"
	"src/main/cpp/thirdparty/httplib/*.h")

file(GLOB main_cpp
    src/main/cpp/mainjni.cpp)

add_library( ${APP_NAME} SHARED                              ${main_cpp}
                                                             ${kenan_engine2_cpp}
                                                             ${kenan_system_cpp}
                                                             ${kenan_graphics_cpp}
                                                             ${kenan_v8bindings_cpp}
                                                             ${thirdparty_png_c}
                                                             ${kenan_platform_cpp})

add_definitions( -DIN_ANDROID=1 -DUSE_SKIA_ANDROID=0)

target_include_directories( ${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/libs/include/v8
                                                ${CMAKE_SOURCE_DIR}/libs/include/libpng
                                                ${CMAKE_SOURCE_DIR}/src/main/cpp
                                                ${CMAKE_SOURCE_DIR}/src/main/cpp/kenan-engine2
                                                ${CMAKE_SOURCE_DIR}/src/main/cpp/kenan-system
                                                ${CMAKE_SOURCE_DIR}/src/main/cpp/kenan-graphics
                                                ${CMAKE_SOURCE_DIR}/src/main/cpp/kenan-platform
                                                ${CMAKE_SOURCE_DIR}/src/main/cpp/kenan-platform/loader
                                                ${CMAKE_SOURCE_DIR}/src/main/cpp/kenan-platform/controller
                                                ${CMAKE_SOURCE_DIR}/src/main/cpp/kenan-v8bindings
                                                ${CMAKE_SOURCE_DIR}/src/main/cpp/thirdparty)

target_link_libraries( ${APP_NAME} v8_base v8_snapshot log
                                    z
                                    c
                                    EGL
                                    GLESv2
                                    png)