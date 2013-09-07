#include "buffer.h"
#include <string.h>
#include <sys/ioctl.h>
#include <iostream>
#include <assert.h>
using namespace std;
#define UNUSE_SPACE_LEN(b) ((b)->blkLen - (b)->misalign - (b)->off) 
#define UNUSE_SPACE_PTR(b) ((b)->data + (b)->misalign + (b)->off)
#define NUM_MAX_IOVEC UIO_MAXIOV
namespace alcedo
{

Buffer::Buffer():
	totalLen_(0),
	first_(NULL),
	last_(NULL),
	lastWithData_(NULL)
{

}
Buffer::~Buffer()
{

}
Block* Buffer::_newBlock(int size)
{
	size += BLOCK_SIZE; 
	int alloc_size = MIN_BLOCK_SIZE;		
	while(alloc_size < size)
		alloc_size <<= 1;
	
	Block *blk = (Block*)malloc(alloc_size);
	blk->next = NULL;
	blk->misalign = 0;
	blk->off = 0;
	blk->blkLen = alloc_size - BLOCK_SIZE;
		
	return blk;
}
void Buffer::_insertBlock(Block* blk)
{
	if(!blk)
	{
		return ;	
	}
	if(last_)		
	{
		last_->next = blk;
		last_ = blk;
	}
	else
	{
		first_ = last_ = lastWithData_ = blk;
	}

}
int Buffer::peekBuffer(buffer_iovec *vec, int nvec, int howmuch, int *psize)
{
	Block* blk = first_;	
	int i,datalen = 0, tmp= 0;
	for(i = 0; datalen < howmuch && blk && i < nvec; i++)
	{
		tmp = blk->off;	
		if(!tmp)
		{
			break;
		}
		if(tmp + datalen > howmuch)	
		{
			tmp = howmuch - datalen;
		}
		vec[i].iov_base = blk->data + blk->misalign;	
		vec[i].iov_len = tmp; 
		datalen += tmp;
		blk = blk->next;
	}
	(*psize) = datalen;
	return i;
}
int Buffer::drainBuffer(int howmuch)
{
	Block *blk, *next;
	int drain_len = 0, tmp = 0;
	int old_len = totalLen_ ;
	if(old_len < howmuch)
		howmuch = old_len;

	totalLen_ -= howmuch; 
	for(blk = first_; drain_len < howmuch && blk; blk = next)	
	{
		next = blk->next;
		tmp = blk->off;
		if(blk->off > howmuch - drain_len)	
		{
			tmp = howmuch - drain_len;
			blk->misalign += tmp;
			blk->off -= tmp;
			break;
		}
		else
		{
			free(blk);	
			blk = NULL;
		}
		drain_len += tmp;
	}
	first_ = blk;
	if(!blk)
	{
		lastWithData_ = last_ = NULL; 
	}

	return howmuch;
}
int Buffer::_setupForVec(buffer_iovec *vec, int nvec, int size)
{
	Block *blk = lastWithData_, *pre;		
	int i;
	int space = 0;
	if(UNUSE_SPACE_LEN(blk) == 0)	
	{
		blk = blk->next;	
	}
	for(i = 0; blk && (i < nvec ) && space < size;
		   	blk = blk->next, i++)
	{
		pre = blk;
		vec[i].iov_base = UNUSE_SPACE_PTR(blk);
		vec[i].iov_len = UNUSE_SPACE_LEN(blk);
		space += vec[i].iov_len;
	}
	return i;	
}

int	Buffer::setupSpace(buffer_iovec *vec, int nvec, int size) 
{
	assert(nvec > 1);
	if(vec == NULL || nvec <= 0)	
		return -1;
	_ensureSpace(size, nvec);
	int ret = _setupForVec(vec, nvec, size);
	return ret;

}
int Buffer::commitSpace(buffer_iovec *vec, int nvec)
{
	if(nvec == 0 || vec == NULL)	
		return 0;
	Block *blk = lastWithData_;
	Block *tmp = blk;
	int i, added;
	if(UNUSE_SPACE_LEN(blk) == 0)
		blk = blk->next;
	//make sure 

	for(i = 0; i < nvec; i++)
	{
		//not in the bound
		if(!tmp)
		{
			return 0;
		}
		if(vec[i].iov_base != UNUSE_SPACE_PTR(tmp) ||
				vec[i].iov_len > UNUSE_SPACE_LEN(tmp))
			return 0;
		tmp = tmp->next;	
	}
	//adjust buffer
	added = 0;
	for(i = 0; i < nvec; i++)	
	{
		blk->off += vec[i].iov_len;
		added += vec[i].iov_len;
		if(vec[i].iov_len)
			lastWithData_ = blk;
		blk = blk->next;
	}
	totalLen_ += added;
	return 1;
}
#define MAX_MOVE_DATA 1024
bool Buffer::_worthMarshal(Block* blk, int nalloc)
{
	return ((blk->blkLen - blk->off) >= nalloc &&
			(blk->off < blk->blkLen/2) && 
			(blk->off < MAX_MOVE_DATA));
}
void Buffer::_remarshalling(Block* blk)
{
	memmove(blk->data, blk->misalign + blk->data, blk->off);
	blk->misalign = 0;
}
int Buffer::_ensureSpace(int size, int nvec)
{
	Block *blk = last_;		
	int avail = 0;
	int used = 0;
	int space = 0;
	if(blk == NULL)
	{
		blk = _newBlock(size);	
		if(!blk)
			return -1;
		_insertBlock(blk);
		return (1);
	}
	blk = lastWithData_;
	if(_worthMarshal(blk, size))
	{
		_remarshalling(blk);	
		return (1);
	}

	for(; blk; blk = blk->next)	
	{
		if(blk->off)
		{
			space = UNUSE_SPACE_LEN(blk);
			if(space)
			{
				used++;
			}
		}
		else
		{
			blk->misalign = 0;	
			space = blk->blkLen;
			used++;
		}
		//not enough space for nvec
		//remove blocks and instead of a new block
		if(used == nvec && (avail + space) < size)
		{
			assert(!blk->off);
			_removeAtBlock(blk);
			Block *tmp = _newBlock(size - avail);
			_insertBlock(tmp);
			return used;
		}
		avail += space; 
	}
	if(avail >= size)
		return (used);

	blk = _newBlock(size - avail);
	used++;
	if(!blk)
		return -1;
	_insertBlock(blk);
	return used;
}
int Buffer::bufferWritev(int fd, int howmuch)
{
	Block *blk = first_;
	int has_written = 0, tmp = 0;
	if(howmuch > totalLen_)	
		howmuch = totalLen_;
	buffer_iovec iov[NUM_MAX_IOVEC];
	int i;
	for(i = 0; i < NUM_MAX_IOVEC && has_written < howmuch && blk; i++)
	{
		tmp = blk->off;
		if(has_written + blk->off > howmuch)
		{
			tmp = howmuch - has_written;
		}
		iov[i].iov_base = blk->data;	
		iov[i].iov_len = tmp;
		has_written += tmp; 
	}
	int n = ::writev(fd, iov, i);	
	if(n > 0)
	{
		drainBuffer(n);	
	}
	return n;
}
int Buffer::bufferReadv(int fd)
{
	int nread = 0;
	int ret = 0;
	if(::ioctl(0, FIONREAD, &nread) == -1)
		return -1;
	if(nread == 0)	
		return 0;
	int space = _ensureSpace(nread, -1);	
	if(space < nread)	
		return -1;

	buffer_iovec iov[NUM_MAX_IOVEC]; 
	int num = _setupForVec(iov, NUM_MAX_IOVEC, nread);
	ret = ::readv(fd, iov, num);	
	
	return ret;
}
int Buffer::append(char *bytes, int len)
{
	if(bytes == NULL || len <= 0)
		return -1;
	//expand buffer
	_ensureSpace(len, -1);

	Block *blk = lastWithData_;
	char *start;
	int remaining = 0, blklen = 0;

	if(UNUSE_SPACE_LEN(blk) == 0)
		blk = blk->next;
	for(;blk && len > 0; blk = blk->next)	
	{
		start = UNUSE_SPACE_PTR(blk); 	
		blklen = UNUSE_SPACE_LEN(blk);
		if(blklen > len)
			blklen = len;
		memmove(start, bytes + remaining, blklen);		
		len -= blklen;
		remaining += blklen;

		blk->off += blklen;
		totalLen_ += blklen;
		lastWithData_ = blk;
	}
	if(len > 0)
	{
		return -1;	
	}
	return 1;
}
int Buffer::copyOut(char *out, int howmuch)
{
	Block *blk = first_;	
	char *start;
	int datalen = totalLen_, old_len = totalLen_, remaining;	
	
	if(out == NULL || howmuch <= 0)
		return -1;
	if(datalen > howmuch)
		datalen = old_len = howmuch;
	while(blk && datalen >0)	
	{
		start = blk->data + blk->misalign;
		remaining = blk->off;
		if(datalen < blk->off)
			remaining = datalen;
		memcpy(out, start, remaining);
		datalen -= remaining;
		blk = blk->next;
	}
	return old_len;
}
int Buffer::readAsString(std::string &out, int howmuch)
{
	char bytes[howmuch];
	int nret = copyOut(bytes, howmuch);
	out.clear();
	out.resize(nret);
	memcpy((char*)out.c_str(), bytes, nret);
	drainBuffer(nret);
	return out.size();
}
int Buffer::readAsBytes(char *out, int howmuch)
{
	int nret = copyOut(out, howmuch);
	drainBuffer(nret);
	return nret;
}
void Buffer::_removeAtBlock(Block *rm_blk)
{
	Block *next_blk = rm_blk, *tmp;
	int start_rm = 0;
	if(!rm_blk)
		return;
	while(next_blk)
	{

		tmp = next_blk;
		next_blk = next_blk->next;	
		free(tmp);
	}

}
void Buffer::dumpBuffer()
{
	Block *blk ;
	int i = 0;
	cout<<"totallen="<<totalLen_<<endl;
	for(blk = first_; blk; blk = blk->next)
	{
		cout<< "index="<< i<<
			";mislign="<< blk->misalign<<
			" off="<<blk->off<<
			" blkLen="<< blk->blkLen<<
			" data="<<blk->data + blk->misalign<<endl;
		i++;
	}
	
}
}
