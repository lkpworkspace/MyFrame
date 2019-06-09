#include "MyModule.h"
#include "MyFrame.h"
#include "MyContext.h"
#include <iostream>
#include <unistd.h>
#include <string.h>

void PrintSockMsg(struct my_sock_msg* msg)
{
    printf("type: %d\n",msg->type);
    printf("id: %d\n",msg->id);
    printf("size: %d\n",msg->ud);
    printf("data: %.*s\n",msg->ud, msg->buffer);
    fflush(stdout);
}

class MyTCPSockTest : public MyModule
{
public:
    MyTCPSockTest(){}
    virtual ~MyTCPSockTest(){}

    virtual int Init(MyContext* c, const char* param) override
    {
        std::cout << "MyTCPSockTest init" << std::endl;
        my_callback(c, CB, nullptr);
        my_listen(c, "127.0.0.1", 8888, 0);
        return 0;
    }

    static int CB(MyContext* context, void *ud, int type, int session, uint32_t source , const void *msg, size_t sz)
    {
        std::cout << m_srv << " from " << source << " to " << my_handle(context) << ": size " << sz << ", " << m_count++ << std::endl;
        struct my_sock_msg* sock_msg = (struct my_sock_msg*)msg;
        PrintSockMsg(sock_msg);
        free(sock_msg->buffer);
        return 0;
    }

    static int m_count;
    static std::string m_srv;
};

int MyTCPSockTest::m_count = 0;
std::string MyTCPSockTest::m_srv = "MyTCPSockTest: ";

extern "C" MyModule* my_mod_create()
{
    return static_cast<MyModule*>(new MyTCPSockTest());
}

extern "C" void my_mod_destory(MyModule* m)
{
    delete m;
}
