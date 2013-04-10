#ifndef NETMSGDEF_H
#define NETMSGDEF_H

enum MSGID
{
	MSG_S_REQ_USER_LOGIN, //server请求用户登陆
	MSG_S_WAIT_USER_TALK,

	MSG_C_REQ_LOGIN,//用户请求登陆
	MSG_C_TALK
};

#define NET_VERSION 0xFFFF

//定义网络层消息头
typedef struct _NetMsgHead
{
	//消息头标识
	int Version;
	//消息id
	int MessageID;
	//包体长度
	int DataLen;
}NetMsgHead;
#endif
