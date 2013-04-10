#include "NetServer.h"
#include "NetSession.h"

CNetServer::CNetServer(unsigned int port, std::size_t thread_pool_size):
	m_Acceptor(m_IoService, tcp::endpoint(tcp::v4(), port)),
		m_ThreadPoolSize(thread_pool_size), m_SessionID(0)
{
  //  m_sslContext.set_options(
  //      boost::asio::ssl::context::default_workarounds
   //     | boost::asio::ssl::context::no_sslv2
   //     | boost::asio::ssl::context::single_dh_use);
	//    m_sslContext.set_options(boost::asio::ssl::context::verify_none);

   // m_sslContext.set_password_callback(boost::bind(&CNetServer::get_password, this));
    //  m_sslContext.use_certificate_chain_file("s3voice-cert.pem");
    //  m_sslContext.use_private_key_file("s3voice-key.pem", boost::asio::ssl::context::pem);
  //  m_sslContext.use_tmp_dh_file("dh512.pem");



}

CNetServer::~CNetServer(void)
{

}

void CNetServer::Run()
{
	
	StartAccept();

  // Create a pool of threads to run all of the io_services.
	std::vector<boost::shared_ptr<boost::thread>> vThreads;
	for(std::size_t i = 0; i < m_ThreadPoolSize; i ++)
	{
		boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&boost::asio::io_service::run, &m_IoService)));
		vThreads.push_back(thread);
	}

	  // Wait for all threads in the pool to exit.
	 for (std::size_t i = 0; i < vThreads.size(); ++i)
		  vThreads[i]->join();


	//m_IoService.run();

}

void CNetServer::HanldeAccept(pNetSession session, const boost::system::error_code& error)
{
	std::cout << "New connection: " << session->socket().remote_endpoint().address() << std::endl;
	if(!error)
	{
		//还未检查连接数是否超过限制

		session->Start();
		StartAccept();
	}
}

void CNetServer::StartAccept()
{

	pNetSession pNewNetSession(new CNetSession(m_IoService));
			//设置会话ID
	pNewNetSession->SetSessionID(m_SessionID);	
	m_SessionID ++;
	m_vNetSession.push_back(pNewNetSession);

	m_Acceptor.async_accept(pNewNetSession->socket(), boost::bind(&CNetServer::HanldeAccept, this, pNewNetSession, boost::asio::placeholders::error));
}

void CNetServer::Stop()
{
	m_IoService.stop();
	m_Acceptor.close();

}
