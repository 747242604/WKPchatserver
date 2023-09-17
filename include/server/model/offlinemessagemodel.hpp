#ifndef OFFLINEMESSAGEMODEL_H
#define OFFLINEMESSAGEMODEL_H
#include <string>
#include <vector>
using namespace std;
class offlineMsgModel
{
    public:
        //存储离线消息表的操作接口方法
        void insert(int userid, string msg);
        //删除用户的离线消息
        void remove(int userid);
        //查询用户的离线消息
        vector<string> query(int userid);

    private:
        /* data */
};

#endif