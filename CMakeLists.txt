cmake_minimum_required(VERSION 3.16)

project(didi_taxi VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)

set(QT_INSTALL_DIR D:/Software/Library/Qt/6.8.3/msvc2022_64)

set(CMAKE_PREFIX_PATH
    ${CMAKE_SOURCE_DIR}/thirdparty/QsLog/cmake
    ${CMAKE_SOURCE_DIR}/thirdparty/RapidJSON/cmake
    ${QT_INSTALL_DIR}/lib/cmake/Qt6
)
find_package(QsLog REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(RapidJSON REQUIRED)

set(THIRD_PARTY_DIR thirdparty)
set(THIRD_PARTY_INCLUDE_DIR 
    ${QsLog_INCLUDE_DIRS}
    ${RAPIDJSON_INCLUDE_DIRS}
)
set(THIRD_PARTY_LIBS 
    ${QsLog_LIBRARIES}
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
)

set(SRC_FILE_PATH ${CMAKE_SOURCE_DIR}/source/src)
set(UI_FILE_PATH ${CMAKE_SOURCE_DIR}/ui)
set(INCLUDE_FILE_PATH ${CMAKE_SOURCE_DIR}/source/include)

list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS ${UI_FILE_PATH})

file(GLOB SRC_FILES ${SRC_FILE_PATH}/*.cpp)
file(GLOB UI_FILES ${UI_FILE_PATH}/*.ui)
file(GLOB INCLUDE_FILES ${INCLUDE_FILE_PATH}/*.h)
file(GLOB QRC_FILES ${CMAKE_SOURCE_DIR}/res/*.qrc)

qt6_add_resources(qrc_FILES ${QRC_FILES})

set(PROJECT_SOURCES
    ${SRC_FILES}
    ${INCLUDE_FILES}
    ${UI_FILES}
    ${qrc_FILES}
)

# 打印PROJECT_SOURCES
message(STATUS "PROJECT_SOURCES: ${PROJECT_SOURCES}")

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${THIRD_PARTY_LIBS}
)
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${INCLUDE_FILE_PATH}
    ${THIRD_PARTY_INCLUDE_DIR}
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${QT_INSTALL_DIR}/plugins/platforms
    ${CMAKE_BINARY_DIR}/platforms
    COMMENT "Copying qt platforms folder to output directory"
)


