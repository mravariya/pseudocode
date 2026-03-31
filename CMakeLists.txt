cmake_minimum_required(VERSION 3.16)
project(PseudocodeInterpreter VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(MSVC)
  add_compile_options(/W4 /utf-8)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(PC_CORE
  src/pc_common.c
  src/pc_token.c
  src/pc_lexer.c
  src/pc_error.c
  src/pc_ast.c
  src/pc_parser.c
  src/pc_value.c
  src/pc_interp.c
  src/pc_stdlib.c
  src/pc_pkg_shared.c
)

add_executable(pseudocode src/main.c ${PC_CORE} src/pc_repl.c src/pc_welcome.c)
target_include_directories(pseudocode PRIVATE src)

add_executable(pkg src/pkg_main.c ${PC_CORE})
target_include_directories(pkg PRIVATE src)
target_compile_definitions(pkg PRIVATE PC_BUILD_PKG=1)

# Default: embed source-tree path (./build/pseudocode works without cmake --install).
# ON for Homebrew/distro packages: embed CMAKE_INSTALL_PREFIX so Cellar/prefix layout finds stdlib-packages.
option(PC_EMBED_INSTALL_STDLIB "Embed stdlib path under CMAKE_INSTALL_PREFIX (for cmake --install / packaging)" OFF)
set(PC_STDLIB_PACKAGES_DIR "${CMAKE_SOURCE_DIR}/stdlib-packages")
if(EXISTS "${PC_STDLIB_PACKAGES_DIR}")
  if(PC_EMBED_INSTALL_STDLIB)
    set(PC_STDLIB_EMBED_PATH "${CMAKE_INSTALL_PREFIX}/share/pseudocode/stdlib-packages")
  else()
    set(PC_STDLIB_EMBED_PATH "${PC_STDLIB_PACKAGES_DIR}")
  endif()
  target_compile_definitions(pseudocode PRIVATE "PC_BUILTIN_STDLIB_PACKAGES=\"${PC_STDLIB_EMBED_PATH}\"")
  target_compile_definitions(pkg PRIVATE "PC_BUILTIN_STDLIB_PACKAGES=\"${PC_STDLIB_EMBED_PATH}\"")
endif()

if(UNIX)
  target_link_libraries(pseudocode PRIVATE m)
  target_link_libraries(pkg PRIVATE m)
endif()

install(DIRECTORY stdlib-packages DESTINATION share/pseudocode)

if(UNIX)
  find_library(READLINE_LIB readline)
  if(READLINE_LIB)
    target_compile_definitions(pseudocode PRIVATE PC_HAVE_READLINE=1)
    target_link_libraries(pseudocode PRIVATE ${READLINE_LIB})
    if(APPLE)
      find_library(NCURSES_LIB ncurses REQUIRED)
      target_link_libraries(pseudocode PRIVATE ${NCURSES_LIB})
    endif()
  endif()
endif()

install(TARGETS pseudocode pkg RUNTIME DESTINATION bin)
