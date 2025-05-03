#set(SRC_LIST userservice.cc ../user.pb.cc)
set(SRC_LIST friendservice.cc ../friend.pb.cc)
add_executable(prvider ${SRC_LIST})
target_link_libraries(prvider mprpc protobuf zookeeper_mt)