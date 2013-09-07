#include <stdio.h>
#include <string>
void testb(int *n)
{
	int b=4;
	*n = b;

}
void testa()
{
	char *a = new char[100];
	uint16_t d=128;
	a[1]= (unsigned char)d;
	d = static_cast<uint16_t>(d >> 8);
	
	a[0] = (unsigned char)d;
	uint16_t b = *a;
	uint16_t c = *(a+1);
	///
	uint16_t n;
	n = (unsigned char)a[0];
	n <<= 8;
	n |= (unsigned char)a[1];	
	printf("%s,%d,%d,%d,n=%d\n", a, b,c,d,n);
}
uint32_t peekInt32(char *start)
{
	uint32_t n;
	n = ((unsigned char)(*start));
	n = (n<<8);
	n = ((unsigned char)*(start + 1) | n);
	n = (n << 8);
	n = ((unsigned char)*(start + 2)| n) ;
	n = (n<<8);
	n = ((unsigned char)*(start + 3) | n);
	//drainBuffer(4);
	return n;
}
void writeInt32(uint32_t n, char *ch)
{
	ch[3] = n;
	n = n >>8;
	ch[2] = n;
	n = n >>8;
	ch[1] = n;
	n = n >> 8;
	ch[0] = n;
//	append(ch, 4);

}
int main()
{
	//int a;
	//testb(&a);
	testa();
	//return 1;
	//printf("%d\n", a);
	char *ch = new char[4];
	//memset(ch, 0, 4);
	writeInt32(4294967295, ch);
	
	printf("char=%s\n", ch);
	uint32_t ret = peekInt32(ch);
	printf("ret=%llu\n", ret);
}
