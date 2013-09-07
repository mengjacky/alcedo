#ifndef __ALCEDO_ULTIS_H_
#define __ALCEDO_ULTIS_H_
namespace alcedo
{
namespace utils
{
class Time
{
	static uint64_t now();	
	static std::string format2String(time_t t, std::string &format);
	static void format2Spec(struct timespec &spec, time_t t);
	static time_t parse2Timestamp(std::string& str_time, std::string& format);	
}
}
}
#endif//__ALCEDO_ULTIS_H_
