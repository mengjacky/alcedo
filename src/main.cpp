#include "buffer.h"
#include <string>
#include <iostream>
using namespace std;
using namespace alcedo;
void test_read(Buffer &buffer, int len)
{
	string out;
	int ret = buffer.readAsString(out, len);
	cout <<"ret="<<ret<< " out="<< endl;	
	cout<<"dump"<<endl;
	buffer.dumpBuffer();
	//buffer._ensureSpace(2046);
	cout<<"after ensure, dump"<<endl;

	buffer.dumpBuffer();

}
void test_copyOut(Buffer &buffer)
{
	char bytes[10] = {0};
	int ret = buffer.copyOut(bytes, 10);
	cout<<"out=="<< bytes<<endl;
}
void test_drain(Buffer &buffer)
{
	int ret = buffer.drainBuffer(9);
	buffer.dumpBuffer();
	//buffer._ensureSpace(2010);
	buffer.dumpBuffer();

}
void test_peek(Buffer &buffer)
{
	char a[] = "hello world";
	int len = strlen(a);	
	int ret = buffer.append(a, len);
	
	cout<<ret<<endl;
	buffer.ensureSpace(2046);
	buffer.ensureSpace(8001);
	buffer.dumpBuffer();	

	cout<<"peek"<<endl;
	buffer_iovec iov[2];
	int psize = 0;
	ret = buffer.peekBuffer(iov, 2, 6006, &psize);
	cout <<"ret="<<ret<< " size="<<psize<<endl;
	buffer.dumpBuffer();
}
void test_setup_space(Buffer &buffer)
{
	char a[] = "hello world";
	int len = strlen(a);	
	int ret = buffer.append(a, len);

	cout<<ret<<endl;
	buffer.ensureSpace(2046);
	buffer.ensureSpace(8002);
	buffer.dumpBuffer();	
	//ret = buffer.ensureSpace(8001,1);
	buffer_iovec iov[3];
	ret = buffer.setupSpace(iov, 3, 1000000);
	cout<<ret<<endl;
	buffer.dumpBuffer();	
	cout<<"write"<<endl;
	for(int i = 0; i < ret; i++)
	{
		char *a = (char*)iov[i].iov_base ;	
		int len = iov[i].iov_len;
		snprintf(a,len, "%s,%d","hhhh",i);
		iov[i].iov_len = 7;
	}
	ret = buffer.commitSpace(iov, ret);
	cout<<ret<<endl;
	buffer.dumpBuffer();	


}
void test_readWriteInt(Buffer &buffer)
{
	buffer.writeInt32(1000);	
	uint32_t ret = buffer.peekInt32();	
	cout<<ret<<endl;
	printf("%d\n", ret);
	buffer.dumpBuffer();	
}
int main()
{
	Buffer buffer;
	//test_peek(buffer);
	//test_setup_space(buffer);
	test_readWriteInt(buffer);
	return 1;
}
