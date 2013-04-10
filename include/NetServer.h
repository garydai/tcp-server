#pragma once
#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/thread.hpp>

class CNetSession;
using boost::asio::ip::tcp;
typedef boost::shared_ptr<CNetSession> pNetSession;

class CNetServer
	:private boost::noncopyable//不能被复制赋值
{
public:
	CNetServer(unsigned int port, std::size_t thread_pool_size);

	~CNetServer(void);
  /// Run the server's io_service loop.
	void Run();
  /// Stop the server.
	void Stop();

private:
  /// Handle completion of an asynchronous accept operation.
	void HanldeAccept(pNetSession session, const boost::system::error_code& error);

	void StartAccept();


private:
	boost::asio::io_service m_IoService;

	tcp::acceptor m_Acceptor;

	//boost::asio::ssl::context m_sslContext;

	std::size_t m_ThreadPoolSize;

	std::vector<pNetSession> m_vNetSession;

	//会话ID
	std::size_t m_SessionID;

	pNetSession pSession;
};