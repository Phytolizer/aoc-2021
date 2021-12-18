function(advent ADVENT_DAY)
  set(DAY "Day${ADVENT_DAY}")
  string(REGEX REPLACE "^0+" "" ADVENT_DAY "${ADVENT_DAY}")
  cmake_parse_arguments(
    PARSE_ARGV 0 "ADVENT" "" "" "EXTRA_SOURCES;EXTRA_LIBRARIES"
  )
  set(ADVENT_SOURCES "Main.c;${ADVENT_EXTRA_SOURCES}")
  set(ADVENT_LIBRARIES "Advent;icuuc;icuio;${ADVENT_EXTRA_LIBRARIES}")
  push_directory_executable(
    TARGET_NAME "${DAY}" OUTPUT_NAME "${DAY}.exe"
    SOURCE_FILES ${ADVENT_SOURCES}
    INCLUDED_DIRECTORIES "${CMAKE_CURRENT_BINARY_DIR}/${DAY}"
    LINKED_LIBRARIES ${ADVENT_LIBRARIES}
  )
  add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${DAY}/Input.c"
           "${CMAKE_CURRENT_BINARY_DIR}/${DAY}/Input.h"
    DEPENDS "${CMAKE_CURRENT_LIST_DIR}/Source/${DAY}/Input.txt"
    COMMAND
      "bin2c" "-n" "inputText" "-d"
      "${CMAKE_CURRENT_BINARY_DIR}/${DAY}/Input.h" "-o"
      "${CMAKE_CURRENT_BINARY_DIR}/${DAY}/Input.c"
      "${CMAKE_CURRENT_LIST_DIR}/Source/${DAY}/Input.txt"
  )
  target_sources("${DAY}" PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/${DAY}/Input.c")
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
advent("02")
advent("03" EXTRA_LIBRARIES "openblas")
