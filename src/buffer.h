#ifndef __ALCEDO_BUFFER_H__
#define __ALCEDO_BUFFER_H__
#include <string>
#include <sys/uio.h>
namespace alcedo
{
#define MIN_BLOCK_SIZE	2046 
#define buffer_iovec iovec
struct Block
{
	Block *next;
	int misalign;
	//offset misalign
	int off;
	int blkLen;
	char data[0];
};
#define BLOCK_SIZE sizeof(struct Block)
class Buffer
{
	public:
		Buffer();
		~Buffer();
		int peekBuffer(buffer_iovec *vec, int nvec, int howmuch, int *psize);	
		int drainBuffer(int howmuch);

		int	setupSpace(buffer_iovec *vec, int nvec, int size); 
		int commitSpace(buffer_iovec *vec, int nvec);

		int append(char *bytes, int size);
		int copyOut(char *out, int howmuch);
		int readAsBytes(char *out, int howmuch);
		int readAsString(std::string &out, int howmuch);
		void dumpBuffer();
		uint8_t peekInt8()
		{
			uint8_t n;		
			Block *blk = first_;	
			n = (unsigned char)*(blk->data + blk->misalign);
			//drainBuffer(1);
			return n;
		}
		uint16_t peekInt16()
		{
			uint16_t n;
			Block *blk = first_;
			char *start = blk->data + blk->misalign;
			n = (unsigned char)*start ;
			n <<= 8;
			n |= (unsigned char)*(start + 1);
			//drainBuffer(2);
			return n;
		}
		uint32_t peekInt32()
		{
			uint32_t n;
			Block *blk = first_;
			char *start = blk->data + blk->misalign;
			n = (unsigned char)*start ;
			n <<= 8;
			n |= (unsigned char)*(start + 1);
			n <<= 8;
			n |= (unsigned char)*(start + 2);
			n <<= 8;
			n |= (unsigned char)*(start + 3);
			//drainBuffer(4);
			return n;
		}
		uint64_t peekInt64()
		{
			uint64_t n;
			Block *blk = first_;
			char *start = blk->data + blk->misalign;
			n = (unsigned char)*start;
			n <<= 8;
			n |= (unsigned char)*(start + 1);
			n <<= 8;
			n |= (unsigned char)*(start + 2);
			n <<= 8;
			n |= (unsigned char)*(start + 3);
			n <<= 8;
			n |= (unsigned char)*(start + 4);
			n <<= 8;
			n |= (unsigned char)*(start + 5);
			n <<= 8;
			n |= (unsigned char)*(start + 6);
			n <<= 8;
			n |= (unsigned char)*(start + 7);
			//drainBuffer(8);
			return n;
		}

		void writeInt8(uint8_t n)
		{
			char ch[1] = {0};
			ch[0] = n;
			append(ch, 1);
		}
		void writeInt16(uint16_t n)
		{
			char ch[2] = {0};
			ch[1] = n;	
			n >>= 8;
			ch[0] = n;
			append(ch, 2);
		}
		void writeInt32(uint32_t n)
		{
			char ch[4] = {0};
			ch[3] = n;	
			n >>= 8;
			ch[2] = n;
			n >>= 8;
			ch[1] = n;
			n >>= 8;
			ch[0] = n;
			append(ch, 4);
	
		}
		void writeInt64(uint64_t n)
		{
			char ch[8] = {0};
			ch[7] = n;	
			n >>= 8;
			ch[6] = n;
			n >>= 8;
			ch[5] = n;
			n >>= 8;
			ch[4] = n;
			n >>= 8;
			ch[3] = n;
			n >>= 8;
			ch[2] = n;
			n >>= 8;
			ch[1] = n;
			n >>= 8;
			ch[0] = n;
			append(ch, 8);
		}
		int bufferWritev(int fd, int howmuch);
		int bufferReadv(int fd);	
		//for debug
		int ensureSpace(int size, int nvec = -1)
		{
			return _ensureSpace(size, nvec);
		}
		int setupForVec(buffer_iovec *vec, int nvec, int size)
		{
			return _setupForVec(vec, nvec, size);
		}

	private:
		Block* _newBlock(int size);
		void _insertBlock(Block* blk);
		bool _worthMarshal(Block* blk, int nalloc);
		void _remarshalling(Block* blk);
		int _ensureSpace(int size, int nvec);
		int _setupForVec(buffer_iovec *vec, int nvec, int size);
		void _removeAtBlock(Block *rm_blk);
	private:
		//all datalen in the buffer
		int totalLen_;	
		Block* first_;		 
		Block* last_;
		Block* lastWithData_;

};
}
#endif //__ALCEDO_BUFFER_H__
