#ifndef NETMSGDEF_H
#define NETMSGDEF_H

enum MSGID
{
	MSG_S_REQ_USER_LOGIN, //server�����û���½
	MSG_S_WAIT_USER_TALK,

	MSG_C_REQ_LOGIN,//�û������½
	MSG_C_TALK
};

#define NET_VERSION 0xFFFF

//�����������Ϣͷ
typedef struct _NetMsgHead
{
	//��Ϣͷ��ʶ
	int Version;
	//��Ϣid
	int MessageID;
	//���峤��
	int DataLen;
}NetMsgHead;
#endif
