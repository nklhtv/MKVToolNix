/*
   mkvmerge -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL v2
   see the file COPYING for details
   or visit http://www.gnu.org/copyleft/gpl.html

   IO callback class definitions

   Written by Moritz Bunkus <moritz@bunkus.org>.
   Modifications by Nils Maier <maierman@web.de>
*/

#pragma once

#include "common/common_pch.h"

#include "common/mm_io.h"

class mm_read_buffer_io_c: public mm_proxy_io_c {
protected:
  memory_cptr m_af_buffer;
  unsigned char *m_buffer;
  size_t m_cursor;
  bool m_eof;
  size_t m_fill;
  int64_t m_offset;
  const size_t m_size;
  bool m_buffering;
  debugging_option_c m_debug_seek, m_debug_read;

public:
  mm_read_buffer_io_c(mm_io_c *in, size_t buffer_size = 1 << 17, bool delete_in = true);
  virtual ~mm_read_buffer_io_c();

  virtual uint64 getFilePointer();
  virtual void setFilePointer(int64 offset, seek_mode mode = seek_beginning);
  virtual int64_t get_size();
  inline virtual bool eof() { return m_eof; }
  virtual void clear_eof() { m_eof = false; }
  virtual void enable_buffering(bool enable);

protected:
  virtual uint32 _read(void *buffer, size_t size);
  virtual size_t _write(const void *buffer, size_t size);
};
