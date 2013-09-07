#ifndef __ALCEDO_REQUEST_FACTORY_H__
#define __ALCEDO_REQUEST_FACTORY_H__
namespace alcedo
{
class IRequestFactory
{
	public:
		Request* createRequest(int32_t code);
};
}
#endif //__ALCEDO_REQUEST_FACTORY_H__
