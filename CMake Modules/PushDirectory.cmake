function(push_directory_executable)
  cmake_parse_arguments(
    PARSE_ARGV 0 "PUSH_DIRECTORY" "" "TARGET_NAME"
    "SOURCE_FILES;INCLUDED_DIRECTORIES;LINKED_LIBRARIES"
  )
  list(TRANSFORM PUSH_DIRECTORY_SOURCE_FILES
       PREPEND "Source/${PUSH_DIRECTORY_TARGET_NAME}/"
  )
  add_executable(
    "${PUSH_DIRECTORY_TARGET_NAME}" "${PUSH_DIRECTORY_SOURCE_FILES}"
  )
  target_include_directories(
    "${PUSH_DIRECTORY_TARGET_NAME}"
    PRIVATE "${PUSH_DIRECTORY_INCLUDED_DIRECTORIES}"
  )
  target_link_libraries(
    "${PUSH_DIRECTORY_TARGET_NAME}"
    PRIVATE "${PUSH_DIRECTORY_LINKED_LIBRARIES}"
  )
endfunction(push_directory_executable)
