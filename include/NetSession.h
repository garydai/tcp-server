#pragma once
#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include "NetMsgDef.h"

using boost::asio::ip::tcp;
#define BUFF_SIZE 1024
typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;

//一个session就是一个连接
class CNetSession : public boost::enable_shared_from_this<CNetSession>
{
public:
	CNetSession(boost::asio::io_service& io_service);
	~CNetSession(void);
	void Run();
	void Accept();

	boost::asio::ip::tcp::socket& socket()
	{
		return m_Socket;
	}
	void HandleShandshake(const boost::system::error_code& error);
	void Start();
	void Stop();
	void OnSendOk(const boost::system::error_code& error, size_t bytes_transferred);
	void OnRecvOk(const boost::system::error_code& error, size_t bytes_transferred);
	void SetMsgToBuf(MSGID id, std::wstring wszcontent);
	void HandleMsg(char* pData);
	void RspLogIn(char* pData);
	void WaitForReq();

	void SetSessionID(size_t id)
	{
		m_SessionID = id;
	}

private:
	boost::asio::ip::tcp::socket m_Socket;
	std::string m_MSG;
	bool m_bWaitForMsgHead;
	char m_RecvBuf[BUFF_SIZE];
	char m_SendBuf[BUFF_SIZE];
	char* m_pData;
	char* m_pDataBegin;
	size_t m_WaitLen;

	size_t m_SessionID;



};
