/*
   mkvmerge -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL
   see the file COPYING for details
   or visit http://www.gnu.org/copyleft/gpl.html

   $Id$

   VC1 ES demultiplexer module

   Written by Moritz Bunkus <moritz@bunkus.org>.
*/

#include "os.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "byte_buffer.h"
#include "common.h"
#include "error.h"
#include "output_control.h"
#include "r_vc1.h"
#include "p_vc1.h"

using namespace std;

#define PROBESIZE 4
#define READ_SIZE 1024 * 1024

int
vc1_es_reader_c::probe_file(mm_io_c *io,
                            int64_t size) {
  try {
    if (PROBESIZE > size)
      return 0;

    io->setFilePointer(0, seek_beginning);

    memory_cptr buf = memory_c::alloc(READ_SIZE);
    int num_read    = io->read(buf->get(), READ_SIZE);

    if (4 > num_read)
      return 0;

    uint32_t marker = get_uint32_be(buf->get());
    if ((VC1_MARKER_SEQHDR != marker) && (VC1_MARKER_ENTRYPOINT != marker) && (VC1_MARKER_FRAME != marker))
      return 0;

    vc1::es_parser_c parser;
    parser.add_bytes(buf->get(), num_read);

    return parser.is_sequence_header_available();

  } catch (...) {
    mxinfo("have an xcptn\n");
  }

  return 0;
}

vc1_es_reader_c::vc1_es_reader_c(track_info_c &n_ti)
  throw (error_c)
  : generic_reader_c(n_ti)
  , m_bytes_processed(0)
  , m_buffer(memory_c::alloc(READ_SIZE))
{

  try {
    m_io   = counted_ptr<mm_io_c>(new mm_file_io_c(ti.fname));
    m_size = m_io->get_size();

    vc1::es_parser_c parser;

    int num_read = m_io->read(m_buffer->get(), READ_SIZE);
    parser.add_bytes(m_buffer->get(), num_read);

    if (!parser.is_sequence_header_available())
      throw false;

    parser.get_sequence_header(m_seqhdr);

    m_io->setFilePointer(0, seek_beginning);

  } catch (...) {
    throw error_c("vc1_es_reader: Could not open the source file.");
  }

  if (verbose)
    mxinfo(FMT_FN "Using the VC1 ES demultiplexer.\n", ti.fname.c_str());
}

void
vc1_es_reader_c::create_packetizer(int64_t) {
  if (NPTZR() != 0)
    return;

  add_packetizer(new vc1_video_packetizer_c(this, ti));

  mxinfo(FMT_TID "Using the VC1 video output module.\n", ti.fname.c_str(), (int64_t)0);
}

file_status_e
vc1_es_reader_c::read(generic_packetizer_c *,
                      bool) {
  if (m_bytes_processed >= m_size)
    return FILE_STATUS_DONE;

  int num_read = m_io->read(m_buffer->get(), READ_SIZE);
  if (0 >= num_read)
    return FILE_STATUS_DONE;

  PTZR0->process(new packet_t(new memory_c(m_buffer->get(), num_read)));

  m_bytes_processed += num_read;

  if ((READ_SIZE != num_read) || (m_bytes_processed >= m_size))
    PTZR0->flush();

  return READ_SIZE == num_read ? FILE_STATUS_MOREDATA : FILE_STATUS_DONE;
}

int
vc1_es_reader_c::get_progress() {
  return 100 * m_bytes_processed / m_size;
}

void
vc1_es_reader_c::identify() {
  id_result_container("VC1 elementary stream");
  id_result_track(0, ID_RESULT_TRACK_VIDEO, "VC1");
}

