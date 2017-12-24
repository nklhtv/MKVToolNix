/*
   mkvmerge -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL v2
   see the file COPYING for details
   or visit http://www.gnu.org/copyleft/gpl.html

   retrieves and displays information about a Matroska file (Qt binding)

   Written by Moritz Bunkus <moritz@bunkus.org>.
*/

#pragma once

#include "common/common_pch.h"

#include <QObject>

#include "common/kax_info.h"

namespace mtx {

class qt_kax_info_c: public QObject, public kax_info_c {
  Q_OBJECT;
  std::string m_source_file_name;

public:
  qt_kax_info_c() = default;
  virtual ~qt_kax_info_c();

  virtual void set_source_file_name(std::string const &file_name);
  virtual result_e process_file(std::string const &file_name) override;

  virtual void ui_show_error(std::string const &error) override;
  virtual void ui_show_element(int level, std::string const &text, int64_t position, int64_t size) override;
  virtual void ui_show_progress(int percentage, std::string const &text) override;

public slots:
  virtual void start_processing();

signals:
  void element_found(int level, QString const &text, int64_t position, int64_t size);
  void error_found(const QString &message);
  void progress_changed(int percentage, const QString &text);
  void started();
  void finished(mtx::kax_info_c::result_e result);
};

}

Q_DECLARE_METATYPE(mtx::kax_info_c::result_e);