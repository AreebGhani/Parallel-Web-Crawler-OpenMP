cmake_minimum_required(VERSION 3.31.6)
project(WebCrawlerApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------- Platform-Independent OpenMP Setup --------
find_package(OpenMP)

# Manually handle OpenMP for macOS (Apple Clang doesn't support OpenMP by default)
if(APPLE AND NOT OpenMP_FOUND)
    message(STATUS "Configuring OpenMP manually for macOS")
    set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include")
    set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include")
    set(OpenMP_C_LIB_NAMES "omp")
    set(OpenMP_CXX_LIB_NAMES "omp")
    set(OpenMP_omp_LIBRARY /usr/local/opt/libomp/lib/libomp.dylib)

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/usr/local/opt/libomp/lib -lomp")
endif()

# -------- Find Dependencies --------
find_package(wxWidgets REQUIRED COMPONENTS core base)
find_package(CURL REQUIRED)
find_package(unofficial-gumbo CONFIG REQUIRED)

include(${wxWidgets_USE_FILE})

# -------- WIN32 flag for Windows --------
if(WIN32)
    set(GUI_TYPE WIN32)
else()
    set(GUI_TYPE "")
endif()

# -------- Add Executable --------
add_executable(WebCrawlerApp ${GUI_TYPE}
    src/main.cpp
    src/fetch.cpp
    src/parser.cpp
    src/assets/logo.xpm
)

# -------- Link Libraries --------
target_link_libraries(WebCrawlerApp
    ${wxWidgets_LIBRARIES}
    CURL::libcurl
    unofficial::gumbo::gumbo
)

# -------- Add OpenMP Link If Found --------
if(OpenMP_FOUND)
    target_link_libraries(WebCrawlerApp OpenMP::OpenMP_CXX)
endif()
