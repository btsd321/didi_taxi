get_filename_component(RAPIDJSON_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(RAPIDJSON_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/../include")
message(STATUS "RapidJSON found. Headers: ${RAPIDJSON_INCLUDE_DIRS}")
