/*
   mkvmerge -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL v2
   see the file COPYING for details
   or visit https://www.gnu.org/licenses/old-licenses/gpl-2.0.html

   ISO 639 language definitions, lookup functions

   Written by Moritz Bunkus <moritz@bunkus.org>.
*/

#pragma once

#include "common/common_pch.h"

#include "common/iso639_types.h"

namespace mtx::iso639 {

void init();
std::optional<language_t> look_up(std::string const &s, bool allow_short_english_names = false);
void list_languages();

} // namespace mtx::iso639
