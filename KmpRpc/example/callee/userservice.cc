#include <iostream>
#include <string>
#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

/*
UserService原来是一个本地服务, 提供了两个进程内的本地方法,Login和GetFriendLists
*/
class UserService : public fixbug::UserServicerRpc // 使用在rpc服务发布端(rpc服务提供者)
{
public:
    bool Login(std::string name, std::string pwd)
    {
        std::cout << "doing local service Login" << std::endl;
        std::cout << " name: " << name << " pwd: " << pwd << std::endl;
        return true;
    }

    bool Register(uint32_t id, std::string name, std::string pwd)
    {
        std::cout << "doing local service Login" << std::endl;
        std::cout << " id: " << id << " name: " << name << " pwd: " << pwd << std::endl;
        return true;
    }

    /*
    //重写基类UserServiceRpc的虚函数
    1. caller ===> Login(LoginRequest)  => muduo => callee
    2. callee ===> Login(LoginRequest)  => 交到下面重写的这个Login方法上了
    */
    void Login(::google::protobuf::RpcController *controller,
               const ::fixbug::LoginRequest *request,
               ::fixbug::LoginRespose *response,
               ::google::protobuf::Closure *done)
    {
        // 框架给业务上报了请求参数LoginRequest , 应用获取相应数据做本地服务
        std::string name = request->name();
        std::string pwd = request->pwd();

        // 做本地业务
        bool login_reslut = Login(name, pwd);

        // 把响应写入 包括错误码 错误消息  返回值
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("Login do error!");
        response->set_sucess(login_reslut);

        // 执行回调操作  执行响应对象数据的序列化和网络发送(都是由框架来完成的)
        done->Run();
    }

    void Register(google::protobuf::RpcController *controller,
                  const ::fixbug::RegisterRequest *request,
                  ::fixbug::RegisterRespose *response,
                  ::google::protobuf::Closure *done)
    {
        uint32_t id = request->id();
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool Register_reslut = Register(id, name, pwd);

        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("Register do error!");
        response->set_sucess(Register_reslut);

        done->Run();
    }
};

int main(int argc, char **argv)
{
    // 调用框架的初始化操作
    MprpcApplication::Init(argc, argv);

    // provider是一个rpc网络服务对象,把Userservice对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new UserService());

    // 启动一个rpc服务发布节点 Run以后,进程进入阻塞状态,等待远程rpc调用请求
    provider.Run();

    return 0;
}