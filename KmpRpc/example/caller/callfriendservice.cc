#include <iostream>
#include "mprpcapplication.h"
#include "friend.pb.h"

int main(int argc, char **argv)
{
    // 整个程序启动以后,想使用mprpc框架来享受rpc服务调用,一定需要先调用框架的初始化函数(只初始化一次)
    MprpcApplication::Init(argc, argv);

    fixbug::FriendServicerRpc_Stub stub(new MprpcChannel());

    fixbug::GetFriendsListRequest request;
    fixbug::GetFriendsListResponse response;

    request.set_userid(1);

    MprpcController controller;
    stub.GetFriendsList(&controller, &request, &response, nullptr);

    // 一次rpc调用完成, 读调用结果
    if (controller.Failed())
    {
        std::cout << controller.ErrorText() << std::endl;
    }
    else
    {
        if (0 == response.result().errcode())
        {
            std::cout << "GetFriendsList success:" << std::endl;
            int size = response.friends_size();
            for (int i = 0; i < size; i++)
            {
                std::cout << "index:" << i << " name:" << response.friends(i) << std::endl;
            }
        }
        else
        {
            std::cout << "GetFriendsList response error" << response.result().errmsg() << std::endl;
        }
    }
}
