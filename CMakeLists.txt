cmake_minimum_required (VERSION 2.6)

project (server)

#set(CROSS "/home/kar/Work/Pi/toolchain/gcc-linaro-7.3.1-2018.05-x86_64_armv8l-linux-gnueabihf/bin/")
#
#set(CC      ${CROSS}/armv8l-linux-gnueabihf-g++)
#set(CXX     ${CROSS}/armv8l-linux-gnueabihf-gcc)
#

#add_compile_options(-mabi=aapcs-linux -mfpu=neon-vfpv4 -marm -mfloat-abi=hard -mcpu=cortex-a53)

set(SRC_CONN connection)
set(SRC_DATA data)
set(SRC_MATH math)
set(SRC_PROC process)
set(SRC_UTIL util)

include_directories(
  ${SRC_CONN}
  ${SRC_DATA}
  ${SRC_MATH}
  ${SRC_PROC}
  ${SRC_UTIL}
)

set(SOURCES
  ${SRC_CONN}/socket.cpp
  ${SRC_DATA}/frame.cpp
  ${SRC_DATA}/sync_queue.cpp
  ${SRC_MATH}/axis.cpp
  ${SRC_MATH}/quaternion.cpp
  ${SRC_PROC}/processer.cpp
  ${SRC_PROC}/writer.cpp
  main.cpp
)

add_executable(server ${SOURCES})
target_link_libraries(server pthread -lstdc++ -lm)
