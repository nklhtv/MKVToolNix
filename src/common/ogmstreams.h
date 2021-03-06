/*
   mkvmerge -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL v2
   see the file COPYING for details
   or visit https://www.gnu.org/licenses/old-licenses/gpl-2.0.html

   definitions for the OGM file format

   Written by Moritz Bunkus <moritz@bunkus.org>.
*/

#pragma once

namespace mtx::ogm {

/*
   Taken from http://tobias.everwicked.com/packfmt.htm

 First packet (header)
 ---------------------

 pos    | content                 | description
 -------+-------------------------+----------------------------------
 0x0000 | 0x01                    | indicates 'header packet'
 -------+-------------------------+----------------------------------
 0x0001 | stream_header           | the size is indicated in the
        |                         | size member

 Second packet (comment)
 -----------------------

 pos    | content                 | description
 -------+-------------------------+----------------------------------
 0x0000 | 0x03                    | indicates 'comment packet'
 -------+-------------------------+----------------------------------
 0x0001 | data                    | see vorbis doc on www.xiph.org

 Data packets
 ------------

 pos      | content                 | description
 ---------+-------------------------+----------------------------------
 0x0000   | Bit0  0                 | indicates data packet
          | Bit1  Bit 2 of lenbytes |
          | Bit2  unused            |
          | Bit3  keyframe          |
          | Bit4  unused            |
          | Bit5  unused            |
          | Bit6  Bit 0 of lenbytes |
          | Bit7  Bit 1 of lenbytes |
 ---------+-------------------------+----------------------------------
 0x0001   | LowByte                 | Length of this packet in samples
          | ...                     | (frames for video, samples for
          | HighByte                | audio, 1ms units for text)
 ---------+-------------------------+----------------------------------
 0x0001+  | data                    | packet contents
 lenbytes |                         |

*/

//// OggDS headers
// Header for the new header format
struct stream_header_video {
  ogg_int32_t  width;
  ogg_int32_t  height;
};

struct stream_header_audio {
  ogg_int16_t  channels;
  ogg_int16_t  blockalign;
  ogg_int32_t  avgbytespersec;
};

#if defined(COMP_MSC)
#pragma pack(push,1)
#endif
struct PACKED_STRUCTURE stream_header {
  char        streamtype[8];
  char        subtype[4];

  ogg_int32_t size;             // size of the structure

  ogg_int64_t time_unit;        // in reference time
  ogg_int64_t samples_per_unit;
  ogg_int32_t default_len;      // in media time

  ogg_int32_t buffersize;
  ogg_int16_t bits_per_sample;

  ogg_int16_t padding;

  union {
    // Video specific
    stream_header_video  video;
    // Audio specific
    stream_header_audio  audio;
  } sh;

};

struct PACKED_STRUCTURE vp8_header_t {
  uint8_t header_id;            // VP8 Ogg header mapping ID 0x4f
  uint32_t id;                  // VP8 Ogg mapping ID 0x56503830 ("VP80")
  uint8_t header_type;          // VP8 stream info header type 0x01
  uint8_t version_major;        // Mapping major version
  uint8_t version_minor;        // Mapping minor version
  uint16_t pixel_width;         // Stored frame width
  uint16_t pixel_height;        // Stored frame height
  uint8_t par_num[3];           // Pixel aspect ratio numerator
  uint8_t par_den[3];           // Pixel aspect ratio denominator
  uint32_t frame_rate_num;      // Frame rate numerator
  uint32_t frame_rate_den;      // Frame rate denominator
};
#if defined(COMP_MSC)
#pragma pack(pop)
#endif

/// Some defines from OggDS
constexpr auto PACKET_TYPE_HEADER  = 0x01;
constexpr auto PACKET_TYPE_COMMENT = 0x03;
constexpr auto PACKET_TYPE_BITS    = 0x07;
constexpr auto PACKET_LEN_BITS01   = 0xc0;
constexpr auto PACKET_LEN_BITS2    = 0x02;
constexpr auto PACKET_IS_SYNCPOINT = 0x08;

} // namespace mtx::ogm
