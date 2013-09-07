#ifndef __ALCEDO_ENDPOINT_H__
#define __ALCEDO_ENDPOINT_H__
#include "inetadress.h"
namespace alcedo
{

class BackendInfo 
{
	public:
		EndpointInfo(int id, InetAddress addr, ConnType type):
			id_(id),
			addr_(addr),
			type_(type),
			codec_(codec)
		{
			
		}
		InetAddress& getAddress(){return addr_;}

	private:
		int id_;
		bool is_server_; 
		InetAdress addr_;
		ConnType type_;	
		ICodecProtocol *codec_;

};
class Endpoint
{
	public:
		typedef std::map<int, BackendInfo*> InfoMap;
		typedef std::vector<Acceptor*> AcceptorList;
		Endpoint();
		~Endpoint();
		static Endpoint* getInstance()
		{
			if(enpoint == NULL)	
				endpoint = new Endpoint();
			return endpoint;
		}
		connect();
		listen();
		set_io_thread_num(int num){ io_thread_num_ = num;}
		void start();
		void setBackend(int serverId, 
				InetAddress cli_addr, 
				ConnType type, 
				ICodecProtocol *codec)
		{
			BackendInfo *info = new BackendInfo(idx, srv_addr, type);
			back_infos[serverId] = info;
		}
		void setServerInfo(int idx, 
				InetAddress srv_addr, 
				ConnType type, 
				IDcodeProtocol *codec)
		{
			Acceptor *acceptor = new Acceptor(info->getAddress(), ConnType type, ICodecProtocol *codec);
			acceptors_.push_back(acceptor);
		}
		void dispatchConn(int fd, InetAddress *addr, ConnType type, ICodecProtocol *codec);
	private:
		int getThreadId();
		int io_thread_num_;
		InfoMap back_infos_;
		AcceptorList acceptors_;
		EventReactorPool *pool_;
		static Endpoint *endpoint;
};
}
#endif//__ALCEDO_ENDPOINT_H__
