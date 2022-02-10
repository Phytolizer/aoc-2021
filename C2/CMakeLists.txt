set(ADVENT_SOURCES "Core.c" "Files.c")
list(TRANSFORM ADVENT_SOURCES PREPEND "Advent/Source/")
add_library(AdventC2 ${ADVENT_SOURCES})
target_include_directories(AdventC2 PUBLIC "Advent/Include")

function(advent_day DAY)
  add_executable("AdventC2_${DAY}" "Days/Source/${DAY}.c")
  set(DAY_DIR "${CMAKE_CURRENT_LIST_DIR}/Days/Source/${DAY}")
  configure_file(Config.h.cin "${PROJECT_BINARY_DIR}/Include/${DAY}/Config.h")
  target_link_libraries("AdventC2_${DAY}" PRIVATE AdventC2)
  target_include_directories(
    "AdventC2_${DAY}" PRIVATE "${PROJECT_BINARY_DIR}/Include/${DAY}"
  )
endfunction()

advent_day(Day01)
