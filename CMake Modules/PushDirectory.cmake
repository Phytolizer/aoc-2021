macro(_push_directory_preprocess_arguments)
  cmake_parse_arguments(
    PARSE_ARGV 0 "PUSH_DIRECTORY" "" "TARGET_NAME;OUTPUT_NAME"
    "SOURCE_FILES;INCLUDED_DIRECTORIES;LINKED_LIBRARIES"
  )
  if(PUSH_DIRECTORY_TARGET_NAME STREQUAL "")
    message(
      FATAL_ERROR
        "${PUSH_DIRECTORY_FUNCTION_NAME} requires a TARGET_NAME argument"
    )
  endif()
  if(PUSH_DIRECTORY_SOURCE_FILES STREQUAL "")
    message(
      FATAL_ERROR
        "${PUSH_DIRECTORY_FUNCTION_NAME} requires a SOURCE_FILES argument"
    )
  endif()
  list(TRANSFORM PUSH_DIRECTORY_SOURCE_FILES
       PREPEND "Source/${PUSH_DIRECTORY_TARGET_NAME}/"
  )
endmacro()

macro(_push_directory_apply_to_target TARGET_NAME)
  target_include_directories(
    "${TARGET_NAME}" PUBLIC "${PUSH_DIRECTORY_INCLUDED_DIRECTORIES}"
  )
  foreach(LIBRARY "${PUSH_DIRECTORY_LINKED_LIBRARIES}")
    target_link_libraries("${TARGET_NAME}" PUBLIC "${LIBRARY}")
  endforeach()
  if(PUSH_DIRECTORY_OUTPUT_NAME)
    set_target_properties(
      "${TARGET_NAME}" PROPERTIES OUTPUT_NAME "${PUSH_DIRECTORY_OUTPUT_NAME}"
    )
  endif()
endmacro()

function(push_directory_executable)
  set(PUSH_DIRECTORY_FUNCTION_NAME "push_directory_executable")
  _push_directory_preprocess_arguments()
  add_executable(
    "${PUSH_DIRECTORY_TARGET_NAME}" "${PUSH_DIRECTORY_SOURCE_FILES}"
  )
  _push_directory_apply_to_target("${PUSH_DIRECTORY_TARGET_NAME}")
endfunction()

function(push_directory_library)
  set(PUSH_DIRECTORY_FUNCTION_NAME "push_directory_library")
  _push_directory_preprocess_arguments()
  add_library(
    "${PUSH_DIRECTORY_TARGET_NAME}" SHARED "${PUSH_DIRECTORY_SOURCE_FILES}"
  )
  _push_directory_apply_to_target("${PUSH_DIRECTORY_TARGET_NAME}")
endfunction()
