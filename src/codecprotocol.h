#ifndef __ALCEDO_CODEC_PROTOCOL_H__
#define __ALCEDO_CODEC_PROTOCOL_H__
#include <string> 
class Buffer;
class ICodecProtocol
{
	public:
		ICodecProtocol()
		{
		}
		virtual ~ICodecProtocol()
		{
		}
		virtual int encode(void *packet, Buffer *out_buffer) = 0;
		virtual int decode(void *packet, Buffer *in_buffer) = 0;
};
class DefaultCodecProtocol : public ICodecProtocol
{

};
#endif
