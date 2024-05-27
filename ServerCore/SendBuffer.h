//#pragma once
//
//class SendBufferChunk;
//
///*----------------
//	SendBuffer
//-----------------*/
//
//class SendBuffer
//{
//public:
//	SendBuffer(SendBufferChunkPtr owner, BYTE* buffer, uint32 allocSize);
//	~SendBuffer();
//
//	BYTE*		Buffer() { return _buffer; }
//	uint32		AllocSize() { return _allocSize; }
//	uint32		WriteSize() { return _writeSize; }
//	void		Close(uint32 writeSize);
//
//private:
//	BYTE*				_buffer;
//	uint32				_allocSize = 0;
//	uint32				_writeSize = 0;
//	SendBufferChunkPtr	_owner;
//};
//
///*--------------------
//	SendBufferChunk
//--------------------*/
//
//class SendBufferChunk : public enable_shared_from_this<SendBufferChunk>
//{
//	enum
//	{
//		SEND_BUFFER_CHUNK_SIZE = 6000
//	};
//
//public:
//	SendBufferChunk();
//	~SendBufferChunk();
//
//	void				Reset();
//	SendBufferPtr		Open(uint32 allocSize);
//	void				Close(uint32 writeSize);
//
//	bool				IsOpen() { return _open; }
//	BYTE*				Buffer() { return &_buffer[_usedSize]; }
//	uint32				FreeSize() { return static_cast<uint32>(_buffer.size()) - _usedSize; }
//
//private:
//	Array<BYTE, SEND_BUFFER_CHUNK_SIZE>		_buffer = {};
//	bool									_open = false;
//	uint32									_usedSize = 0;
//};
//
///*---------------------
//	SendBufferManager
//----------------------*/
//
//class SendBufferManager
//{
//public:
//	SendBufferPtr		Open(uint32 size);
//
//private:
//	SendBufferChunkPtr	Pop();
//	void				Push(SendBufferChunkPtr buffer);
//
//	static void			PushGlobal(SendBufferChunk* buffer);
//
//private:
//	USE_LOCK;
//	Vector<SendBufferChunkPtr> _sendBufferChunks;
//};


#pragma once


class SendBufferChunk;


/*----------------
	SendBuffer
-----------------*/


class SendBuffer : enable_shared_from_this<SendBuffer>
{
public:
	SendBuffer( AtInt32 bufferSize );
	~SendBuffer();

	BYTE* Buffer()    { return _buffer.data(); }
	int32 WriteSize() { return _writeSize; }
	int32 Capacity()  { return static_cast< AtInt32 >( _buffer.size() ); }

	void CopyData( void* data, AtInt32 len );
	void Close( AtUInt32 writeSize );

private:
	vector< BYTE >	_buffer;
	int32			_writeSize = 0;
};
