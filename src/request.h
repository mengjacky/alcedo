#ifndef __ALCEDO_REQUEST_H__
#define __ALCEDO_REQUEST_H__
namespace alcedo{
struct Header
{

};
class Buffer;
class Request
{
	public:
		virtual int parseBuffer(Buffer *buffer) ;
		virtual int wrap2Buffer(Buffer *buffer) ;

		virtual int parseBackBuffer(Buffer *buffer) ;
		virtual int wrap2BackBuffer(Buffer *buffer) ;

		int forward(int serverId);
		int doResponse();
		int doResponse(char *bytes, int len);

		void setConnection(Connection *conn){conn_ = conn;}
	private:
		
		Connection *conn_;
};
}
#endif//__ALCEDO_REQUEST_H__
