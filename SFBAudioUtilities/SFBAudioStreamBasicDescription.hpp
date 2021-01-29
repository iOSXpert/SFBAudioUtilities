/*
 * Copyright (c) 2014 - 2021 Stephen F. Booth <me@sbooth.org>
 * MIT license
 */

#pragma once

#import <cstring>

#import <CoreAudio/CoreAudioTypes.h>

#import "SFBCFWrapper.hpp"

/*! @brief Common PCM audio formats */
typedef CF_ENUM(uint32_t, SFBCommonPCMFormat) {
	kSFBCommonPCMFormatFloat32 			= 1, 		/*!< Native-endian @c float */
	kSFBCommonPCMFormatFloat64 			= 2, 		/*!< Native-endian @c double */
	kSFBCommonPCMFormatInt16 			= 3, 		/*!< Native-endian signed 16-bit integers */
	kSFBCommonPCMFormatInt32 			= 4, 		/*!< Native-endian signed 32-bit integers */
};

/*! @brief A class extending the functionality of a Core %Audio @c AudioStreamBasicDescription */
class SFBAudioStreamBasicDescription : public AudioStreamBasicDescription
{

public:
	
	// ========================================
	/*! @name Creation and Destruction */
	//@{

	/*! @brief Creates a new, empty @c SFBAudioStreamBasicDescription */
	inline SFBAudioStreamBasicDescription() noexcept
	{
		std::memset(this, 0, sizeof(AudioStreamBasicDescription));
	}

	/*! @brief Creates a new @c SFBAudioStreamBasicDescription for the specified @c AudioStreamBasicDescription */
	inline SFBAudioStreamBasicDescription(const AudioStreamBasicDescription& asbd) noexcept
	{
		std::memcpy(this, &asbd, sizeof(AudioStreamBasicDescription));
	}

	/*! @brief Creates a new @c SFBAudioStreamBasicDescription for the specified @c AudioStreamBasicDescription */
	inline SFBAudioStreamBasicDescription(const AudioStreamBasicDescription * const asbd) noexcept
	{
		assert(asbd != nullptr);
		std::memcpy(this, asbd, sizeof(AudioStreamBasicDescription));
	}

	/*! @brief Creates a new @c SFBAudioStreamBasicDescription for the speciifed @c SFBCommonPCMFormat */
	SFBAudioStreamBasicDescription(SFBCommonPCMFormat commonPCMFormat, Float32 sampleRate, UInt32 channelsPerFrame, bool isInterleaved) noexcept;

	/*! @brief Copy constructor */
	inline SFBAudioStreamBasicDescription(const SFBAudioStreamBasicDescription& rhs) noexcept
	{
		*this = rhs;
	}

	/*! @brief Assignment operator */
	inline SFBAudioStreamBasicDescription& operator=(const SFBAudioStreamBasicDescription& rhs) noexcept
	{
		if(this != &rhs)
			std::memcpy(this, &rhs, sizeof(AudioStreamBasicDescription));
		return *this;
	}

	/*! @brief Returns @c true if @c rhs is equal to @c this */
	inline bool operator==(const SFBAudioStreamBasicDescription& rhs) const noexcept
	{
		return !memcmp(this, &rhs, sizeof(AudioStreamBasicDescription));
	}

	/*! @brief Returns @c true if @c rhs is not equal to @c this */
	inline bool operator!=(const SFBAudioStreamBasicDescription& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	//@}


	// ========================================
	/*! @name Format information */
	//@{

	/*! @brief Returns @c true if this format is interleaved */
	inline bool IsInterleaved() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsNonInterleaved) == 0;
	}

	/*! @brief Returns the number of interleaved channels */
	inline UInt32 InterleavedChannelCount() const noexcept
	{
		return IsInterleaved() ? mChannelsPerFrame : 1;
	}

	/*! @brief Returns the number of channel streams */
	inline UInt32 ChannelStreamCount() const noexcept
	{
		return IsInterleaved() ? 1 : mChannelsPerFrame;
	}

	/*! @brief Returns the number of channels */
	inline UInt32 ChannelCount() const noexcept
	{
		return mChannelsPerFrame;
	}

	/*! @brief Returns @c true if this format is PCM */
	inline bool IsPCM() const noexcept
	{
		return kAudioFormatLinearPCM == mFormatID;
	}

	/*! @brief Returns @c true if this format is big-endian */
	inline bool IsBigEndian() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsBigEndian) == kAudioFormatFlagIsBigEndian;
	}

	/*! @brief Returns @c true if this format is little-endian */
	inline bool IsLittleEndian() const noexcept
	{
		return !IsBigEndian();
	}

	/*! @brief Returns @c true if this format is native-endian */
	inline bool IsNativeEndian() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsBigEndian) == kAudioFormatFlagsNativeEndian;
	}

	/*! @brief Returns @c true if this format is floating-point */
	inline bool IsFloat() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsFloat) == kAudioFormatFlagIsFloat;
	}

	/*! @brief Returns @c true if this format is signed integer */
	inline bool IsSignedInteger() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsSignedInteger) == kAudioFormatFlagIsSignedInteger;
	}

	/*! @brief Returns @c true if this format is packed */
	inline bool IsPacked() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsPacked) == kAudioFormatFlagIsPacked;
	}

	/*! @brief Returns @c true if this format ia high-aligned */
	inline bool IsAlignedHigh() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsAlignedHigh) == kAudioFormatFlagIsAlignedHigh;
	}

	//@}


	// ========================================
	/*! @name Format transformation */
	//@{

	/*! @brief Sets @c format to the equivalent non-interleaved format of @c this. Fails for non-PCM formats. */
	bool GetNonInterleavedEquivalent(SFBAudioStreamBasicDescription& format) const noexcept;

	/*! @brief Sets @c format to the equivalent interleaved format of @c this. Fails for non-PCM formats. */
	bool GetInterleavedEquivalent(SFBAudioStreamBasicDescription& format) const noexcept;

	/*! @brief Sets @c format to the equivalent standard format of @c this. Fails for non-PCM formats. */
	bool GetStandardEquivalent(SFBAudioStreamBasicDescription& format) const noexcept;

	//@}


	/*! @brief Returns a string representation of this format suitable for logging */
	SFBCFString Description(const char * const prefix = nullptr) const noexcept;

};
