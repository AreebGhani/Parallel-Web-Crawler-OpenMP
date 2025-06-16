cmake_minimum_required(VERSION 3.31.6)
project(WebCrawlerApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets REQUIRED COMPONENTS core base)
find_package(CURL REQUIRED)
find_package(unofficial-gumbo CONFIG REQUIRED)

include(${wxWidgets_USE_FILE})

if(WIN32)
    set(GUI_TYPE WIN32)
else()
    set(GUI_TYPE "")
endif()

add_executable(WebCrawlerApp ${GUI_TYPE}
    lib/main.cpp
    lib/fetch.cpp
    lib/parser.cpp
)

target_link_libraries(WebCrawlerApp
    ${wxWidgets_LIBRARIES}
    CURL::libcurl
    unofficial::gumbo::gumbo
)
