#include "utils.h"
#include <sys/time.h>
#include <string.h>

namespace alcedo
{
namespace utils
{
time_t Time::now()
{
	struct timeval tv;
	gettimeofday(&tv);	
	return (tv.tv_sec * 1000000 + tv.tv_tv_usec);
}
void Time::parse2Spec(struct timespec &spec, time_t t)
{
	memset(spec, 0, sizeof(spec));
	spec.tv_sec = t / 1000000;	
	spec.tv_usec = (t % 1000000) * 1000;
}
std::string Time::format2String(time_t t, std::string &format)
{
	struct tm s_tm;	
	localtime_r(&t, &s_tm);
	std::string str_time;
	str_time.resize(128);
	size_t s = strftime(str_time.c_str(), 
						128,
						format,
						&s_tm);
	str_time.resize(s);
	return str_time;
}
time_t Time::parse2Timestamp(std::string& str_time, std::string& format)
{
	struct tm s_tm;
	char *ptr = strptime(str_time.c_str(), format.c_str(), &s_tm);
	if(ptr){
		return mktime(&s_tm);	
	}
	return 0;
}
}
}
