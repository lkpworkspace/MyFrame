#include "MyHandleMgr.h"
#include "MyContext.h"
#include "assert.h"

#include <string.h>

#define MY_DEFAULT_SLOT_SIZE 4

MyHandleMgr::MyHandleMgr() :
    m_harbor(0),
    m_slot_size(MY_DEFAULT_SLOT_SIZE),
    m_slot_idx(0),
    m_ctx_count(0),
    m_handle_index(0)
{
    m_slot = (MyContext**)malloc(sizeof(MyContext*) * m_slot_size);
    memset(m_slot, 0, sizeof(MyContext*) * m_slot_size);
}

MyHandleMgr::~MyHandleMgr(){}

uint32_t MyHandleMgr::RegHandle(MyContext* ctx)
{
    for (;;) {
        int i;
        for (i = 0; i < m_slot_size; i++) {
            uint32_t handle = (i + m_handle_index) & MY_HANDLE_MASK;
            int hash = handle & (m_slot_size-1);
            if (m_slot[hash] == NULL) {
                m_slot[hash] = ctx;
                m_handle_index = handle + 1;

                handle |= m_harbor;
                ctx->m_handle = handle;
                m_ctx_count++;
                return handle;
            }
        }
        assert((m_slot_size * 2 - 1) <= MY_HANDLE_MASK);
        MyContext** new_slot = (MyContext**)malloc(m_slot_size * 2 * sizeof(MyContext*));
        memset(new_slot, 0, m_slot_size * 2 * sizeof(MyContext*));
        for (i = 0; i < m_slot_size; i++) {
            int hash = m_slot[i]->GetHandle() & (m_slot_size * 2 - 1);
            assert(new_slot[hash] == NULL);
            new_slot[hash] = m_slot[i];
        }
        free(m_slot);
        m_slot = new_slot;
        m_slot_size *= 2;
    }
    return 0;
}

MyContext* MyHandleMgr::GetContext(uint32_t handle)
{
    MyContext* result = nullptr;
    uint32_t hash = handle & (m_slot_size-1);
    MyContext* ctx = m_slot[hash];
    if (ctx && ctx->m_handle == handle) {
        result = ctx;
    }
    return result;
}

MyContext* MyHandleMgr::GetContext()
{
    if(m_msg_list.IsEmpty()) return nullptr;

    bool first = true;
    MyNode* b = m_msg_list.Begin();
    MyNode* ctx_node = nullptr; 
    MyContext* ctx = nullptr;

    while(!m_msg_list.IsEmpty()){
        ctx_node = m_msg_list.Begin();
        ctx = static_cast<MyContext*>(ctx_node);

        if(ctx->m_in_global == false){
            m_msg_list.DelHead();
            m_msg_list.AddTail(ctx_node);
        }else{
            m_msg_list.DelHead();

            ctx->m_in_msg_list = false;
            ctx->m_in_global = false;
            return ctx;
        }
        if((b == ctx_node) && !first){
            break;
        } 
        first = false;
    }
    return nullptr;
}

void MyHandleMgr::PushContext(MyContext* ctx)
{
    if(ctx->m_in_msg_list) return;
    ctx->m_in_msg_list = true;
    m_msg_list.AddTail(ctx);
}
