cmake_minimum_required(VERSION 3.7.2)

project(Hello C) # create a new C project called 'init' 


add_executable(Hello src/main.c) # add files to our project. Paths are relative to this file.

# should it be just platsupport - fails with both but in different ways?
target_link_libraries(Hello sel4 muslc utils platsupport sel4muslcsys sel4platsupport sel4utils sel4debug sel4allocman)


# Set this image as the rootserver
DeclareRootserver(Hello)

