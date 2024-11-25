set(LIB_NAME games_utils)

set(base_cpp_regexes *.cpp *.cxx *.c *.cc)
set(base_hpp_regexes *.hpp *.hxx *.h *.hh)
list(TRANSFORM base_cpp_regexes PREPEND "./" OUTPUT_VARIABLE src_regexes)
list(TRANSFORM base_hpp_regexes PREPEND "./" OUTPUT_VARIABLE inc_regexes)

file(GLOB src CONFIGURE_DEPENDS ${src_regexes})
file(GLOB inc CONFIGURE_DEPENDS ${inc_regexes})

add_library(${LIB_NAME} ${LIB_TYPE} ${src} ${inc})
target_include_directories(${LIB_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
