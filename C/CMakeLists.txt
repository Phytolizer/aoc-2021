push_directory_library(
  TARGET_NAME "Advent" SOURCE_FILES "Advent.c" INCLUDED_DIRECTORIES "Include"
)
push_directory_executable(
  TARGET_NAME "Day01"
  SOURCE_FILES "Main.c"
  LINKED_LIBRARIES "Advent"
)
