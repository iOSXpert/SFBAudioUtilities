/*
 * Copyright (c) 2013 - 2021 Stephen F. Booth <me@sbooth.org>
 * MIT license
 */

#import <algorithm>
#import <cstdlib>
#import <stdexcept>

#import "SFBAudioBufferList.hpp"

AudioBufferList * SFBAllocateAudioBufferList(const SFBAudioFormat& format, UInt32 frameCapacity) noexcept
{
	auto bufferDataSize = frameCapacity * format.mBytesPerFrame;
	auto bufferCount = format.ChannelStreamCount();
	auto bufferListSize = offsetof(AudioBufferList, mBuffers) + (sizeof(AudioBuffer) * bufferCount);
	auto allocationSize = bufferListSize + (bufferDataSize * bufferCount);

	AudioBufferList *abl = static_cast<AudioBufferList *>(std::calloc(1, allocationSize));
	if(!abl)
		return nullptr;

	abl->mNumberBuffers = bufferCount;

	for(auto i = 0; i < bufferCount; ++i) {
		abl->mBuffers[i].mNumberChannels = format.InterleavedChannelCount();
		abl->mBuffers[i].mData = reinterpret_cast<uint8_t *>(abl) + bufferListSize + (bufferDataSize * i);
		abl->mBuffers[i].mDataByteSize = bufferDataSize;
	}

	return abl;
}

SFBAudioBufferList::SFBAudioBufferList() noexcept
: mBufferList(nullptr), mFrameCapacity(0)
{}

SFBAudioBufferList::SFBAudioBufferList(const SFBAudioFormat& format, UInt32 frameCapacity)
: SFBAudioBufferList()
{
	if(!Allocate(format, frameCapacity))
		throw std::bad_alloc();
}

SFBAudioBufferList::~SFBAudioBufferList()
{
	std::free(mBufferList);
}

bool SFBAudioBufferList::Allocate(const SFBAudioFormat& format, UInt32 frameCapacity) noexcept
{
	if(mBufferList)
		Deallocate();

	mBufferList = SFBAllocateAudioBufferList(format, frameCapacity);
	if(!mBufferList)
		return false;

	mFormat = format;
	mFrameCapacity = frameCapacity;

	return true;
}

bool SFBAudioBufferList::Deallocate() noexcept
{
	if(!mBufferList)
		return false;

	mFrameCapacity = 0;
	mFormat = {};

	std::free(mBufferList);

	return true;
}

UInt32 SFBAudioBufferList::FrameLength() const noexcept
{
	if(!mBufferList)
		return 0;

#if DEBUG
	auto buffer0FrameLength = mBufferList->mBuffers[0].mDataByteSize / mFormat.mBytesPerFrame;
	for(UInt32 bufferIndex = 0; bufferIndex < mBufferList->mNumberBuffers; ++bufferIndex)
		assert(mBufferList->mBuffers[bufferIndex].mDataByteSize / mFormat.mBytesPerFrame == buffer0FrameLength);
#endif

	return mBufferList->mBuffers[0].mDataByteSize / mFormat.mBytesPerFrame;
}

bool SFBAudioBufferList::SetFrameLength(UInt32 frameLength) noexcept
{
	if(!mBufferList || frameLength > mFrameCapacity)
		return false;

	for(UInt32 bufferIndex = 0; bufferIndex < mBufferList->mNumberBuffers; ++bufferIndex)
		mBufferList->mBuffers[bufferIndex].mDataByteSize = frameLength * mFormat.mBytesPerFrame;

	return true;
}
