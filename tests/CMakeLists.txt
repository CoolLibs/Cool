add_subdirectory(Cool)
target_link_libraries(${PROJECT_NAME} PRIVATE Cool::Core)
cool_setup(${PROJECT_NAME})