//
// Copyright (c) 2013-2023 The SRS Authors
//
// SPDX-License-Identifier: MIT
//
#include <srs_utest_avc.hpp>

#include <srs_protocol_raw_avc.hpp>
#include <srs_kernel_buffer.hpp>
#include <srs_kernel_error.hpp>
#include <srs_core_autofree.hpp>
#include <srs_kernel_utility.hpp>

VOID TEST(SrsAVCTest, H264ParseAnnexb)
{
    srs_error_t err;

    // Multiple frames.
    if (true) {
        SrsRawH264Stream h;

        uint8_t buf[] = {
            0, 0, 1, 0xd, 0xa, 0xf, 0, 0, 1, 0xa,
        };
        SrsBuffer b((char*)buf, sizeof(buf));

        char* frame = NULL; int nb_frame = 0;
        HELPER_ASSERT_SUCCESS(h.annexb_demux(&b, &frame, &nb_frame));
        EXPECT_EQ(3, nb_frame);
        EXPECT_EQ((char*)(buf+3), frame);

        HELPER_ASSERT_SUCCESS(h.annexb_demux(&b, &frame, &nb_frame));
        EXPECT_EQ(1, nb_frame);
        EXPECT_EQ((char*)(buf+9), frame);
    }

    // N prefix case, should success.
    if (true) {
        SrsRawH264Stream h;

        uint8_t buf[] = {
            0, 0, 0, 1, 0xd, 0xa, 0xf, 0xa,
        };
        SrsBuffer b((char*)buf, sizeof(buf));

        char* frame = NULL; int nb_frame = 0;
        HELPER_ASSERT_SUCCESS(h.annexb_demux(&b, &frame, &nb_frame));
        EXPECT_EQ(4, nb_frame);
        EXPECT_EQ((char*)(buf+4), frame);
    }

    // No prefix, should fail and return an empty frame.
    if (true) {
        SrsRawH264Stream h;

        uint8_t buf[] = {
            0, 0, 2, 0xd, 0xa, 0xf, 0xa,
        };
        SrsBuffer b((char*)buf, sizeof(buf));

        char* frame = NULL; int nb_frame = 0;
        HELPER_ASSERT_FAILED(h.annexb_demux(&b, &frame, &nb_frame));
        EXPECT_EQ(0, nb_frame);
    }

    // No prefix, should fail and return an empty frame.
    if (true) {
        SrsRawH264Stream h;

        uint8_t buf[] = {
            0, 1, 0xd, 0xa, 0xf, 0xa,
        };
        SrsBuffer b((char*)buf, sizeof(buf));

        char* frame = NULL; int nb_frame = 0;
        HELPER_ASSERT_FAILED(h.annexb_demux(&b, &frame, &nb_frame));
        EXPECT_EQ(0, nb_frame);
    }

    // No prefix, should fail and return an empty frame.
    if (true) {
        SrsRawH264Stream h;

        uint8_t buf[] = {
            0xd, 0xa, 0xf, 0xa,
        };
        SrsBuffer b((char*)buf, sizeof(buf));

        char* frame = NULL; int nb_frame = 0;
        HELPER_ASSERT_FAILED(h.annexb_demux(&b, &frame, &nb_frame));
        EXPECT_EQ(0, nb_frame);
    }

    // Normal case, should success.
    if (true) {
        SrsRawH264Stream h;

        uint8_t buf[] = {
            0, 0, 1, 0xd, 0xa, 0xf, 0xa,
        };
        SrsBuffer b((char*)buf, sizeof(buf));

        char* frame = NULL; int nb_frame = 0;
        HELPER_ASSERT_SUCCESS(h.annexb_demux(&b, &frame, &nb_frame));
        EXPECT_EQ(4, nb_frame);
        EXPECT_EQ((char*)(buf+3), frame);
    }
}

VOID TEST(SrsAVCTest, H264SequenceHeader)
{
    srs_error_t err;

    // For muxing sequence header.
    if (true) {
        SrsRawH264Stream h; string sh;
        HELPER_ASSERT_SUCCESS(h.mux_sequence_header("Hello", "world", sh));
        EXPECT_EQ(11+5+5, (int)sh.length());
        EXPECT_STREQ("Hello", sh.substr(8, 5).c_str());
        EXPECT_STREQ("world", sh.substr(16).c_str());
    }

    // For PPS demuxing.
    if (true) {
        SrsRawH264Stream h;
        string pps;
        HELPER_ASSERT_FAILED(h.pps_demux(NULL, 0, pps));
        EXPECT_TRUE(pps.empty());
    }
    if (true) {
        SrsRawH264Stream h;
        string frame = "Hello, world!", pps;
        HELPER_ASSERT_SUCCESS(h.pps_demux((char*)frame.data(), frame.length(), pps));
        EXPECT_STREQ("Hello, world!", pps.c_str());
    }

    // For SPS demuxing.
    if (true) {
        SrsRawH264Stream h;
        string sps;
        HELPER_ASSERT_SUCCESS(h.sps_demux(NULL, 0, sps));
        EXPECT_TRUE(sps.empty());
    }
    if (true) {
        SrsRawH264Stream h;
        string frame = "Hello, world!", sps;
        HELPER_ASSERT_SUCCESS(h.sps_demux((char*)frame.data(), frame.length(), sps));
        EXPECT_STREQ("Hello, world!", sps.c_str());
    }

    // For PPS.
    if (true) {
        SrsRawH264Stream h;
        uint8_t frame[] = {
            0x9,
        };
        EXPECT_FALSE(h.is_pps((char*)frame, sizeof(frame)));
    }
    if (true) {
        SrsRawH264Stream h;
        uint8_t frame[] = {
            0xf8,
        };
        EXPECT_FALSE(h.is_pps((char*)frame, sizeof(frame)));
    }
    if (true) {
        SrsRawH264Stream h;
        uint8_t frame[] = {
            0xe8,
        };
        EXPECT_TRUE(h.is_pps((char*)frame, sizeof(frame)));
    }
    if (true) {
        SrsRawH264Stream h;
        uint8_t frame[] = {
            8,
        };
        EXPECT_TRUE(h.is_pps((char*)frame, sizeof(frame)));
    }

    // For SPS.
    if (true) {
        SrsRawH264Stream h;
        uint8_t frame[] = {
            0x8,
        };
        EXPECT_FALSE(h.is_sps((char*)frame, sizeof(frame)));
    }
    if (true) {
        SrsRawH264Stream h;
        uint8_t frame[] = {
            0xf7,
        };
        EXPECT_FALSE(h.is_sps((char*)frame, sizeof(frame)));
    }
    if (true) {
        SrsRawH264Stream h;
        uint8_t frame[] = {
            0xe7,
        };
        EXPECT_TRUE(h.is_sps((char*)frame, sizeof(frame)));
    }
    if (true) {
        SrsRawH264Stream h;
        uint8_t frame[] = {
            7,
        };
        EXPECT_TRUE(h.is_sps((char*)frame, sizeof(frame)));
    }
}

VOID TEST(SrsAVCTest, H264IPBFrame)
{
    srs_error_t err;

    // For muxing avc to flv frame.
    if (true) {
        SrsRawH264Stream h; int nb_flv = 0; char* flv = NULL;
        string video("Hello"); int8_t frame_type = SrsVideoAvcFrameTypeKeyFrame; int8_t avc_packet_type = SrsVideoAvcFrameTraitSequenceHeader;
        HELPER_ASSERT_SUCCESS(h.mux_avc2flv(video, frame_type, avc_packet_type, 0, 0x010203, &flv, &nb_flv));
        EXPECT_EQ(10, nb_flv);
        EXPECT_EQ(SrsVideoAvcFrameTypeKeyFrame, uint8_t((flv[0]>>4)&0x0f));
        EXPECT_EQ(SrsVideoAvcFrameTraitSequenceHeader, uint8_t(flv[1]));
        EXPECT_EQ(01, flv[2]); EXPECT_EQ(02, flv[3]); EXPECT_EQ(03, flv[4]);
        EXPECT_STREQ("Hello", HELPER_ARR2STR(flv+5, 5).c_str());
        srs_freepa(flv);
    }

    // For muxing I/P/B frame.
    if (true) {
        SrsRawH264Stream h; string frame;
        HELPER_ASSERT_SUCCESS(h.mux_ipb_frame((char*)"Hello", 5, frame));
        EXPECT_EQ(4+5, (int)frame.length());
        EXPECT_EQ(0, (uint8_t)frame.at(0)); EXPECT_EQ(0, (uint8_t)frame.at(1));
        EXPECT_EQ(0, (uint8_t)frame.at(2)); EXPECT_EQ(5, (uint8_t)frame.at(3));
        EXPECT_STREQ("Hello", frame.substr(4).c_str());
    }
}

VOID TEST(SrsAVCTest, AACDemuxADTS)
{
    srs_error_t err;

    // Fail if not adts format.
    if (true) {
        SrsRawAacStream h; char* frame = NULL; int nb_frame = 0; SrsRawAacStreamCodec codec;
        uint8_t b[] = {0xff, 0x09, 0x2c,0x40, 0,0xe0,0}; SrsBuffer buf((char*)b, sizeof(b));
        HELPER_EXPECT_FAILED(h.adts_demux(&buf, &frame, &nb_frame, codec));
    }

    // Fail if less than 7 bytes.
    if (true) {
        SrsRawAacStream h; char* frame = NULL; int nb_frame = 0; SrsRawAacStreamCodec codec;
        uint8_t b[] = {0xff, 0xf9}; SrsBuffer buf((char*)b, sizeof(b));
        HELPER_EXPECT_FAILED(h.adts_demux(&buf, &frame, &nb_frame, codec));
    }

    // For lower sampling rate, such as 5512HZ.
    if (true) {
        SrsRawAacStream h; char* frame = NULL; int nb_frame = 0; SrsRawAacStreamCodec codec;
        uint8_t b[] = {0xff, 0xf9, 0x2c,0x40, 0,0xe0,0}; SrsBuffer buf((char*)b, sizeof(b));
        HELPER_ASSERT_SUCCESS(h.adts_demux(&buf, &frame, &nb_frame, codec));
        EXPECT_EQ(1, codec.protection_absent); // b[1]
        EXPECT_EQ(SrsAacObjectTypeAacMain, codec.aac_object); // b[2]
        EXPECT_EQ(0xb, codec.sampling_frequency_index); // b[2]
        EXPECT_EQ(1, codec.channel_configuration); // b[3]
        EXPECT_EQ(7, codec.frame_length); // b[5]
        EXPECT_EQ(0, nb_frame);

        EXPECT_EQ(SrsAudioSampleRate5512, codec.sound_rate);
        EXPECT_EQ(0, codec.sound_type);
        EXPECT_EQ(1, codec.sound_size);
    }

    // For lower sampling rate, such as 22050HZ.
    if (true) {
        SrsRawAacStream h; char* frame = NULL; int nb_frame = 0; SrsRawAacStreamCodec codec;
        uint8_t b[] = {0xff, 0xf9, 0x18,0x40, 0,0xe0,0}; SrsBuffer buf((char*)b, sizeof(b));
        HELPER_ASSERT_SUCCESS(h.adts_demux(&buf, &frame, &nb_frame, codec));
        EXPECT_EQ(1, codec.protection_absent); // b[1]
        EXPECT_EQ(SrsAacObjectTypeAacMain, codec.aac_object); // b[2]
        EXPECT_EQ(6, codec.sampling_frequency_index); // b[2]
        EXPECT_EQ(1, codec.channel_configuration); // b[3]
        EXPECT_EQ(7, codec.frame_length); // b[5]
        EXPECT_EQ(0, nb_frame);

        EXPECT_EQ(SrsAudioSampleRate22050, codec.sound_rate);
        EXPECT_EQ(0, codec.sound_type);
        EXPECT_EQ(1, codec.sound_size);
    }

    // For higher sampling rate, use 44100HZ.
    if (true) {
        SrsRawAacStream h; char* frame = NULL; int nb_frame = 0; SrsRawAacStreamCodec codec;
        uint8_t b[] = {0xff, 0xf9, 0x04,0x40, 0,0xe0,0}; SrsBuffer buf((char*)b, sizeof(b));
        HELPER_ASSERT_SUCCESS(h.adts_demux(&buf, &frame, &nb_frame, codec));
        EXPECT_EQ(1, codec.protection_absent); // b[1]
        EXPECT_EQ(SrsAacObjectTypeAacMain, codec.aac_object); // b[2]
        EXPECT_EQ(1, codec.sampling_frequency_index); // b[2]
        EXPECT_EQ(1, codec.channel_configuration); // b[3]
        EXPECT_EQ(7, codec.frame_length); // b[5]
        EXPECT_EQ(0, nb_frame);

        EXPECT_EQ(SrsAudioSampleRate44100, codec.sound_rate);
        EXPECT_EQ(0, codec.sound_type);
        EXPECT_EQ(1, codec.sound_size);
    }

    // If protected, there are 2B signature.
    if (true) {
        SrsRawAacStream h; char* frame = NULL; int nb_frame = 0; SrsRawAacStreamCodec codec;
        uint8_t b[] = {0xff, 0xf0, 0x10,0x40, 0x01,0x40,0, 0,0, 1}; SrsBuffer buf((char*)b, sizeof(b));
        HELPER_ASSERT_SUCCESS(h.adts_demux(&buf, &frame, &nb_frame, codec));
        EXPECT_EQ(0, codec.protection_absent); // b[1]
        EXPECT_EQ(SrsAacObjectTypeAacMain, codec.aac_object); // b[2]
        EXPECT_EQ(4, codec.sampling_frequency_index); // b[2]
        EXPECT_EQ(1, codec.channel_configuration); // b[3]
        EXPECT_EQ(10, codec.frame_length); // b[4,5]
        ASSERT_EQ(1, nb_frame); EXPECT_EQ(1, (uint8_t)frame[0]);

        EXPECT_EQ(SrsAudioSampleRate44100, codec.sound_rate);
        EXPECT_EQ(0, codec.sound_type);
        EXPECT_EQ(1, codec.sound_size);
    }

    // Fail if not enough data.
    if (true) {
        SrsRawAacStream h; char* frame = NULL; int nb_frame = 0; SrsRawAacStreamCodec codec;
        uint8_t b[] = {0xff, 0xf0, 0x10,0x40, 0x04,0,0, 1}; SrsBuffer buf((char*)b, sizeof(b));
        HELPER_EXPECT_FAILED(h.adts_demux(&buf, &frame, &nb_frame, codec));
    }

    // If protected, there should be 2B signature.
    if (true) {
        SrsRawAacStream h; char* frame = NULL; int nb_frame = 0; SrsRawAacStreamCodec codec;
        uint8_t b[] = {0xff, 0xf0, 0x10,0x40, 0x01,0,0, 1}; SrsBuffer buf((char*)b, sizeof(b));
        HELPER_EXPECT_FAILED(h.adts_demux(&buf, &frame, &nb_frame, codec));
    }

    // ID should be 1, but we ignored.
    if (true) {
        SrsRawAacStream h; char* frame = NULL; int nb_frame = 0; SrsRawAacStreamCodec codec;
        uint8_t b[] = {0xff, 0xf1, 0x10,0x40, 0x01,0,0, 1}; SrsBuffer buf((char*)b, sizeof(b));
        HELPER_ASSERT_SUCCESS(h.adts_demux(&buf, &frame, &nb_frame, codec));
    }

    // Minimum AAC frame, with raw data.
    if (true) {
        SrsRawAacStream h; char* frame = NULL; int nb_frame = 0; SrsRawAacStreamCodec codec;
        uint8_t b[] = {0xff, 0xf9, 0x10,0x40, 0x01,0,0, 1}; SrsBuffer buf((char*)b, sizeof(b));
        HELPER_ASSERT_SUCCESS(h.adts_demux(&buf, &frame, &nb_frame, codec));
        EXPECT_EQ(1, codec.protection_absent); // b[1]
        EXPECT_EQ(SrsAacObjectTypeAacMain, codec.aac_object); // b[2]
        EXPECT_EQ(4, codec.sampling_frequency_index); // b[2]
        EXPECT_EQ(1, codec.channel_configuration); // b[3]
        EXPECT_EQ(8, codec.frame_length); // b[4]
        ASSERT_EQ(1, nb_frame); EXPECT_EQ(1, (uint8_t)frame[0]);

        EXPECT_EQ(SrsAudioSampleRate44100, codec.sound_rate);
        EXPECT_EQ(0, codec.sound_type);
        EXPECT_EQ(1, codec.sound_size);
    }

    // Minimum AAC frame, no raw data.
    if (true) {
        SrsRawAacStream h; char* frame = NULL; int nb_frame = 0; SrsRawAacStreamCodec codec;
        uint8_t b[] = {0xff, 0xf9, 0x10,0x40, 0,0xe0,0}; SrsBuffer buf((char*)b, sizeof(b));
        HELPER_ASSERT_SUCCESS(h.adts_demux(&buf, &frame, &nb_frame, codec));
        EXPECT_EQ(1, codec.protection_absent); // b[1]
        EXPECT_EQ(SrsAacObjectTypeAacMain, codec.aac_object); // b[2]
        EXPECT_EQ(4, codec.sampling_frequency_index); // b[2]
        EXPECT_EQ(1, codec.channel_configuration); // b[3]
        EXPECT_EQ(7, codec.frame_length); // b[5]
        EXPECT_EQ(0, nb_frame);

        EXPECT_EQ(SrsAudioSampleRate44100, codec.sound_rate);
        EXPECT_EQ(0, codec.sound_type);
        EXPECT_EQ(1, codec.sound_size);
    }
}

VOID TEST(SrsAVCTest, AACMuxSequenceHeader)
{
    srs_error_t err;

    // For sampling rate 22050HZ.
    if (true) {
        SrsRawAacStream h; string sh; SrsRawAacStreamCodec codec;
        codec.aac_object = SrsAacObjectTypeAacMain;
        codec.channel_configuration = 1;
        codec.sound_rate = SrsAudioSampleRate22050;
        codec.sampling_frequency_index = 7;
        HELPER_ASSERT_SUCCESS(h.mux_sequence_header(&codec, sh));
        EXPECT_EQ(2, (int)sh.length());
        EXPECT_EQ(0x0b, (uint8_t)sh.at(0));
        EXPECT_EQ(0x88, (uint8_t)sh.at(1));
    }

    // For sampling rate 11025HZ.
    if (true) {
        SrsRawAacStream h; string sh; SrsRawAacStreamCodec codec;
        codec.aac_object = SrsAacObjectTypeAacMain;
        codec.channel_configuration = 1;
        codec.sound_rate = SrsAudioSampleRate11025;
        codec.sampling_frequency_index = 0xa;
        HELPER_ASSERT_SUCCESS(h.mux_sequence_header(&codec, sh));
        EXPECT_EQ(2, (int)sh.length());
        EXPECT_EQ(0x0d, (uint8_t)sh.at(0));
        EXPECT_EQ(0x08, (uint8_t)sh.at(1));
    }

    // Fail for invalid sampling rate.
    if (true) {
        SrsRawAacStream h; string sh; SrsRawAacStreamCodec codec;
        codec.aac_object = SrsAacObjectTypeAacMain;
        codec.sampling_frequency_index = SrsAacSampleRateUnset;
        codec.sound_rate = SrsAudioSampleRateReserved;
        HELPER_EXPECT_FAILED(h.mux_sequence_header(&codec, sh));
    }

    // Normal case.
    if (true) {
        SrsRawAacStream h; string sh; SrsRawAacStreamCodec codec;
        codec.aac_object = SrsAacObjectTypeAacMain;
        codec.channel_configuration = 1;
        codec.sampling_frequency_index = 4;
        codec.sound_rate = SrsAudioSampleRateReserved;
        HELPER_ASSERT_SUCCESS(h.mux_sequence_header(&codec, sh));
        EXPECT_EQ(2, (int)sh.length());
        EXPECT_EQ(0x0a, (uint8_t)sh.at(0));
        EXPECT_EQ(0x08, (uint8_t)sh.at(1));
    }

    // Fail for invalid aac object.
    if (true) {
        SrsRawAacStream h; string sh; SrsRawAacStreamCodec codec;
        codec.aac_object = SrsAacObjectTypeReserved;
        HELPER_EXPECT_FAILED(h.mux_sequence_header(&codec, sh));
    }

    // Normal case.
    if (true) {
        SrsRawAacStream h; string sh; SrsRawAacStreamCodec codec;
        codec.aac_object = SrsAacObjectTypeAacMain;
        codec.channel_configuration = 1;
        codec.sound_rate = SrsAudioSampleRate44100;
        codec.sampling_frequency_index = 4;
        HELPER_ASSERT_SUCCESS(h.mux_sequence_header(&codec, sh));
        EXPECT_EQ(2, (int)sh.length());
        EXPECT_EQ(0x0a, (uint8_t)sh.at(0));
        EXPECT_EQ(0x08, (uint8_t)sh.at(1));
    }

    // We ignored the sound_rate.
    if (true) {
        SrsRawAacStream h; string sh; SrsRawAacStreamCodec codec;
        codec.aac_object = SrsAacObjectTypeAacMain;
        codec.channel_configuration = 1;
        codec.sound_rate = SrsAudioSampleRate22050;
        codec.sampling_frequency_index = 4;
        HELPER_ASSERT_SUCCESS(h.mux_sequence_header(&codec, sh));
        EXPECT_EQ(2, (int)sh.length());
        EXPECT_EQ(0x0a, (uint8_t)sh.at(0));
        EXPECT_EQ(0x08, (uint8_t)sh.at(1));
    }

    // Use sound_rate if sampling_frequency_index not set.
    if (true) {
        SrsRawAacStream h; string sh; SrsRawAacStreamCodec codec;
        codec.aac_object = SrsAacObjectTypeAacMain;
        codec.channel_configuration = 1;
        codec.sound_rate = SrsAudioSampleRate44100;
        codec.sampling_frequency_index = SrsAacSampleRateUnset;
        HELPER_ASSERT_SUCCESS(h.mux_sequence_header(&codec, sh));
        EXPECT_EQ(2, (int)sh.length());
        EXPECT_EQ(0x0a, (uint8_t)sh.at(0));
        EXPECT_EQ(0x08, (uint8_t)sh.at(1));
    }
}

VOID TEST(SrsAVCTest, AACMuxToFLV)
{
    srs_error_t err;

    // For MP3 frame.
    if (true) {
        SrsRawAacStream h;
        string frame("Hello"); SrsRawAacStreamCodec codec;
        char* flv = NULL; int nb_flv = 0;
        codec.sound_format = SrsAudioCodecIdMP3;
        codec.sound_rate = 0; codec.sound_size = 1; codec.sound_type = 1;
        codec.aac_packet_type = 4;
        HELPER_ASSERT_SUCCESS(h.mux_aac2flv((char*)frame.data(), frame.length(), &codec, 0, &flv, &nb_flv));
        EXPECT_EQ(6, nb_flv);
        EXPECT_EQ(0x23, (uint8_t)flv[0]);
        EXPECT_STREQ("Hello", HELPER_ARR2STR(flv+1,5).c_str());
        srs_freepa(flv);
    }

    // For Opus frame.
    if (true) {
        SrsRawAacStream h;
        string frame("Hello"); SrsRawAacStreamCodec codec;
        char* flv = NULL; int nb_flv = 0;
        codec.sound_format = SrsAudioCodecIdOpus;
        codec.sound_rate = 0; codec.sound_size = 1; codec.sound_type = 1;
        codec.aac_packet_type = 4;
        HELPER_ASSERT_SUCCESS(h.mux_aac2flv((char*)frame.data(), frame.length(), &codec, 0, &flv, &nb_flv));
        EXPECT_EQ(6, nb_flv);
        EXPECT_EQ(0xd3, (uint8_t)flv[0]);
        EXPECT_STREQ("Hello", HELPER_ARR2STR(flv+1,5).c_str());
        srs_freepa(flv);
    }

    // For Speex frame.
    if (true) {
        SrsRawAacStream h;
        string frame("Hello"); SrsRawAacStreamCodec codec;
        char* flv = NULL; int nb_flv = 0;
        codec.sound_format = SrsAudioCodecIdSpeex;
        codec.sound_rate = 0; codec.sound_size = 1; codec.sound_type = 1;
        codec.aac_packet_type = 4;
        HELPER_ASSERT_SUCCESS(h.mux_aac2flv((char*)frame.data(), frame.length(), &codec, 0, &flv, &nb_flv));
        EXPECT_EQ(6, nb_flv);
        EXPECT_EQ(0xb3, (uint8_t)flv[0]);
        EXPECT_STREQ("Hello", HELPER_ARR2STR(flv+1,5).c_str());
        srs_freepa(flv);
    }

    // For AAC frame.
    if (true) {
        SrsRawAacStream h;
        string frame("Hello"); SrsRawAacStreamCodec codec;
        char* flv = NULL; int nb_flv = 0;
        codec.sound_format = SrsAudioCodecIdAAC;
        codec.sound_rate = 0; codec.sound_size = 1; codec.sound_type = 1;
        codec.aac_packet_type = 4;
        HELPER_ASSERT_SUCCESS(h.mux_aac2flv((char*)frame.data(), frame.length(), &codec, 0, &flv, &nb_flv));
        EXPECT_EQ(7, nb_flv);
        EXPECT_EQ(0xa3, (uint8_t)flv[0]);
        EXPECT_EQ(0x04, (uint8_t)flv[1]);
        EXPECT_STREQ("Hello", HELPER_ARR2STR(flv+2,5).c_str());
        srs_freepa(flv);
    }
    if (true) {
        SrsRawAacStream h;
        string frame("Hello"); SrsRawAacStreamCodec codec;
        char* flv = NULL; int nb_flv = 0;
        codec.sound_format = SrsAudioCodecIdAAC;
        codec.sound_rate = 1; codec.sound_size = 1; codec.sound_type = 0;
        codec.aac_packet_type = 4;
        HELPER_ASSERT_SUCCESS(h.mux_aac2flv((char*)frame.data(), frame.length(), &codec, 0, &flv, &nb_flv));
        EXPECT_EQ(7, nb_flv);
        EXPECT_EQ(0xa6, (uint8_t)flv[0]);
        EXPECT_EQ(0x04, (uint8_t)flv[1]);
        EXPECT_STREQ("Hello", HELPER_ARR2STR(flv+2,5).c_str());
        srs_freepa(flv);
    }
    if (true) {
        SrsRawAacStream h;
        string frame("Hello"); SrsRawAacStreamCodec codec;
        char* flv = NULL; int nb_flv = 0;
        codec.sound_format = SrsAudioCodecIdAAC;
        codec.sound_rate = 1; codec.sound_size = 0; codec.sound_type = 1;
        codec.aac_packet_type = 4;
        HELPER_ASSERT_SUCCESS(h.mux_aac2flv((char*)frame.data(), frame.length(), &codec, 0, &flv, &nb_flv));
        EXPECT_EQ(7, nb_flv);
        EXPECT_EQ(0xa5, (uint8_t)flv[0]);
        EXPECT_EQ(0x04, (uint8_t)flv[1]);
        EXPECT_STREQ("Hello", HELPER_ARR2STR(flv+2,5).c_str());
        srs_freepa(flv);
    }
    if (true) {
        SrsRawAacStream h;
        string frame("Hello"); SrsRawAacStreamCodec codec;
        char* flv = NULL; int nb_flv = 0;
        codec.sound_format = SrsAudioCodecIdAAC;
        codec.sound_rate = 1; codec.sound_size = 1; codec.sound_type = 1;
        codec.aac_packet_type = 4;
        HELPER_ASSERT_SUCCESS(h.mux_aac2flv((char*)frame.data(), frame.length(), &codec, 0, &flv, &nb_flv));
        EXPECT_EQ(7, nb_flv);
        EXPECT_EQ(0xa7, (uint8_t)flv[0]);
        EXPECT_EQ(0x04, (uint8_t)flv[1]);
        EXPECT_STREQ("Hello", HELPER_ARR2STR(flv+2,5).c_str());
        srs_freepa(flv);
    }
    if (true) {
        SrsRawAacStream h;
        string frame("Hello"); SrsRawAacStreamCodec codec;
        char* flv = NULL; int nb_flv = 0;
        codec.sound_format = SrsAudioCodecIdAAC;
        codec.sound_rate = 3; codec.sound_size = 1; codec.sound_type = 1;
        codec.aac_packet_type = 4;
        HELPER_ASSERT_SUCCESS(h.mux_aac2flv((char*)frame.data(), frame.length(), &codec, 0, &flv, &nb_flv));
        EXPECT_EQ(7, nb_flv);
        EXPECT_EQ(0xaf, (uint8_t)flv[0]);
        EXPECT_EQ(0x04, (uint8_t)flv[1]);
        EXPECT_STREQ("Hello", HELPER_ARR2STR(flv+2,5).c_str());
        srs_freepa(flv);
    }
}

#ifdef SRS_H265

VOID TEST(SrsAVCTest, HevcMultiPPS)
{
    srs_error_t err;

    vector<uint8_t> vps = {
        0x40, 0x01, 0x0c, 0x06, 0x3f, 0x3f, 0x22, 0x20, 0x00, 0x00, 0x03, 0x00, 0x3f,
        0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x3f, 0x00, 0x00, 0x18, 0x3f, 0x24,
    };

    vector<uint8_t> sps = {
        0x42, 0x01, 0x06, 0x22, 0x20, 0x00, 0x00, 0x03, 0x00, 0x3f, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00,
        0x3f, 0x00, 0x00, 0x3f, 0x01, 0x3f, 0x20, 0x02, 0x1c, 0x4d, 0x3f, 0x3f, 0x3f, 0x42, 0x3f, 0x53, 0x3f,
        0x3f, 0x01, 0x01, 0x01, 0x04, 0x00, 0x00, 0x0f, 0x3f, 0x00, 0x01, 0x3f, 0x3f, 0x3f, 0x68, 0x3f, 0x3f,
        0x00, 0x1f, 0x3a, 0x00, 0x0f, 0x3f, 0x04, 0x00, 0x3e, 0x74, 0x00, 0x1f, 0x3a, 0x08, 0x00, 0x7c, 0x3f,
        0x00, 0x3e, 0x74, 0x10, 0x00, 0x3f, 0x3f, 0x00, 0x7c, 0x3f, 0x5c, 0x20, 0x10, 0x40
    };

    vector<uint8_t> pps_1 = {
        0x44, 0x01, 0x74, 0x18, 0xc2, 0xb8, 0x33, 0x3f, 0x7d, 0x75, 0x3f, 0x42, 0x28, 0x3f, 0x6b, 0x3f, 0x3f,
        0x11, 0x47, 0x7d, 0x72, 0x79, 0x3e, 0x4f, 0x3f, 0x3f, 0x51, 0x3f, 0x3f, 0x20, 0x3f, 0x3f, 0x3f, 0x10,
        0x63, 0x3f, 0x0a, 0x0e, 0x3f, 0x04, 0x42, 0x3f, 0x3f, 0x3f, 0x34, 0x22, 0x3f, 0x3f, 0x3f, 0x3f, 0x7d,
        0x7e, 0x4f, 0x3f, 0x3f, 0x7c, 0x57, 0x3f, 0x3f, 0x3f, 0x10, 0x41, 0x21, 0x14, 0x41, 0x3f, 0x3f, 0x3c,
        0x3f, 0x5f, 0x3f, 0x3f, 0x28, 0x3f, 0x73, 0x10, 0x44, 0x49, 0x47, 0x08, 0x31, 0xc4, 0x85, 0x06, 0x46,
        0x3f, 0x21, 0x02, 0x41, 0x54, 0x1a, 0x11, 0x45, 0x13, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x11,
        0x3f, 0x5e, 0x3b, 0x3f, 0x30, 0x41, 0x04, 0x3f, 0x51, 0x06, 0x3f, 0x3f, 0x3f, 0x7d, 0x7e, 0x4f, 0x3f,
        0x3f, 0x1d, 0x3f, 0x41, 0x11, 0x25, 0x1c, 0x20, 0x3f, 0x12, 0x14, 0x19, 0x1a, 0x08, 0x3f, 0x09, 0x05,
        0x50, 0x69, 0x14, 0x4f, 0x3f, 0x4f, 0x27, 0x3f, 0x3f, 0x3f, 0x28, 0x3f, 0x73, 0x10, 0xd3, 0x94, 0x71,
        0x3f, 0x73, 0x3f, 0x05, 0x45, 0x3f, 0x01, 0x02, 0x41, 0x54, 0x18, 0x24
    };

    vector<uint8_t> pps_2 = {
        0x44, 0x01, 0x25, 0x06, 0x30, 0x3f, 0x0c, 0x3f, 0x4a, 0x3f, 0x3f, 0x49, 0x08, 0x3f, 0x15, 0x6b, 0x3f,
        0x3f, 0x11, 0x4c, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x46, 0x3b, 0x3f, 0x3f, 0x22, 0x4a,
        0x30, 0x51, 0x3f, 0x2c, 0x2c, 0x32, 0x34, 0x11, 0x08, 0x12, 0x0a, 0x3f, 0xd0, 0x8a, 0x29, 0x56, 0x3f,
        0x3c, 0x3f, 0x5f, 0x3f, 0x3f, 0x3f, 0x1f, 0x15, 0x3f, 0x3f, 0x63, 0x04, 0x10, 0x4c, 0x45, 0x35, 0x49,
        0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x77, 0x31, 0x04, 0x44, 0x3f, 0x60, 0x3f, 0x1c, 0x58,
        0x58, 0x64, 0x68, 0x22, 0x10, 0x24, 0x15, 0x41, 0x3f, 0x14, 0x42, 0x55, 0x3f, 0x4f, 0x27, 0x3f, 0x3f,
        0x3f, 0x3f, 0x47, 0x3f, 0x78, 0x3f, 0x18, 0x3f, 0x04, 0x13, 0x11, 0x4d, 0x52, 0x64, 0x3f, 0x3f, 0x3f,
        0x7e, 0x4f, 0x3f, 0x3f, 0x1d, 0x3f, 0x41, 0x11, 0x25, 0x18, 0x28, 0x3f, 0x16, 0x16, 0x19, 0x1a, 0x08,
        0x3f, 0x09, 0x05, 0x50, 0x69, 0x10, 0x3f, 0x6b, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x11, 0x3f, 0x5e,
        0x3b, 0x3f, 0x30, 0x41, 0x04, 0x3f
    };

    vector<uint8_t> start_code = {0x00, 0x00, 0x00, 0x01};

    string hevc_sh;
    hevc_sh.append((const char*)start_code.data(), start_code.size());
    hevc_sh.append((const char*)vps.data(), vps.size());
    hevc_sh.append((const char*)start_code.data(), start_code.size());
    hevc_sh.append((const char*)sps.data(), sps.size());
    hevc_sh.append((const char*)start_code.data(), start_code.size());
    hevc_sh.append((const char*)pps_1.data(), pps_1.size());
    hevc_sh.append((const char*)start_code.data(), start_code.size());
    hevc_sh.append((const char*)pps_2.data(), pps_2.size());

    SrsBuffer stream((char*)hevc_sh.data(), hevc_sh.size());

    SrsRawHEVCStream hs;
    char* frame = NULL;
    int frame_size = 0;

    HELPER_ASSERT_SUCCESS(hs.annexb_demux(&stream, &frame, &frame_size));
    EXPECT_TRUE(hs.is_vps(frame, frame_size));
    EXPECT_EQ(frame_size, vps.size());
    EXPECT_TRUE(srs_bytes_equals(frame, vps.data(), frame_size));

    HELPER_ASSERT_SUCCESS(hs.annexb_demux(&stream, &frame, &frame_size));
    EXPECT_TRUE(hs.is_sps(frame, frame_size));
    EXPECT_EQ(frame_size, sps.size());
    EXPECT_TRUE(srs_bytes_equals(frame, sps.data(), frame_size));

    HELPER_ASSERT_SUCCESS(hs.annexb_demux(&stream, &frame, &frame_size));
    EXPECT_TRUE(hs.is_pps(frame, frame_size));
    EXPECT_EQ(frame_size, pps_1.size());
    EXPECT_TRUE(srs_bytes_equals(frame, pps_1.data(), frame_size));

    HELPER_ASSERT_SUCCESS(hs.annexb_demux(&stream, &frame, &frame_size));
    EXPECT_TRUE(hs.is_pps(frame, frame_size));
    EXPECT_EQ(frame_size, pps_2.size());
    EXPECT_TRUE(srs_bytes_equals(frame, pps_2.data(), frame_size));

    EXPECT_TRUE(stream.empty());
}

#endif

