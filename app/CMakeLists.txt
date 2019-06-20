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

file(GLOB kenan_v8bindings_cpp
    "src/main/cpp/kenan-v8bindings/*.h"
    "src/main/cpp/kenan-v8bindings/V8Image.cpp")

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
	"src/main/cpp/thirdparty/libpng/*.h")

add_library( ${APP_NAME} SHARED src/main/cpp/native-lib.cpp  ${kenan_engine2_cpp}
                                                             ${kenan_system_cpp}
                                                             ${kenan_graphics_cpp}
                                                             ${thirdparty_png_c})

target_include_directories( ${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/libs/include/v8
                                                ${CMAKE_SOURCE_DIR}/libs/include/libpng
                                                ${CMAKE_SOURCE_DIR}/src/main/cpp
                                                ${CMAKE_SOURCE_DIR}/src/main/cpp/kenan-engine2
                                                ${CMAKE_SOURCE_DIR}/src/main/cpp/kenan-system
                                                ${CMAKE_SOURCE_DIR}/src/main/cpp/kenan-graphics)

target_link_libraries( ${APP_NAME} v8_base v8_snapshot log
                                    z
                                    c
                                    EGL
                                    GLESv2
                                    png)