/*
   mkvmerge -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL v2
   see the file COPYING for details
   or visit https://www.gnu.org/licenses/old-licenses/gpl-2.0.html

   IVF demultiplexer module

   Written by Moritz Bunkus <moritz@bunkus.org>.
*/

#include "common/common_pch.h"

#include <algorithm>

#include "common/endian.h"
#include "common/ivf.h"
#include "common/id_info.h"
#include "input/r_ivf.h"
#include "output/p_av1.h"
#include "output/p_vpx.h"
#include "merge/input_x.h"
#include "merge/file_status.h"

bool
ivf_reader_c::probe_file() {
  if (m_in->read(&m_header, sizeof(ivf::file_header_t)) < sizeof(ivf::file_header_t))
    return false;

  if (memcmp(m_header.file_magic, "DKIF", 4))
    return false;

  m_codec = m_header.get_codec();

  return m_codec.is(codec_c::type_e::V_AV1) || m_codec.is(codec_c::type_e::V_VP8) || m_codec.is(codec_c::type_e::V_VP9);
}

void
ivf_reader_c::read_headers() {
  m_in->setFilePointer(sizeof(ivf::file_header_t));

  m_width          = get_uint16_le(&m_header.width);
  m_height         = get_uint16_le(&m_header.height);
  m_frame_rate_num = get_uint32_le(&m_header.frame_rate_num);
  m_frame_rate_den = get_uint32_le(&m_header.frame_rate_den);
  m_ok             = m_width && m_height && m_frame_rate_num && m_frame_rate_den;

  show_demuxer_info();
}

void
ivf_reader_c::add_available_track_ids() {
  if (m_ok)
    add_available_track_id(0);
}

void
ivf_reader_c::create_packetizer(int64_t) {
  if (!demuxing_requested('v', 0) || !m_reader_packetizers.empty() || !m_ok)
    return;

  if (m_codec.is(codec_c::type_e::V_AV1))
    create_av1_packetizer();

  else if (m_codec.is(codec_c::type_e::V_VP8) || m_codec.is(codec_c::type_e::V_VP9))
    create_vpx_packetizer();

  auto &packetizer = ptzr(0);

  packetizer.set_video_pixel_width(m_width);
  packetizer.set_video_pixel_height(m_height);

  auto default_duration = 1000000000ll * m_frame_rate_den / m_frame_rate_num;
  if (default_duration >= 1000000)
    packetizer.set_track_default_duration(default_duration);

  show_packetizer_info(0, packetizer);
}

void
ivf_reader_c::create_av1_packetizer() {
  auto av1_ptzr = new av1_video_packetizer_c(this, m_ti);
  av1_ptzr->set_is_unframed();

  add_packetizer(av1_ptzr);
}

void
ivf_reader_c::create_vpx_packetizer() {
  add_packetizer(new vpx_video_packetizer_c(this, m_ti, m_codec.get_type()));
}

file_status_e
ivf_reader_c::read(generic_packetizer_c *,
                   bool) {
  size_t remaining_bytes = m_size - m_in->getFilePointer();

  ivf::frame_header_t header;
  if ((sizeof(ivf::frame_header_t) > remaining_bytes) || (m_in->read(&header, sizeof(ivf::frame_header_t)) != sizeof(ivf::frame_header_t)))
    return flush_packetizers();

  remaining_bytes     -= sizeof(ivf::frame_header_t);
  uint32_t frame_size  = get_uint32_le(&header.frame_size);

  if (remaining_bytes < frame_size) {
    m_in->setFilePointer(0, seek_end);
    return flush_packetizers();
  }

  memory_cptr buffer = memory_c::alloc(frame_size);
  if (m_in->read(buffer->get_buffer(), frame_size) < frame_size) {
    m_in->setFilePointer(0, seek_end);
    return flush_packetizers();
  }

  int64_t timestamp = get_uint64_le(&header.timestamp) * 1000000000ull * m_frame_rate_den / m_frame_rate_num;

  mxdebug_if(m_debug, fmt::format("key {4} header.ts {0} num {1} den {2} res {3}\n", get_uint64_le(&header.timestamp), m_frame_rate_num, m_frame_rate_den, timestamp, ivf::is_keyframe(buffer, m_codec.get_type())));

  ptzr(0).process(new packet_t(buffer, timestamp));

  return FILE_STATUS_MOREDATA;
}

void
ivf_reader_c::identify() {
  id_result_container();

  if (!m_ok)
    return;

  auto info = mtx::id::info_c{};
  info.add(mtx::id::pixel_dimensions, fmt::format("{0}x{1}", m_width, m_height));
  id_result_track(0, ID_RESULT_TRACK_VIDEO, m_codec.get_name(), info.get());
}
