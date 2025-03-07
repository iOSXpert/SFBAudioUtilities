//
// Copyright (c) 2013 - 2023 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/SFBAudioUtilities
// MIT license
//

#import <cstdlib>
#import <cstring>
#import <new>

#import <AudioToolbox/AudioFormat.h>

#import "SFBCAChannelLayout.hpp"

namespace {

/// Returns the size in bytes of an @c AudioChannelLayout with the specified number of channel descriptions
constexpr size_t ChannelLayoutSize(UInt32 numberChannelDescriptions) noexcept
{
	return offsetof(AudioChannelLayout, mChannelDescriptions) + (numberChannelDescriptions * sizeof(AudioChannelDescription));
}

/// Allocates an @c AudioChannelLayout
/// @param numberChannelDescriptions The number of channel descriptions that will be stored in the channel layout
/// @return An @c AudioChannelLayout
/// @throws @c std::bad_alloc
AudioChannelLayout * CreateChannelLayout(UInt32 numberChannelDescriptions)
{
	size_t layoutSize = ChannelLayoutSize(numberChannelDescriptions);
	AudioChannelLayout *channelLayout = static_cast<AudioChannelLayout *>(std::malloc(layoutSize));
	if(!channelLayout)
		throw std::bad_alloc();

	std::memset(channelLayout, 0, layoutSize);

	channelLayout->mNumberChannelDescriptions = numberChannelDescriptions;

	return channelLayout;
}

/// Creates and returns a copy of @c rhs
/// @param rhs The @c AudioChannelLayout to copy
/// @return An @c AudioChannelLayout
/// @throws @c std::bad_alloc
AudioChannelLayout * CopyChannelLayout(const AudioChannelLayout * _Nullable rhs)
{
	if(!rhs)
		return nullptr;

	size_t layoutSize = ChannelLayoutSize(rhs->mNumberChannelDescriptions);
	AudioChannelLayout *channelLayout = static_cast<AudioChannelLayout *>(std::malloc(layoutSize));
	if(!channelLayout)
		throw std::bad_alloc();

	std::memcpy(channelLayout, rhs, layoutSize);

	return channelLayout;
}

/// Returns the string representation of an @c AudioChannelLayoutTag
constexpr const char * GetChannelLayoutTagName(AudioChannelLayoutTag layoutTag) noexcept
{
	switch(layoutTag) {
		case kAudioChannelLayoutTag_Mono:					return "kAudioChannelLayoutTag_Mono";
		case kAudioChannelLayoutTag_Stereo:					return "kAudioChannelLayoutTag_Stereo";
		case kAudioChannelLayoutTag_StereoHeadphones:		return "kAudioChannelLayoutTag_StereoHeadphones";
		case kAudioChannelLayoutTag_MatrixStereo:			return "kAudioChannelLayoutTag_MatrixStereo";
		case kAudioChannelLayoutTag_MidSide:				return "kAudioChannelLayoutTag_MidSide";
		case kAudioChannelLayoutTag_XY:						return "kAudioChannelLayoutTag_XY";
		case kAudioChannelLayoutTag_Binaural:				return "kAudioChannelLayoutTag_Binaural";
		case kAudioChannelLayoutTag_Ambisonic_B_Format:		return "kAudioChannelLayoutTag_Ambisonic_B_Format";
		case kAudioChannelLayoutTag_Quadraphonic:			return "kAudioChannelLayoutTag_Quadraphonic";
		case kAudioChannelLayoutTag_Pentagonal:				return "kAudioChannelLayoutTag_Pentagonal";
		case kAudioChannelLayoutTag_Hexagonal:				return "kAudioChannelLayoutTag_Hexagonal";
		case kAudioChannelLayoutTag_Octagonal:				return "kAudioChannelLayoutTag_Octagonal";
		case kAudioChannelLayoutTag_Cube:					return "kAudioChannelLayoutTag_Cube";
		case kAudioChannelLayoutTag_MPEG_3_0_A:				return "kAudioChannelLayoutTag_MPEG_3_0_A";
		case kAudioChannelLayoutTag_MPEG_3_0_B:				return "kAudioChannelLayoutTag_MPEG_3_0_B";
		case kAudioChannelLayoutTag_MPEG_4_0_A:				return "kAudioChannelLayoutTag_MPEG_4_0_A";
		case kAudioChannelLayoutTag_MPEG_4_0_B:				return "kAudioChannelLayoutTag_MPEG_4_0_B";
		case kAudioChannelLayoutTag_MPEG_5_0_A:				return "kAudioChannelLayoutTag_MPEG_5_0_A";
		case kAudioChannelLayoutTag_MPEG_5_0_B:				return "kAudioChannelLayoutTag_MPEG_5_0_B";
		case kAudioChannelLayoutTag_MPEG_5_0_C:				return "kAudioChannelLayoutTag_MPEG_5_0_C";
		case kAudioChannelLayoutTag_MPEG_5_0_D:				return "kAudioChannelLayoutTag_MPEG_5_0_D";
		case kAudioChannelLayoutTag_MPEG_5_1_A:				return "kAudioChannelLayoutTag_MPEG_5_1_A";
		case kAudioChannelLayoutTag_MPEG_5_1_B:				return "kAudioChannelLayoutTag_MPEG_5_1_B";
		case kAudioChannelLayoutTag_MPEG_5_1_C:				return "kAudioChannelLayoutTag_MPEG_5_1_C";
		case kAudioChannelLayoutTag_MPEG_5_1_D:				return "kAudioChannelLayoutTag_MPEG_5_1_D";
		case kAudioChannelLayoutTag_MPEG_6_1_A:				return "kAudioChannelLayoutTag_MPEG_6_1_A";
		case kAudioChannelLayoutTag_MPEG_7_1_A:				return "kAudioChannelLayoutTag_MPEG_7_1_A";
		case kAudioChannelLayoutTag_MPEG_7_1_B:				return "kAudioChannelLayoutTag_MPEG_7_1_B";
		case kAudioChannelLayoutTag_MPEG_7_1_C:				return "kAudioChannelLayoutTag_MPEG_7_1_C";
		case kAudioChannelLayoutTag_Emagic_Default_7_1:		return "kAudioChannelLayoutTag_Emagic_Default_7_1";
		case kAudioChannelLayoutTag_SMPTE_DTV:				return "kAudioChannelLayoutTag_SMPTE_DTV";
		case kAudioChannelLayoutTag_ITU_2_1:				return "kAudioChannelLayoutTag_ITU_2_1";
		case kAudioChannelLayoutTag_ITU_2_2:				return "kAudioChannelLayoutTag_ITU_2_2";
		case kAudioChannelLayoutTag_DVD_4:					return "kAudioChannelLayoutTag_DVD_4";
		case kAudioChannelLayoutTag_DVD_5:					return "kAudioChannelLayoutTag_DVD_5";
		case kAudioChannelLayoutTag_DVD_6:					return "kAudioChannelLayoutTag_DVD_6";
		case kAudioChannelLayoutTag_DVD_10:					return "kAudioChannelLayoutTag_DVD_10";
		case kAudioChannelLayoutTag_DVD_11:					return "kAudioChannelLayoutTag_DVD_11";
		case kAudioChannelLayoutTag_DVD_18:					return "kAudioChannelLayoutTag_DVD_18";
		case kAudioChannelLayoutTag_AudioUnit_6_0:			return "kAudioChannelLayoutTag_AudioUnit_6_0";
		case kAudioChannelLayoutTag_AudioUnit_7_0:			return "kAudioChannelLayoutTag_AudioUnit_7_0";
		case kAudioChannelLayoutTag_AudioUnit_7_0_Front:	return "kAudioChannelLayoutTag_AudioUnit_7_0_Front";
		case kAudioChannelLayoutTag_AAC_6_0:				return "kAudioChannelLayoutTag_AAC_6_0";
		case kAudioChannelLayoutTag_AAC_6_1:				return "kAudioChannelLayoutTag_AAC_6_1";
		case kAudioChannelLayoutTag_AAC_7_0:				return "kAudioChannelLayoutTag_AAC_7_0";
		case kAudioChannelLayoutTag_AAC_7_1_B:				return "kAudioChannelLayoutTag_AAC_7_1_B";
		case kAudioChannelLayoutTag_AAC_7_1_C:				return "kAudioChannelLayoutTag_AAC_7_1_C";
		case kAudioChannelLayoutTag_AAC_Octagonal:			return "kAudioChannelLayoutTag_AAC_Octagonal";
		case kAudioChannelLayoutTag_TMH_10_2_std:			return "kAudioChannelLayoutTag_TMH_10_2_std";
		case kAudioChannelLayoutTag_TMH_10_2_full:			return "kAudioChannelLayoutTag_TMH_10_2_full";
		case kAudioChannelLayoutTag_AC3_1_0_1:				return "kAudioChannelLayoutTag_AC3_1_0_1";
		case kAudioChannelLayoutTag_AC3_3_0:				return "kAudioChannelLayoutTag_AC3_3_0";
		case kAudioChannelLayoutTag_AC3_3_1:				return "kAudioChannelLayoutTag_AC3_3_1";
		case kAudioChannelLayoutTag_AC3_3_0_1:				return "kAudioChannelLayoutTag_AC3_3_0_1";
		case kAudioChannelLayoutTag_AC3_2_1_1:				return "kAudioChannelLayoutTag_AC3_2_1_1";
		case kAudioChannelLayoutTag_AC3_3_1_1:				return "kAudioChannelLayoutTag_AC3_3_1_1";
		case kAudioChannelLayoutTag_EAC_6_0_A:				return "kAudioChannelLayoutTag_EAC_6_0_A";
		case kAudioChannelLayoutTag_EAC_7_0_A:				return "kAudioChannelLayoutTag_EAC_7_0_A";
		case kAudioChannelLayoutTag_EAC3_6_1_A:				return "kAudioChannelLayoutTag_EAC3_6_1_A";
		case kAudioChannelLayoutTag_EAC3_6_1_B:				return "kAudioChannelLayoutTag_EAC3_6_1_B";
		case kAudioChannelLayoutTag_EAC3_6_1_C:				return "kAudioChannelLayoutTag_EAC3_6_1_C";
		case kAudioChannelLayoutTag_EAC3_7_1_A:				return "kAudioChannelLayoutTag_EAC3_7_1_A";
		case kAudioChannelLayoutTag_EAC3_7_1_B:				return "kAudioChannelLayoutTag_EAC3_7_1_B";
		case kAudioChannelLayoutTag_EAC3_7_1_C:				return "kAudioChannelLayoutTag_EAC3_7_1_C";
		case kAudioChannelLayoutTag_EAC3_7_1_D:				return "kAudioChannelLayoutTag_EAC3_7_1_D";
		case kAudioChannelLayoutTag_EAC3_7_1_E:				return "kAudioChannelLayoutTag_EAC3_7_1_E";
		case kAudioChannelLayoutTag_EAC3_7_1_F:				return "kAudioChannelLayoutTag_EAC3_7_1_F";
		case kAudioChannelLayoutTag_EAC3_7_1_G:				return "kAudioChannelLayoutTag_EAC3_7_1_G";
		case kAudioChannelLayoutTag_EAC3_7_1_H:				return "kAudioChannelLayoutTag_EAC3_7_1_H";
		case kAudioChannelLayoutTag_DTS_3_1:				return "kAudioChannelLayoutTag_DTS_3_1";
		case kAudioChannelLayoutTag_DTS_4_1:				return "kAudioChannelLayoutTag_DTS_4_1";
		case kAudioChannelLayoutTag_DTS_6_0_A:				return "kAudioChannelLayoutTag_DTS_6_0_A";
		case kAudioChannelLayoutTag_DTS_6_0_B:				return "kAudioChannelLayoutTag_DTS_6_0_B";
		case kAudioChannelLayoutTag_DTS_6_0_C:				return "kAudioChannelLayoutTag_DTS_6_0_C";
		case kAudioChannelLayoutTag_DTS_6_1_A:				return "kAudioChannelLayoutTag_DTS_6_1_A";
		case kAudioChannelLayoutTag_DTS_6_1_B:				return "kAudioChannelLayoutTag_DTS_6_1_B";
		case kAudioChannelLayoutTag_DTS_6_1_C:				return "kAudioChannelLayoutTag_DTS_6_1_C";
		case kAudioChannelLayoutTag_DTS_7_0:				return "kAudioChannelLayoutTag_DTS_7_0";
		case kAudioChannelLayoutTag_DTS_7_1:				return "kAudioChannelLayoutTag_DTS_7_1";
		case kAudioChannelLayoutTag_DTS_8_0_A:				return "kAudioChannelLayoutTag_DTS_8_0_A";
		case kAudioChannelLayoutTag_DTS_8_0_B:				return "kAudioChannelLayoutTag_DTS_8_0_B";
		case kAudioChannelLayoutTag_DTS_8_1_A:				return "kAudioChannelLayoutTag_DTS_8_1_A";
		case kAudioChannelLayoutTag_DTS_8_1_B:				return "kAudioChannelLayoutTag_DTS_8_1_B";
		case kAudioChannelLayoutTag_DTS_6_1_D:				return "kAudioChannelLayoutTag_DTS_6_1_D";
		case kAudioChannelLayoutTag_WAVE_4_0_B:				return "kAudioChannelLayoutTag_WAVE_4_0_B";
		case kAudioChannelLayoutTag_WAVE_5_0_B:				return "kAudioChannelLayoutTag_WAVE_5_0_B";
		case kAudioChannelLayoutTag_WAVE_5_1_B:				return "kAudioChannelLayoutTag_WAVE_5_1_B";
		case kAudioChannelLayoutTag_WAVE_6_1:				return "kAudioChannelLayoutTag_WAVE_6_1";
		case kAudioChannelLayoutTag_WAVE_7_1:				return "kAudioChannelLayoutTag_WAVE_7_1";
		case kAudioChannelLayoutTag_Atmos_5_1_2:			return "kAudioChannelLayoutTag_Atmos_5_1_2";
			//			case kAudioChannelLayoutTag_Atmos_5_1_4:			return "kAudioChannelLayoutTag_Atmos_5_1_4";
			//			case kAudioChannelLayoutTag_Atmos_7_1_2:			return "kAudioChannelLayoutTag_Atmos_7_1_2";
		case kAudioChannelLayoutTag_Atmos_7_1_4:			return "kAudioChannelLayoutTag_Atmos_7_1_4";
		case kAudioChannelLayoutTag_Atmos_9_1_6:			return "kAudioChannelLayoutTag_Atmos_9_1_6";
	}

	switch(layoutTag & 0xFFFF0000) {
		case kAudioChannelLayoutTag_HOA_ACN_SN3D:			return "kAudioChannelLayoutTag_HOA_ACN_SN3D";
		case kAudioChannelLayoutTag_HOA_ACN_N3D:			return "kAudioChannelLayoutTag_HOA_ACN_N3D";
		case kAudioChannelLayoutTag_DiscreteInOrder:		return "kAudioChannelLayoutTag_DiscreteInOrder";
		case kAudioChannelLayoutTag_Unknown:				return "kAudioChannelLayoutTag_Unknown";
	}

	return nullptr;
}

/// Returns the string representation of an @c AudioChannelLabel
constexpr const char * GetChannelLabelName(AudioChannelLabel label) noexcept
{
	switch(label) {
		case kAudioChannelLabel_Unknown:					return "kAudioChannelLabel_Unknown";
		case kAudioChannelLabel_Unused:						return "kAudioChannelLabel_Unused";
		case kAudioChannelLabel_UseCoordinates:				return "kAudioChannelLabel_UseCoordinates";
		case kAudioChannelLabel_Left:						return "kAudioChannelLabel_Left";
		case kAudioChannelLabel_Right:						return "kAudioChannelLabel_Right";
		case kAudioChannelLabel_Center:						return "kAudioChannelLabel_Center";
		case kAudioChannelLabel_LFEScreen:					return "kAudioChannelLabel_LFEScreen";
		case kAudioChannelLabel_LeftSurround:				return "kAudioChannelLabel_LeftSurround";
		case kAudioChannelLabel_RightSurround:				return "kAudioChannelLabel_RightSurround";
		case kAudioChannelLabel_LeftCenter:					return "kAudioChannelLabel_LeftCenter";
		case kAudioChannelLabel_RightCenter:				return "kAudioChannelLabel_RightCenter";
		case kAudioChannelLabel_CenterSurround:				return "kAudioChannelLabel_CenterSurround";
		case kAudioChannelLabel_LeftSurroundDirect:			return "kAudioChannelLabel_LeftSurroundDirect";
		case kAudioChannelLabel_RightSurroundDirect:		return "kAudioChannelLabel_RightSurroundDirect";
		case kAudioChannelLabel_TopCenterSurround:			return "kAudioChannelLabel_TopCenterSurround";
		case kAudioChannelLabel_VerticalHeightLeft:			return "kAudioChannelLabel_VerticalHeightLeft";
		case kAudioChannelLabel_VerticalHeightCenter:		return "kAudioChannelLabel_VerticalHeightCenter";
		case kAudioChannelLabel_VerticalHeightRight:		return "kAudioChannelLabel_VerticalHeightRight";
		case kAudioChannelLabel_TopBackLeft:				return "kAudioChannelLabel_TopBackLeft";
		case kAudioChannelLabel_TopBackCenter:				return "kAudioChannelLabel_TopBackCenter";
		case kAudioChannelLabel_TopBackRight:				return "kAudioChannelLabel_TopBackRight";
		case kAudioChannelLabel_RearSurroundLeft:			return "kAudioChannelLabel_RearSurroundLeft";
		case kAudioChannelLabel_RearSurroundRight:			return "kAudioChannelLabel_RearSurroundRight";
		case kAudioChannelLabel_LeftWide:					return "kAudioChannelLabel_LeftWide";
		case kAudioChannelLabel_RightWide:					return "kAudioChannelLabel_RightWide";
		case kAudioChannelLabel_LFE2:						return "kAudioChannelLabel_LFE2";
		case kAudioChannelLabel_LeftTotal:					return "kAudioChannelLabel_LeftTotal";
		case kAudioChannelLabel_RightTotal:					return "kAudioChannelLabel_RightTotal";
		case kAudioChannelLabel_HearingImpaired:			return "kAudioChannelLabel_HearingImpaired";
		case kAudioChannelLabel_Narration:					return "kAudioChannelLabel_Narration";
		case kAudioChannelLabel_Mono:						return "kAudioChannelLabel_Mono";
		case kAudioChannelLabel_DialogCentricMix:			return "kAudioChannelLabel_DialogCentricMix";
		case kAudioChannelLabel_CenterSurroundDirect:		return "kAudioChannelLabel_CenterSurroundDirect";
		case kAudioChannelLabel_Haptic:						return "kAudioChannelLabel_Haptic";
		case kAudioChannelLabel_LeftTopMiddle:				return "kAudioChannelLabel_LeftTopMiddle";
		case kAudioChannelLabel_RightTopMiddle:				return "kAudioChannelLabel_RightTopMiddle";
		case kAudioChannelLabel_LeftTopRear:				return "kAudioChannelLabel_LeftTopRear";
		case kAudioChannelLabel_CenterTopRear:				return "kAudioChannelLabel_CenterTopRear";
		case kAudioChannelLabel_RightTopRear:				return "kAudioChannelLabel_RightTopRear";
		case kAudioChannelLabel_Ambisonic_W:				return "kAudioChannelLabel_Ambisonic_W";
		case kAudioChannelLabel_Ambisonic_X:				return "kAudioChannelLabel_Ambisonic_X";
		case kAudioChannelLabel_Ambisonic_Y:				return "kAudioChannelLabel_Ambisonic_Y";
		case kAudioChannelLabel_Ambisonic_Z:				return "kAudioChannelLabel_Ambisonic_Z";
		case kAudioChannelLabel_MS_Mid:						return "kAudioChannelLabel_MS_Mid";
		case kAudioChannelLabel_MS_Side:					return "kAudioChannelLabel_MS_Side";
		case kAudioChannelLabel_XY_X:						return "kAudioChannelLabel_XY_X";
		case kAudioChannelLabel_XY_Y:						return "kAudioChannelLabel_XY_Y";
		case kAudioChannelLabel_BinauralLeft:				return "kAudioChannelLabel_BinauralLeft";
		case kAudioChannelLabel_BinauralRight:				return "kAudioChannelLabel_BinauralRight";
		case kAudioChannelLabel_HeadphonesLeft:				return "kAudioChannelLabel_HeadphonesLeft";
		case kAudioChannelLabel_HeadphonesRight:			return "kAudioChannelLabel_HeadphonesRight";
		case kAudioChannelLabel_ClickTrack:					return "kAudioChannelLabel_ClickTrack";
		case kAudioChannelLabel_ForeignLanguage:			return "kAudioChannelLabel_ForeignLanguage";
		case kAudioChannelLabel_Discrete:					return "kAudioChannelLabel_Discrete";
		case kAudioChannelLabel_Discrete_0:					return "kAudioChannelLabel_Discrete_0";
		case kAudioChannelLabel_Discrete_1:					return "kAudioChannelLabel_Discrete_1";
		case kAudioChannelLabel_Discrete_2:					return "kAudioChannelLabel_Discrete_2";
		case kAudioChannelLabel_Discrete_3:					return "kAudioChannelLabel_Discrete_3";
		case kAudioChannelLabel_Discrete_4:					return "kAudioChannelLabel_Discrete_4";
		case kAudioChannelLabel_Discrete_5:					return "kAudioChannelLabel_Discrete_5";
		case kAudioChannelLabel_Discrete_6:					return "kAudioChannelLabel_Discrete_6";
		case kAudioChannelLabel_Discrete_7:					return "kAudioChannelLabel_Discrete_7";
		case kAudioChannelLabel_Discrete_8:					return "kAudioChannelLabel_Discrete_8";
		case kAudioChannelLabel_Discrete_9:					return "kAudioChannelLabel_Discrete_9";
		case kAudioChannelLabel_Discrete_10:				return "kAudioChannelLabel_Discrete_10";
		case kAudioChannelLabel_Discrete_11:				return "kAudioChannelLabel_Discrete_11";
		case kAudioChannelLabel_Discrete_12:				return "kAudioChannelLabel_Discrete_12";
		case kAudioChannelLabel_Discrete_13:				return "kAudioChannelLabel_Discrete_13";
		case kAudioChannelLabel_Discrete_14:				return "kAudioChannelLabel_Discrete_14";
		case kAudioChannelLabel_Discrete_15:				return "kAudioChannelLabel_Discrete_15";
		case kAudioChannelLabel_Discrete_65535:				return "kAudioChannelLabel_Discrete_65535";
		case kAudioChannelLabel_HOA_ACN:					return "kAudioChannelLabel_HOA_ACN";
		case kAudioChannelLabel_HOA_ACN_0:					return "kAudioChannelLabel_HOA_ACN_0";
		case kAudioChannelLabel_HOA_ACN_1:					return "kAudioChannelLabel_HOA_ACN_1";
		case kAudioChannelLabel_HOA_ACN_2:					return "kAudioChannelLabel_HOA_ACN_2";
		case kAudioChannelLabel_HOA_ACN_3:					return "kAudioChannelLabel_HOA_ACN_3";
		case kAudioChannelLabel_HOA_ACN_4:					return "kAudioChannelLabel_HOA_ACN_4";
		case kAudioChannelLabel_HOA_ACN_5:					return "kAudioChannelLabel_HOA_ACN_5";
		case kAudioChannelLabel_HOA_ACN_6:					return "kAudioChannelLabel_HOA_ACN_6";
		case kAudioChannelLabel_HOA_ACN_7:					return "kAudioChannelLabel_HOA_ACN_7";
		case kAudioChannelLabel_HOA_ACN_8:					return "kAudioChannelLabel_HOA_ACN_8";
		case kAudioChannelLabel_HOA_ACN_9:					return "kAudioChannelLabel_HOA_ACN_9";
		case kAudioChannelLabel_HOA_ACN_10:					return "kAudioChannelLabel_HOA_ACN_10";
		case kAudioChannelLabel_HOA_ACN_11:					return "kAudioChannelLabel_HOA_ACN_11";
		case kAudioChannelLabel_HOA_ACN_12:					return "kAudioChannelLabel_HOA_ACN_12";
		case kAudioChannelLabel_HOA_ACN_13:					return "kAudioChannelLabel_HOA_ACN_13";
		case kAudioChannelLabel_HOA_ACN_14:					return "kAudioChannelLabel_HOA_ACN_14";
		case kAudioChannelLabel_HOA_ACN_15:					return "kAudioChannelLabel_HOA_ACN_15";
		case kAudioChannelLabel_HOA_ACN_65024:				return "kAudioChannelLabel_HOA_ACN_65024";

		default:											return nullptr;
	}
}

}

size_t SFB::AudioChannelLayoutSize(const AudioChannelLayout *channelLayout) noexcept
{
	if(!channelLayout)
		return 0;
	return ChannelLayoutSize(channelLayout->mNumberChannelDescriptions);
}

// Constants
const SFB::CAChannelLayout SFB::CAChannelLayout::Mono		= CAChannelLayout(kAudioChannelLayoutTag_Mono);
const SFB::CAChannelLayout SFB::CAChannelLayout::Stereo 	= CAChannelLayout(kAudioChannelLayoutTag_Stereo);

SFB::CAChannelLayout SFB::CAChannelLayout::ChannelLayoutWithBitmap(UInt32 channelBitmap)
{
	CAChannelLayout channelLayout{};
	channelLayout.mChannelLayout = CreateChannelLayout(0);
	channelLayout.mChannelLayout->mChannelLayoutTag = kAudioChannelLayoutTag_UseChannelBitmap;
	channelLayout.mChannelLayout->mChannelBitmap = channelBitmap;
	return channelLayout;
}

SFB::CAChannelLayout::CAChannelLayout(const CAChannelLayout& rhs)
: CAChannelLayout{}
{
	*this = rhs;
}

SFB::CAChannelLayout& SFB::CAChannelLayout::operator=(const CAChannelLayout& rhs)
{
	if(this != &rhs) {
		std::free(mChannelLayout);
		mChannelLayout = CopyChannelLayout(rhs.mChannelLayout);
	}
	return *this;
}

SFB::CAChannelLayout::CAChannelLayout(AudioChannelLayoutTag layoutTag)
: mChannelLayout(CreateChannelLayout(0))
{
	mChannelLayout->mChannelLayoutTag = layoutTag;
}

SFB::CAChannelLayout::CAChannelLayout(std::vector<AudioChannelLabel> channelLabels)
: mChannelLayout(CreateChannelLayout(static_cast<UInt32>(channelLabels.size())))
{
	mChannelLayout->mChannelLayoutTag = kAudioChannelLayoutTag_UseChannelDescriptions;
	for(std::vector<AudioChannelLabel>::size_type i = 0; i != channelLabels.size(); ++i)
		mChannelLayout->mChannelDescriptions[i].mChannelLabel = channelLabels[i];
}

SFB::CAChannelLayout::CAChannelLayout(const AudioChannelLayout *rhs)
: mChannelLayout(CopyChannelLayout(rhs))
{}

SFB::CAChannelLayout& SFB::CAChannelLayout::operator=(const AudioChannelLayout *rhs)
{
	std::free(mChannelLayout);
	mChannelLayout = CopyChannelLayout(rhs);
	return *this;
}

bool SFB::CAChannelLayout::operator==(const CAChannelLayout& rhs) const noexcept
{
	// Two empty channel layouts are considered equivalent
	if(!mChannelLayout && !rhs.mChannelLayout)
		return true;

	if(!mChannelLayout || !rhs.mChannelLayout)
		return false;

	const AudioChannelLayout *layouts [] = {
		rhs.mChannelLayout,
		mChannelLayout
	};

	UInt32 layoutsEqual = false;
	UInt32 propertySize = sizeof(layoutsEqual);
	OSStatus result = AudioFormatGetProperty(kAudioFormatProperty_AreChannelLayoutsEquivalent, sizeof(layouts), static_cast<void *>(layouts), &propertySize, &layoutsEqual);

	if(noErr != result)
		return false;
	//os_log_error(OS_LOG_DEFAULT, "AudioFormatGetProperty (kAudioFormatProperty_AreChannelLayoutsEquivalent) failed: %d", result);

	return layoutsEqual;
}

size_t SFB::CAChannelLayout::ChannelCount() const noexcept
{
	if(!mChannelLayout)
		return 0;

	if(mChannelLayout->mChannelLayoutTag == kAudioChannelLayoutTag_UseChannelDescriptions)
		return mChannelLayout->mNumberChannelDescriptions;

	if(mChannelLayout->mChannelLayoutTag == kAudioChannelLayoutTag_UseChannelBitmap)
		return (size_t)__builtin_popcount(mChannelLayout->mChannelBitmap);

	return AudioChannelLayoutTag_GetNumberOfChannels(mChannelLayout->mChannelLayoutTag);
}

bool SFB::CAChannelLayout::MapToLayout(const CAChannelLayout& outputLayout, std::vector<SInt32>& channelMap) const
{
	// No valid map exists for empty/unknown layouts
	if(!mChannelLayout || !outputLayout.mChannelLayout)
		return false;

	const AudioChannelLayout *layouts [] = {
		mChannelLayout,
		outputLayout.mChannelLayout
	};

	auto outputChannelCount = outputLayout.ChannelCount();
	if(outputChannelCount == 0)
		return false;

	SInt32 rawChannelMap [outputChannelCount];
	UInt32 propertySize = (UInt32)sizeof(rawChannelMap);
	OSStatus result = AudioFormatGetProperty(kAudioFormatProperty_ChannelMap, sizeof(layouts), static_cast<void *>(layouts), &propertySize, &rawChannelMap);

	if(noErr != result)
		return false;
	//os_log_error(OS_LOG_DEFAULT, "AudioFormatGetProperty (kAudioFormatProperty_ChannelMap) failed: %d", result);

	auto start = static_cast<SInt32 *>(rawChannelMap);
	channelMap.assign(start, start + outputChannelCount);

	return true;
}

AudioChannelLayout * SFB::CAChannelLayout::RelinquishACL() noexcept
{
	auto channelLayout = mChannelLayout;
	mChannelLayout = nullptr;
	return channelLayout;
}

SFB::CFString SFB::CAChannelLayout::Description(const char * const prefix) const noexcept
{
	if(!mChannelLayout) {
		if(prefix)
			return CFString(prefix, kCFStringEncodingUTF8);
		else
			return CFString{};
	}

	CFMutableString result{ CFStringCreateMutable(kCFAllocatorDefault, 0) };

	if(prefix)
		CFStringAppendCString(result, prefix, kCFStringEncodingUTF8);

	if(kAudioChannelLayoutTag_UseChannelBitmap == mChannelLayout->mChannelLayoutTag)
		CFStringAppendFormat(result, NULL, CFSTR("Channel bitmap: 0x%0.8x"), mChannelLayout->mChannelBitmap);
	else if(kAudioChannelLayoutTag_UseChannelDescriptions == mChannelLayout->mChannelLayoutTag){
		CFStringAppendFormat(result, NULL, CFSTR("%u channels ["), mChannelLayout->mNumberChannelDescriptions);

		const AudioChannelDescription *desc = mChannelLayout->mChannelDescriptions;
		for(UInt32 i = 0; i < mChannelLayout->mNumberChannelDescriptions; ++i, ++desc) {
			if(kAudioChannelLabel_UseCoordinates == desc->mChannelLabel)
				CFStringAppendFormat(result, NULL, CFSTR("(%f, %f, %f), flags = 0x%0.8x"), desc->mCoordinates[0], desc->mCoordinates[1], desc->mCoordinates[2], desc->mChannelFlags);
			else
				CFStringAppendFormat(result, NULL, CFSTR("%s (0x%0.8x)"), GetChannelLabelName(desc->mChannelLabel), desc->mChannelLabel);
			if(i < mChannelLayout->mNumberChannelDescriptions - 1)
				CFStringAppend(result, CFSTR(", "));
		}

		CFStringAppend(result, CFSTR("]"));
	}
	else
		CFStringAppendFormat(result, NULL, CFSTR("%s (0x%0.8x)"), GetChannelLayoutTagName(mChannelLayout->mChannelLayoutTag), mChannelLayout->mChannelLayoutTag);

	return CFString(static_cast<CFStringRef>(result.Relinquish()));
}
