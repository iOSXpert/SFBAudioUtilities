//
// Copyright (c) 2013 - 2021 Stephen F. Booth <me@sbooth.org>
// MIT license
//

#pragma once

#import <atomic>

#import <CoreAudio/CoreAudioTypes.h>

#import "SFBAudioStreamBasicDescription.hpp"

/// A ring buffer supporting non-interleaved audio based on Apple's @c CARingBuffer.
///
/// This class is thread safe when used from one reader thread and one writer thread (single producer, single consumer model).
class SFBCARingBuffer
{

public:

#pragma mark Creation and Destruction

	/// Creates a new @c SFBCARingBuffer
	/// @note @c Allocate() must be called before the object may be used.
	SFBCARingBuffer() noexcept;

	/// Destroys the @c SFBCARingBuffer and release all associated resources.
	~SFBCARingBuffer();

	// This class is non-copyable
	SFBCARingBuffer(const SFBCARingBuffer& rhs) = delete;

	// This class is non-assignable
	SFBCARingBuffer& operator=(const SFBCARingBuffer& rhs) = delete;

#pragma mark Buffer management

	/// Allocates space for audio data.
	/// @note Only interleaved formats are supported.
	/// @note This method is not thread safe.
	/// @note Capacities from 2 to 2,147,483,648 (0x80000000) frames are supported
	/// @param format The format of the audio that will be written to and read from this buffer.
	/// @param capacityFrames The desired capacity, in frames
	/// @return @c true on success, @c false on error
	bool Allocate(const SFBAudioStreamBasicDescription& format, size_t capacityFrames) noexcept;

	/// Frees the resources used by this @c SFBCARingBuffer
	/// @note This method is not thread safe.
	void Deallocate() noexcept;


	/// Returns the capacity in frames of this @c SFBCARingBuffer
	inline size_t CapacityFrames() const noexcept
	{
		return mCapacityFrames;
	}

	/// Returns the format of this @c SFBCARingBuffer
	inline const SFBAudioStreamBasicDescription& Format() const noexcept
	{
		return mFormat;
	}

	/// Gets the time bounds of the audio contained in this @c SFBCARingBuffer
	/// @param startTime The starting sample time of audio contained in the buffer
	/// @param endTime The end sample time of audio contained in the buffer
	/// @return @c true on success, @c false on error
	bool GetTimeBounds(int64_t& startTime, int64_t& endTime) const noexcept;

#pragma mark Reading and writing audio

	/// Reads audio from the @c SFBCARingBuffer
	///
	/// The sample times should normally increase sequentially, although gaps are filled with silence. A sufficiently large
	/// gap effectively empties the buffer before storing the new data.
	/// @note If @c timeStamp is less than the previous sample time the behavior is undefined
	/// @param bufferList An @c AudioBufferList to receive the audio
	/// @param frameCount The desired number of frames to read
	/// @param timeStamp The starting sample time
	/// @return @c true on success, @c false on error
	bool Read(AudioBufferList * const bufferList, size_t frameCount, int64_t timeStamp) noexcept;

	/// Writes audio to the @c SFBCARingBuffer
	/// @param bufferList An @c AudioBufferList containing the audio to copy
	/// @param frameCount The desired number of frames to write
	/// @param timeStamp The starting sample time
	/// @return @c true on success, @c false on error
	bool Write(const AudioBufferList * const bufferList, size_t frameCount, int64_t timeStamp) noexcept;

protected:

	/// Returns the byte offset of @c frameNumber
	inline size_t FrameByteOffset(int64_t frameNumber) const noexcept
	{
		return (frameNumber & mCapacityFramesMask) * mFormat.mBytesPerFrame;
	}

	/// Constrains @c startRead and @c endRead to valid timestamps in the buffer
	bool ClampTimesToBounds(int64_t& startRead, int64_t& endRead) const noexcept;

	/// Returns the buffer's starting sample time
	/// @note This should only be called from @c Write()
	inline int64_t StartTime() const noexcept
	{
		return mTimeBoundsQueue[mTimeBoundsQueueCounter & sTimeBoundsQueueMask].mStartTime;
	}

	/// Returns the buffer's ending sample time
	/// @note This should only be called from @c Write()
	inline int64_t EndTime() const noexcept
	{
		return mTimeBoundsQueue[mTimeBoundsQueueCounter & sTimeBoundsQueueMask].mEndTime;
	}

	/// Sets the buffer's start and end sample times
	/// @note This should only be called from @c Write()
	void SetTimeBounds(int64_t startTime, int64_t endTime) noexcept;

private:

	/// The format of the audio
	SFBAudioStreamBasicDescription mFormat;

	/// The channel pointers and buffers allocated in one chunk of memory
	uint8_t **mBuffers;

	/// The frame capacity per channel
	size_t mCapacityFrames;
	/// Mask used to wrap read and write locations
	/// @note Equal to @c mCapacityFrames-1
	size_t mCapacityFramesMask;

	/// A range of valid sample times in the buffer
	struct TimeBounds {
		/// The starting sample time
		int64_t mStartTime;
		/// The ending sample time
		int64_t mEndTime;
		/// The value of @c mTimeBoundsQueueCounter when the struct was modified
		std::atomic_uint64_t mUpdateCounter;
	};

	/// The number of elements in @c mTimeBoundsQueue
	static const size_t sTimeBoundsQueueSize = 32;
	/// Mask value used to wrap time bounds counters
	/// @note Equal to @c sTimeBoundsQueueSize-1
	static const size_t sTimeBoundsQueueMask = sTimeBoundsQueueSize - 1;

	/// Array of @c TimeBounds structs
	TimeBounds mTimeBoundsQueue[sTimeBoundsQueueSize];
	/// Monotonically increasing counter incremented when the buffer's time bounds changes
	std::atomic_uint64_t mTimeBoundsQueueCounter;

};