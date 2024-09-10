# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "tkdnn_ros: 2 messages, 0 services")

set(MSG_I_FLAGS "-Itkdnn_ros:/home/amov/catkin_ws/src/tkdnn-ros/msg;-Istd_msgs:/opt/ros/melodic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(tkdnn_ros_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/amov/catkin_ws/src/tkdnn-ros/msg/bboxes.msg" NAME_WE)
add_custom_target(_tkdnn_ros_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "tkdnn_ros" "/home/amov/catkin_ws/src/tkdnn-ros/msg/bboxes.msg" "tkdnn_ros/bbox:std_msgs/Header"
)

get_filename_component(_filename "/home/amov/catkin_ws/src/tkdnn-ros/msg/bbox.msg" NAME_WE)
add_custom_target(_tkdnn_ros_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "tkdnn_ros" "/home/amov/catkin_ws/src/tkdnn-ros/msg/bbox.msg" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(tkdnn_ros
  "/home/amov/catkin_ws/src/tkdnn-ros/msg/bboxes.msg"
  "${MSG_I_FLAGS}"
  "/home/amov/catkin_ws/src/tkdnn-ros/msg/bbox.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/tkdnn_ros
)
_generate_msg_cpp(tkdnn_ros
  "/home/amov/catkin_ws/src/tkdnn-ros/msg/bbox.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/tkdnn_ros
)

### Generating Services

### Generating Module File
_generate_module_cpp(tkdnn_ros
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/tkdnn_ros
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(tkdnn_ros_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(tkdnn_ros_generate_messages tkdnn_ros_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/amov/catkin_ws/src/tkdnn-ros/msg/bboxes.msg" NAME_WE)
add_dependencies(tkdnn_ros_generate_messages_cpp _tkdnn_ros_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amov/catkin_ws/src/tkdnn-ros/msg/bbox.msg" NAME_WE)
add_dependencies(tkdnn_ros_generate_messages_cpp _tkdnn_ros_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(tkdnn_ros_gencpp)
add_dependencies(tkdnn_ros_gencpp tkdnn_ros_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS tkdnn_ros_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(tkdnn_ros
  "/home/amov/catkin_ws/src/tkdnn-ros/msg/bboxes.msg"
  "${MSG_I_FLAGS}"
  "/home/amov/catkin_ws/src/tkdnn-ros/msg/bbox.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/tkdnn_ros
)
_generate_msg_eus(tkdnn_ros
  "/home/amov/catkin_ws/src/tkdnn-ros/msg/bbox.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/tkdnn_ros
)

### Generating Services

### Generating Module File
_generate_module_eus(tkdnn_ros
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/tkdnn_ros
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(tkdnn_ros_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(tkdnn_ros_generate_messages tkdnn_ros_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/amov/catkin_ws/src/tkdnn-ros/msg/bboxes.msg" NAME_WE)
add_dependencies(tkdnn_ros_generate_messages_eus _tkdnn_ros_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amov/catkin_ws/src/tkdnn-ros/msg/bbox.msg" NAME_WE)
add_dependencies(tkdnn_ros_generate_messages_eus _tkdnn_ros_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(tkdnn_ros_geneus)
add_dependencies(tkdnn_ros_geneus tkdnn_ros_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS tkdnn_ros_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(tkdnn_ros
  "/home/amov/catkin_ws/src/tkdnn-ros/msg/bboxes.msg"
  "${MSG_I_FLAGS}"
  "/home/amov/catkin_ws/src/tkdnn-ros/msg/bbox.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/tkdnn_ros
)
_generate_msg_lisp(tkdnn_ros
  "/home/amov/catkin_ws/src/tkdnn-ros/msg/bbox.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/tkdnn_ros
)

### Generating Services

### Generating Module File
_generate_module_lisp(tkdnn_ros
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/tkdnn_ros
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(tkdnn_ros_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(tkdnn_ros_generate_messages tkdnn_ros_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/amov/catkin_ws/src/tkdnn-ros/msg/bboxes.msg" NAME_WE)
add_dependencies(tkdnn_ros_generate_messages_lisp _tkdnn_ros_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amov/catkin_ws/src/tkdnn-ros/msg/bbox.msg" NAME_WE)
add_dependencies(tkdnn_ros_generate_messages_lisp _tkdnn_ros_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(tkdnn_ros_genlisp)
add_dependencies(tkdnn_ros_genlisp tkdnn_ros_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS tkdnn_ros_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(tkdnn_ros
  "/home/amov/catkin_ws/src/tkdnn-ros/msg/bboxes.msg"
  "${MSG_I_FLAGS}"
  "/home/amov/catkin_ws/src/tkdnn-ros/msg/bbox.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/tkdnn_ros
)
_generate_msg_nodejs(tkdnn_ros
  "/home/amov/catkin_ws/src/tkdnn-ros/msg/bbox.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/tkdnn_ros
)

### Generating Services

### Generating Module File
_generate_module_nodejs(tkdnn_ros
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/tkdnn_ros
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(tkdnn_ros_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(tkdnn_ros_generate_messages tkdnn_ros_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/amov/catkin_ws/src/tkdnn-ros/msg/bboxes.msg" NAME_WE)
add_dependencies(tkdnn_ros_generate_messages_nodejs _tkdnn_ros_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amov/catkin_ws/src/tkdnn-ros/msg/bbox.msg" NAME_WE)
add_dependencies(tkdnn_ros_generate_messages_nodejs _tkdnn_ros_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(tkdnn_ros_gennodejs)
add_dependencies(tkdnn_ros_gennodejs tkdnn_ros_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS tkdnn_ros_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(tkdnn_ros
  "/home/amov/catkin_ws/src/tkdnn-ros/msg/bboxes.msg"
  "${MSG_I_FLAGS}"
  "/home/amov/catkin_ws/src/tkdnn-ros/msg/bbox.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/tkdnn_ros
)
_generate_msg_py(tkdnn_ros
  "/home/amov/catkin_ws/src/tkdnn-ros/msg/bbox.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/tkdnn_ros
)

### Generating Services

### Generating Module File
_generate_module_py(tkdnn_ros
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/tkdnn_ros
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(tkdnn_ros_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(tkdnn_ros_generate_messages tkdnn_ros_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/amov/catkin_ws/src/tkdnn-ros/msg/bboxes.msg" NAME_WE)
add_dependencies(tkdnn_ros_generate_messages_py _tkdnn_ros_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/amov/catkin_ws/src/tkdnn-ros/msg/bbox.msg" NAME_WE)
add_dependencies(tkdnn_ros_generate_messages_py _tkdnn_ros_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(tkdnn_ros_genpy)
add_dependencies(tkdnn_ros_genpy tkdnn_ros_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS tkdnn_ros_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/tkdnn_ros)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/tkdnn_ros
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(tkdnn_ros_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/tkdnn_ros)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/tkdnn_ros
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(tkdnn_ros_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/tkdnn_ros)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/tkdnn_ros
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(tkdnn_ros_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/tkdnn_ros)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/tkdnn_ros
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(tkdnn_ros_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/tkdnn_ros)
  install(CODE "execute_process(COMMAND \"/usr/bin/python2\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/tkdnn_ros\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/tkdnn_ros
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(tkdnn_ros_generate_messages_py std_msgs_generate_messages_py)
endif()
