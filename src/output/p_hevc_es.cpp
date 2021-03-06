/*
   mkvmerge -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL v2
   see the file COPYING for details
   or visit https://www.gnu.org/licenses/old-licenses/gpl-2.0.html

   HEVC ES video output module

*/

#include "common/common_pch.h"

#include <matroska/KaxTracks.h>

#include "common/codec.h"
#include "common/hacks.h"
#include "common/hevc.h"
#include "common/mpeg.h"
#include "merge/connection_checks.h"
#include "merge/output_control.h"
#include "output/p_hevc_es.h"

using namespace libmatroska;

hevc_es_video_packetizer_c::hevc_es_video_packetizer_c(generic_reader_c *p_reader,
                                                       track_info_c &p_ti)
  : generic_packetizer_c(p_reader, p_ti)
  , m_default_duration_for_interlaced_content(-1)
  , m_first_frame(true)
  , m_set_display_dimensions(false)
  , m_debug_timestamps(   debugging_c::requested("hevc_es|hevc_es_timestamps"))
  , m_debug_aspect_ratio(debugging_c::requested("hevc_es|hevc_es_aspect_ratio"))
{
  m_relaxed_timestamp_checking = true;

  set_track_type(track_video);

  set_codec_id(MKV_V_MPEGH_HEVC);

  m_parser.set_keep_ar_info(false);

  // If no external timestamp file has been specified then mkvmerge
  // might have created a factory due to the --default-duration
  // command line argument. This factory must be disabled for the HEVC
  // packetizer because it takes care of handling the default
  // duration/FPS itself.
  if (m_ti.m_ext_timestamps.empty())
    m_timestamp_factory.reset();

  int64_t factory_default_duration;
  if (m_timestamp_factory && (-1 != (factory_default_duration = m_timestamp_factory->get_default_duration(-1)))) {
    m_parser.force_default_duration(factory_default_duration);
    set_track_default_duration(factory_default_duration);
    m_default_duration_forced = true;
    mxdebug_if(m_debug_timestamps, fmt::format("Forcing default duration due to timestamp factory to {0}\n", m_htrack_default_duration));

  } else if (m_default_duration_forced && (-1 != m_htrack_default_duration)) {
    m_default_duration_for_interlaced_content = m_htrack_default_duration / 2;
    m_parser.force_default_duration(m_default_duration_for_interlaced_content);
    mxdebug_if(m_debug_timestamps, fmt::format("Forcing default duration due to --default-duration to {0}\n", m_htrack_default_duration));
  }
}

void
hevc_es_video_packetizer_c::set_headers() {
  generic_packetizer_c::set_headers();

  m_track_entry->EnableLacing(false);
}

void
hevc_es_video_packetizer_c::set_container_default_field_duration(int64_t default_duration) {
  m_parser.set_container_default_duration(default_duration);
}

void
hevc_es_video_packetizer_c::add_extra_data(memory_cptr data) {
  m_parser.add_bytes(data->get_buffer(), data->get_size());
}

int
hevc_es_video_packetizer_c::process(packet_cptr packet) {
  try {
    if (packet->has_timestamp())
      m_parser.add_timestamp(packet->timestamp);
    m_parser.add_bytes(packet->data->get_buffer(), packet->data->get_size());
    flush_frames();

  } catch (mtx::exception &error) {
    mxerror_tid(m_ti.m_fname, m_ti.m_id,
                fmt::format(Y("mkvmerge encountered broken or unparsable data in this HEVC video track. "
                              "Either your file is damaged (which mkvmerge cannot cope with yet) or this is a bug in mkvmerge itself. "
                              "The error message was:\n{0}\n"), error.error()));
  }

  return FILE_STATUS_MOREDATA;
}

void
hevc_es_video_packetizer_c::handle_delayed_headers() {
  if (0 < m_parser.get_num_skipped_frames())
    mxwarn_tid(m_ti.m_fname, m_ti.m_id, fmt::format(Y("This HEVC track does not start with a key frame. The first {0} frames have been skipped.\n"), m_parser.get_num_skipped_frames()));

  set_codec_private(m_parser.get_hevcc());

  handle_aspect_ratio();
  handle_actual_default_duration();

  rerender_track_headers();
}

void
hevc_es_video_packetizer_c::handle_aspect_ratio() {
  mxdebug_if(m_debug_aspect_ratio, fmt::format("already set? {0} has par been found? {1}\n", display_dimensions_or_aspect_ratio_set(), m_parser.has_par_been_found()));

  if (display_dimensions_or_aspect_ratio_set() || !m_parser.has_par_been_found())
    return;

  auto dimensions = m_parser.get_display_dimensions(m_hvideo_pixel_width, m_hvideo_pixel_height);
  set_video_display_dimensions(dimensions.first, dimensions.second, generic_packetizer_c::ddu_pixels, OPTION_SOURCE_BITSTREAM);

  mxinfo_tid(m_ti.m_fname, m_ti.m_id,
             fmt::format(Y("Extracted the aspect ratio information from the HEVC video data "
                           "and set the display dimensions to {0}/{1}.\n"), m_ti.m_display_width, m_ti.m_display_height));

  mxdebug_if(m_debug_aspect_ratio,
             fmt::format("PAR {0} pixel_width/hgith {1}/{2} display_width/height {3}/{4}\n",
                         boost::rational_cast<double>(m_parser.get_par()), m_hvideo_pixel_width, m_hvideo_pixel_height, m_ti.m_display_width, m_ti.m_display_height));
}

void
hevc_es_video_packetizer_c::handle_actual_default_duration() {
  int64_t actual_default_duration = m_parser.get_most_often_used_duration();
  mxdebug_if(m_debug_timestamps, fmt::format("Most often used duration: {0} forced? {1} current default duration: {2}\n", actual_default_duration, m_default_duration_forced, m_htrack_default_duration));

  if (   !m_default_duration_forced
      && (0 < actual_default_duration)
      && (m_htrack_default_duration != actual_default_duration))
    set_track_default_duration(actual_default_duration);

  else if (   m_default_duration_forced
           && (0 < m_default_duration_for_interlaced_content)
           && (std::abs(actual_default_duration - m_default_duration_for_interlaced_content) <= 20000)) {
    m_default_duration_forced = false;
    set_track_default_duration(m_default_duration_for_interlaced_content);
  }
}

void
hevc_es_video_packetizer_c::flush_impl() {
  m_parser.flush();
  flush_frames();
}

void
hevc_es_video_packetizer_c::flush_frames() {
  while (m_parser.frame_available()) {
    if (m_first_frame) {
      handle_delayed_headers();
      m_first_frame = false;
    }

    auto frame = m_parser.get_frame();
    add_packet(new packet_t(frame.m_data, frame.m_start,
                            frame.m_end > frame.m_start ? frame.m_end - frame.m_start : m_htrack_default_duration,
                            frame.m_keyframe            ? -1                          : frame.m_start + frame.m_ref1));
  }
}

unsigned int
hevc_es_video_packetizer_c::get_nalu_size_length()
  const {
  return m_parser.get_nalu_size_length();
}

void
hevc_es_video_packetizer_c::connect(generic_packetizer_c *src,
                                         int64_t p_append_timestamp_offset) {
  generic_packetizer_c::connect(src, p_append_timestamp_offset);

  if (2 != m_connected_to)
    return;

  auto *real_src = dynamic_cast<hevc_es_video_packetizer_c *>(src);
  assert(real_src);

  m_htrack_default_duration = real_src->m_htrack_default_duration;
  m_default_duration_forced = real_src->m_default_duration_forced;

  if (m_default_duration_forced && (-1 != m_htrack_default_duration)) {
    m_default_duration_for_interlaced_content = m_htrack_default_duration / 2;
    m_parser.force_default_duration(m_default_duration_for_interlaced_content);
  }
}

connection_result_e
hevc_es_video_packetizer_c::can_connect_to(generic_packetizer_c *src,
                                                std::string &error_message) {
  auto *vsrc = dynamic_cast<hevc_es_video_packetizer_c *>(src);
  if (!vsrc)
    return CAN_CONNECT_NO_FORMAT;

  connect_check_codec_private(src);

  return CAN_CONNECT_YES;
}
