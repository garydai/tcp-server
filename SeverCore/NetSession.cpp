#include "NetSession.h"


using namespace std;
std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}
//要有个延时时间来传输数据
CNetSession::CNetSession(boost::asio::io_service& io_service)
: m_Socket(io_service)
{
	memset(m_RecvBuf, 0, sizeof(m_RecvBuf));
	memset(m_SendBuf, 0, sizeof(m_SendBuf));
	m_bWaitForMsgHead = true;
	m_pData = NULL;
}

CNetSession::~CNetSession()
{

}

void CNetSession::HandleShandshake(const boost::system::error_code& error)
{
	if (!error)
	{
		cout<<"握手成功"<<endl;
		cout<<"发送消息->握手成功，请求用户登陆"<<endl;
		NetMsgHead MsgHead;
		wstring wszContent = L"消息内容->请求用户登陆";
		SetMsgToBuf(MSG_S_REQ_USER_LOGIN, wszContent);

	    boost::asio::async_write(m_Socket, boost::asio::buffer(m_SendBuf, BUFF_SIZE),
		   boost::bind(&CNetSession::OnSendOk, shared_from_this(),
		    boost::asio::placeholders::error,
		    boost::asio::placeholders::bytes_transferred));

	}
	else
	{
		cout<<"握手失败"<<endl;
	}
}

void CNetSession::Start()
{
  // m_Socket.async_handshake(boost::asio::ssl::stream_base::server,
   //    boost::bind(&CNetSession::HandleShandshake, shared_from_this(),
  //        boost::asio::placeholders::error));
/*	SetMsgToBuf(MSG_S_WAIT_USER_TALK, L"等你发消息呀");
	cout<<"向客户端发消息：等你发消息呀"<<endl;
	boost::asio::async_write(m_Socket, boost::asio::buffer(m_SendBuf, BUFF_SIZE),
		boost::bind(&CNetSession::OnSendOk, shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
		*/
		memset(m_RecvBuf, 0, sizeof(m_RecvBuf));
		m_Socket.async_read_some(boost::asio::buffer(m_RecvBuf, BUFF_SIZE),
			boost::bind(&CNetSession::OnRecvOk, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));


}

void CNetSession::OnSendOk(const boost::system::error_code& error,
    size_t /*bytes_transferred*/)
{
	if(error) //发送失败,重传
	{
		cout<<"发送失败"<<endl;
		cout<<error.message() << std::endl;
	}
	else
	{
		cout<<"发送成功"<<endl;
		memset(m_RecvBuf, 0, sizeof(m_RecvBuf));
		m_Socket.async_read_some(boost::asio::buffer(m_RecvBuf, BUFF_SIZE),
			boost::bind(&CNetSession::OnRecvOk, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));


	}

}

void CNetSession::OnRecvOk(const boost::system::error_code& error,
    size_t bytes_transferred)
{
	if(error) //接收失败
	{
		cout<<"接收失败"<<endl;
		cout<<error.value()<< std::endl;
	//	cout<<error.category()<< std::endl;
		//cout<<error.what()<< std::endl;
		Stop();
	}
	else
	{
		cout<<"接收成功"<<endl;
	//	HandleMsg(m_RecvBuf);
	//	memset(m_RecvBuf, 0, sizeof(m_RecvBuf));	
	//	m_Socket.async_read_some(boost::asio::buffer(m_RecvBuf,BUFF_SIZE),
	//	boost::bind(&CNetSession::OnRecvOk, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	//	return;
		if(m_bWaitForMsgHead)//等消息头
		{
			if(bytes_transferred >= sizeof(NetMsgHead))//消息头接收成功
			{	
				NetMsgHead* pHead = (NetMsgHead*)m_RecvBuf;
				if(bytes_transferred >= sizeof(NetMsgHead) + pHead->DataLen)//所有数据都收到
				{
					m_bWaitForMsgHead = true;//继续等下一个包头
					m_pDataBegin = m_RecvBuf;
					HandleMsg(m_RecvBuf);

					return ;
				}
				else
				{
					//包体部分未全部到达
					int TotalLen = pHead->DataLen + sizeof(NetMsgHead);
					m_pData = new char[TotalLen];
					memset(m_pData, 0, TotalLen);
					memcpy(m_pData, m_RecvBuf, bytes_transferred);
					m_pDataBegin = m_pData + bytes_transferred;
					m_WaitLen = pHead->DataLen - (bytes_transferred - sizeof(NetMsgHead));
					m_bWaitForMsgHead = false;

				}
			}
			else//消息头接收失败
			{

			}
		}
		else//等包体
		{
			memcpy(m_pDataBegin, m_RecvBuf, min(bytes_transferred, m_WaitLen));
			//数据还未全部到达
			if (bytes_transferred < m_WaitLen)
			{
				m_pDataBegin += bytes_transferred;
				m_WaitLen -= bytes_transferred;
			}
			else
			{
				//所有的数据都已收到
				//S3_LOG_DEBUG(m_logger,L"All data is received, [Session:" << m_SessionID << "]");
				HandleMsg(m_pData);
				delete[] m_pData;
				return ;
				m_pData = NULL;
				m_bWaitForMsgHead = true;//继续等下一个数据包
				m_pDataBegin = NULL;
				m_WaitLen =0;

			}
		}
		memset(m_RecvBuf, 0, sizeof(m_RecvBuf));	
		m_Socket.async_read_some(boost::asio::buffer(m_RecvBuf,BUFF_SIZE),
		boost::bind(&CNetSession::OnRecvOk, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));


	}

}

void CNetSession::Stop()
{
	socket().close();
}



void CNetSession::RspLogIn(char* pData)
{
	wstring wszContent = (TCHAR*)(sizeof(NetMsgHead) + pData);
	cout<<"接收消息内容：";
	wcout<<wszContent<<endl;
	WaitForReq();
}

void CNetSession::WaitForReq()
{
	SetMsgToBuf(MSG_S_WAIT_USER_TALK, L"等你发消息呀");
	cout<<"向客户端发消息：等你发消息呀"<<endl;
	boost::asio::async_write(m_Socket, boost::asio::buffer(m_SendBuf, BUFF_SIZE),
		boost::bind(&CNetSession::OnSendOk, shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));


}

void CNetSession::HandleMsg(char* pData)
{

	NetMsgHead* pHead = (NetMsgHead*)pData;
	switch(pHead->MessageID)
	{
	case MSG_C_REQ_LOGIN:
		{
			//ReqLogIn();
			RspLogIn(pData);

			break;
		}
	case MSG_C_TALK:
		{
			RspLogIn(pData);
			break;
		}
	}
}


void CNetSession::SetMsgToBuf(MSGID id, wstring wszcontent)
{
	memset(m_SendBuf, 0, sizeof(m_SendBuf));
	NetMsgHead msghead;
	msghead.Version = NET_VERSION;
	msghead.MessageID = id;
	msghead.DataLen = wszcontent.length()*sizeof(TCHAR);
	memcpy(m_SendBuf, &msghead, sizeof(msghead));
	memcpy(m_SendBuf + sizeof(msghead), wszcontent.c_str(), wszcontent.length()*sizeof(TCHAR));

}