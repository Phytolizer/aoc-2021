function(advent ADVENT_DAY)
  set(DAY "Day${ADVENT_DAY}")
  string(REGEX REPLACE "^0+" "" ADVENT_DAY "${ADVENT_DAY}")
  cmake_parse_arguments(PARSE_ARGV 0 "ADVENT" "" "" "EXTRA_SOURCES")
  set(ADVENT_SOURCES "Main.c;${EXTRA_SOURCES}")
  push_directory_executable(
    TARGET_NAME "${DAY}"
    OUTPUT_NAME "${DAY}.exe"
    SOURCE_FILES "${ADVENT_SOURCES}"
    INCLUDED_DIRECTORIES "${CMAKE_CURRENT_BINARY_DIR}/${DAY}"
    LINKED_LIBRARIES "Advent"
  )
  configure_file(
    "DayConfig.h.cin" "${CMAKE_CURRENT_BINARY_DIR}/${DAY}/DayConfig.h"
  )
endfunction()

push_directory_library(
  TARGET_NAME "Advent"
  SOURCE_FILES "Advent.c"
  INCLUDED_DIRECTORIES "Include" "${CMAKE_CURRENT_BINARY_DIR}"
)
configure_file(
  "AdventConfig.h.cin" "${CMAKE_CURRENT_BINARY_DIR}/AdventConfig.h"
)
advent("01")
