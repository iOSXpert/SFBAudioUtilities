//
// Copyright (c) 2020 - 2021 Stephen F. Booth <me@sbooth.org>
// MIT license
//

#pragma once

#import <algorithm>
#import <stdexcept>
#import <type_traits>

namespace SFB {

/// A @c ByteStream provides heterogeneous typed access to an untyped buffer.
class ByteStream
{

public:

	/// Creates an empty @c ByteStream
	ByteStream() noexcept
	: mBuffer(nullptr), mBufferLength(0), mReadPosition(0)
	{}

	/// Initializes a @c ByteStream object with the same buffer, length, and read position as @c rhs
	/// @param rhs The object to copy
	ByteStream(const ByteStream& rhs) noexcept
	: mBuffer(rhs.mBuffer), mBufferLength(rhs.mBufferLength), mReadPosition(rhs.mReadPosition)
	{}

	/// Sets the buffer, length, and read position to those of @c rhs
	/// @param rhs The object to copy
	/// @return A reference to @c this
	ByteStream& operator=(const ByteStream& rhs) noexcept
	{
		if(this != &rhs) {
			mBuffer = rhs.mBuffer;
			mBufferLength = rhs.mBufferLength;
			mReadPosition = rhs.mReadPosition;
		}
		return *this;
	}

	/// Destructor
	~ByteStream() = default;

	/// Move constructor
	ByteStream(ByteStream&& rhs) noexcept
	: mBuffer(rhs.mBuffer), mBufferLength(rhs.mBufferLength), mReadPosition(rhs.mReadPosition)
	{
		rhs.mBuffer = nullptr;
		rhs.mBufferLength = 0;
		rhs.mReadPosition = 0;
	}

	/// Move assignment operator
	ByteStream& operator=(ByteStream&& rhs) noexcept
	{
		if(this != &rhs) {
			mBuffer = rhs.mBuffer;
			mBufferLength = rhs.mBufferLength;
			mReadPosition = rhs.mReadPosition;

			rhs.mBuffer = nullptr;
			rhs.mBufferLength = 0;
			rhs.mReadPosition = 0;
		}
		return *this;
	}


	/// Initializes a @c ByteStream object with the specified buffer and length and sets the read position to @c 0
	/// @param buf The buffer providing the data
	/// @param len The length of @c buf in bytes
	/// @throw @c std::invalid_argument if @c buf==nullptr and @c len>0
	ByteStream(const void * const _Nullable buf, uint32_t len)
	: mBuffer(buf), mBufferLength(len), mReadPosition(0)
	{
		if(!mBuffer && len > 0)
			throw std::invalid_argument("!mBuffer && len > 0");
	}


	/// Compares to @c ByteStream objects for equality
	/// Two @c ByteStream objects are equal if they have the same buffer, length, and read position
	/// @param rhs The object to compare
	/// @return @c true if the objects are equal, @c false otherwise
	bool operator==(const ByteStream& rhs) noexcept
	{
		return mBuffer == rhs.mBuffer && mBufferLength == rhs.mBufferLength && mReadPosition == rhs.mReadPosition;
	}

	/// Compares to @c ByteStream objects for inequality
	/// Two @c ByteStream objects are equal if they have the same buffer, length, and read position
	/// @param rhs The object to compare
	/// @return @c true if the objects are not equal, @c false otherwise
	bool operator!=(const ByteStream& rhs) noexcept
	{
		return mBuffer != rhs.mBuffer || mBufferLength != rhs.mBufferLength || mReadPosition != rhs.mReadPosition;
	}


	/// Reads an integral type and advances the read position
	/// @tparam T The integral type to read
	/// @param value The destination value
	/// @return @c true on success, @c false otherwise
	template <typename T>
	typename std::enable_if<std::is_integral<T>::value, bool>::type Read(T& value) noexcept
	{
		auto valueSize = sizeof(value);
		if(valueSize > Remaining())
			return false;
		auto bytesRead = Read(&value, valueSize);
		return bytesRead == valueSize;
	}

	/// Reads an unsigned little endian integral type converted to host byte ordering and advances the read position
	/// @tparam T The unsigned integral type to read
	/// @param value The destination value
	/// @return @c true on success, @c false otherwise
	template <typename T>
	typename std::enable_if<std::is_unsigned<T>::value, bool>::type ReadLE(T& value) noexcept
	{
		auto valueSize = sizeof(value);
		if(valueSize > Remaining())
			return false;
		auto bytesRead = Read(&value, valueSize);
		if(valueSize != bytesRead)
			return false;

		switch(valueSize) {
			case 2:	value = static_cast<T>(OSSwapLittleToHostInt16(value)); break;
			case 4:	value = static_cast<T>(OSSwapLittleToHostInt32(value)); break;
			case 8:	value = static_cast<T>(OSSwapLittleToHostInt64(value)); break;
		}

		return true;
	}

	/// Reads an unsigned big endian integral type converted to host byte ordering and advances the read position
	/// @tparam T The unsigned integral type to read
	/// @param value The destination value
	/// @return @c true on success, @c false otherwise
	template <typename T>
	typename std::enable_if<std::is_unsigned<T>::value, bool>::type ReadBE(T& value) noexcept
	{
		auto valueSize = sizeof(value);
		if(valueSize > Remaining())
			return false;
		auto bytesRead = Read(&value, valueSize);
		if(valueSize != bytesRead)
			return false;

		switch(valueSize) {
			case 2:	value = static_cast<T>(OSSwapBigToHostInt16(value)); break;
			case 4:	value = static_cast<T>(OSSwapBigToHostInt32(value)); break;
			case 8:	value = static_cast<T>(OSSwapBigToHostInt64(value)); break;
		}

		return true;
	}

	/// Reads an unsigned integral type, swaps its byte ordering, and advances the read position
	/// @tparam T The unsigned integral type to read
	/// @param value The destination value
	/// @return @c true on success, @c false otherwise
	template <typename T>
	typename std::enable_if<std::is_unsigned<T>::value, bool>::type ReadSwapped(T& value) noexcept
	{
		auto valueSize = sizeof(value);
		if(valueSize > Remaining())
			return false;
		auto bytesRead = Read(&value, valueSize);
		if(valueSize != bytesRead)
			return false;

		switch(valueSize) {
			case 2: value = static_cast<T>(OSSwapInt16(value)); break;
			case 4: value = static_cast<T>(OSSwapInt32(value)); break;
			case 8: value = static_cast<T>(OSSwapInt64(value)); break;
		}

		return true;
	}

	/// Reads an integral type and advances the read position
	/// @tparam T The integral type to read
	/// @return The value read or @c 0 on failure
	template <typename T>
	typename std::enable_if<std::is_integral<T>::value, T>::type Read() noexcept
	{
		T value;
		return Read(value) ? value : 0;
	}

	/// Reads an unsigned little endian integral type converted to host byte ordering and advances the read position
	/// @tparam T The unsigned integral type to read
	/// @return The value read or @c 0 on failure
	template <typename T>
	typename std::enable_if<std::is_unsigned<T>::value, T>::type ReadLE() noexcept
	{
		T value;
		return ReadLE(value) ? value : 0;
	}

	/// Reads an unsigned big endian integral type converted to host byte ordering and advances the read position
	/// @tparam T The unsigned integral type to read
	/// @return The value read or @c 0 on failure
	template <typename T>
	typename std::enable_if<std::is_unsigned<T>::value, T>::type ReadBE() noexcept
	{
		T value;
		return ReadBE(value) ? value : 0;
	}

	/// Reads an unsigned integral type, swaps its byte ordering, and advances the read position
	/// @tparam T The unsigned integral type to read
	/// @return The value read or @c 0 on failure
	template <typename T>
	typename std::enable_if<std::is_unsigned<T>::value, T>::type ReadSwapped() noexcept
	{
		T value;
		return ReadSwapped(value) ? value : 0;
	}

	/// Reads bytes and advances the read position
	/// @param buf The destination buffer or @c nullptr to discard the bytes
	/// @param count The number of bytes to read
	/// @return The number of bytes actually read
	uint32_t Read(void * const _Nullable buf, uint32_t count) noexcept
	{
		auto bytesToCopy = std::min(count, mBufferLength - mReadPosition);
		if(buf)
			std::memcpy(buf, static_cast<const uint8_t *>(mBuffer) + mReadPosition, bytesToCopy);
		mReadPosition += bytesToCopy;
		return bytesToCopy;
	}

	/// Advances the read position
	/// @param count The number of bytes to skip
	/// @return The number of bytes actually skipped
	uint32_t Skip(uint32_t count) noexcept
	{
		mReadPosition += std::min(count, mBufferLength - mReadPosition);
		return mReadPosition;
	}

	/// Rewinds the read position
	/// @param count The number of bytes to rewind
	/// @return The number of bytes actually skipped
	uint32_t Rewind(uint32_t count) noexcept
	{
		auto bytesToSkip = std::min(count, mReadPosition);
		mReadPosition -= bytesToSkip;
		return bytesToSkip;
	}

	/// Returns the number of bytes in the buffer
	/// @return The number of bytes in the buffer
	inline uint32_t Length() const noexcept
	{
		return mBufferLength;
	}

	/// Returns the number of bytes remaining
	inline uint32_t Remaining() const noexcept
	{
		return mBufferLength - mReadPosition;
	}

	/// Returns the read position
	/// @return The read posiiton
	inline uint32_t Position() const noexcept
	{
		return mReadPosition;
	}

	/// Sets the read position
	/// @param pos The desired read position
	/// @return The new read posiiton
	inline uint32_t SetPosition(uint32_t pos) noexcept
	{
		mReadPosition = std::min(pos, mBufferLength);
		return mReadPosition;
	}

private:

	/// The wrapped buffer
	const void * _Nullable mBuffer;
	/// The number of bytes in @c mBuffer
	uint32_t mBufferLength;
	/// The current read position
	uint32_t mReadPosition;

};

} // namespace SFB
