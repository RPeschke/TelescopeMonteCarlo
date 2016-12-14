FIND_PACKAGE( ROOT REQUIRED)
set(name "Telescope_mc1")




aux_source_directory(src CORE_SRC)


SET(MY_INCLUDE_DIRECTORIES ./main/inc ./inc ../../main/inc ${ROOT_INCLUDE_DIR} )
INCLUDE_DIRECTORIES(  ${MY_INCLUDE_DIRECTORIES} ${ROOT_INCLUDE_DIR})





add_executable( ${name}  ${CORE_SRC}  )



SOURCE_GROUP(Headers  inc/*.h)

target_link_libraries( ${name}  ${ROOT_LIBRARIES} ${ADDITIONAL_LIBRARIES})


INSTALL(TARGETS ${name}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
