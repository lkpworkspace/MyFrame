#include "MyFrame.h"
#include "MyApp.h"
#include "MyContext.h"

int my_send(MyContext* ctx,
            uint32_t source,
            uint32_t destination ,
            int type, /* 消息类型 */
            int session,
            void * msg, /* 消息内容 */
            size_t sz /* 消息长度 */)
{
    return ctx->SendMsg(source, destination, type, session, msg, sz);
}

void my_callback(MyContext* ctx, my_cb cb, void* ud)
{
    ctx->SetCB(cb, ud);
}

uint32_t my_handle(MyContext* ctx)
{
    return ctx->GetHandle();
}

