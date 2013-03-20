
set( EXT_LIBS boost_unit_test_framework boost_test_frame)

file(GLOB_RECURSE SOURCE "*.cpp")

set( TARGET_NAME boost_utf_dependency_tests)

add_executable( ${TARGET_NAME} ${SOURCE} ${HEADERS})
target_link_libraries( ${TARGET_NAME} ${EXT_LIBS})
