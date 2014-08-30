cmake_minimum_required(VERSION 2.8)

project (yay)
set(CMAKE_BUILD_TYPE Release)

find_package(Boost COMPONENTS system python regex filesystem REQUIRED)
find_package(EXPAT REQUIRED)
find_package(PythonLibs REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wnon-virtual-dtor ")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")

MESSAGE("build type: ${CMAKE_BUILD_TYPE}")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
)

set(SOURCES
    src/yay_xml_util.cpp
   # src/yay_snowball.cpp
    src/yay_cmdproc.cpp
    src/yay_keymaps.cpp
    src/yay_logger.cpp
    src/yay_ngrams.cpp
    src/yay_shell.cpp
    src/yay_string_pool.cpp
    src/yay_translit_ru.cpp
    src/yay_utf8.cpp
    src/yay_util.cpp
    src/yay_util_time.cpp
    src/yay_geo.cpp)   

set(EXTRA_LIBS ${EXTRA_LIBS}
               ${Boost_SYSTEM_LIBRARY}
               ${Boost_FILESYSTEM_LIBRARY}
               ${Boost_REGEX_LIBRARY})

add_library(yay SHARED ${SOURCES})
add_library(yay_static STATIC ${SOURCES})
set_target_properties(yay_static PROPERTIES OUTPUT_NAME yay)

install(TARGETS yay yay_static
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
install(DIRECTORY include/yay DESTINATION include)