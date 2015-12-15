/*
   mkvmerge -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL v2
   see the file COPYING for details
   or visit http://www.gnu.org/copyleft/gpl.html

   tag helper functions

   Written by Moritz Bunkus <moritz@bunkus.org>.
*/

#include "common/common_pch.h"

#include "common/chapters/chapters.h"
#include "common/ebml.h"
#include "common/strings/editing.h"
#include "common/strings/utf8.h"
#include "common/tags/tags.h"
#include "common/tags/target_type.h"

#include <matroska/KaxTags.h>
#include <matroska/KaxTag.h>

using namespace libebml;
using namespace libmatroska;

namespace mtx { namespace tags {

void
fix_mandatory_elements(EbmlElement *e) {
  if (dynamic_cast<KaxTag *>(e)) {
    KaxTag &t = *static_cast<KaxTag *>(e);
    GetChild<KaxTagTargets>(t);
    GetChild<KaxTagSimple>(t);

  } else if (dynamic_cast<KaxTagSimple *>(e))
    FixMandatoryElement<KaxTagName, KaxTagLangue, KaxTagDefault>(static_cast<KaxTagSimple *>(e));

  else if (dynamic_cast<KaxTagTargets *>(e)) {
    KaxTagTargets &t = *static_cast<KaxTagTargets *>(e);
    GetChild<KaxTagTargetTypeValue>(t);
    FixMandatoryElement<KaxTagTargetTypeValue>(t);

  }

  if (dynamic_cast<EbmlMaster *>(e))
    for (auto child : *static_cast<EbmlMaster *>(e))
      fix_mandatory_elements(child);
}

KaxTags *
select_for_chapters(KaxTags &tags,
                    KaxChapters &chapters) {
  KaxTags *new_tags = nullptr;

  for (auto tag_child : tags) {
    auto tag = dynamic_cast<KaxTag *>(tag_child);
    if (!tag)
      continue;

    bool copy              = true;
    KaxTagTargets *targets = FindChild<KaxTagTargets>(tag);

    if (targets) {
      for (auto child : *targets) {
        auto t_euid = dynamic_cast<KaxTagEditionUID *>(child);
        if (t_euid && !find_edition_with_uid(chapters, t_euid->GetValue())) {
          copy = false;
          break;
        }

        auto t_cuid = dynamic_cast<KaxTagChapterUID *>(child);
        if (t_cuid && !find_chapter_with_uid(chapters, t_cuid->GetValue())) {
          copy = false;
          break;
        }
      }
    }

    if (!copy)
      continue;

    if (!new_tags)
      new_tags = new KaxTags;
    new_tags->PushElement(*(tag->Clone()));
  }

  return new_tags;
}

KaxTagSimple &
find_simple(const std::string &name,
            EbmlMaster &m) {
  return find_simple(to_utfstring(name), m);
}

KaxTagSimple &
find_simple(const UTFstring &name,
            EbmlMaster &m) {
  if (Is<KaxTagSimple>(&m)) {
    KaxTagName *tname = FindChild<KaxTagName>(&m);
    if (tname && (name == UTFstring(*tname)))
      return *static_cast<KaxTagSimple *>(&m);
  }

  size_t i;
  for (i = 0; i < m.ListSize(); i++)
    if (Is<KaxTag, KaxTagSimple>(m[i]))
      try {
        return find_simple(name, *static_cast<EbmlMaster *>(m[i]));
      } catch (...) {
      }

  throw false;
}

std::string
get_simple_value(const std::string &name,
                 EbmlMaster &m) {
  try {
    auto tvalue = FindChild<KaxTagString>(&find_simple(name, m));
    if (tvalue)
      return tvalue->GetValueUTF8();
  } catch (...) {
  }

  return "";
}

std::string
get_simple_name(const KaxTagSimple &tag) {
  KaxTagName *tname = FindChild<KaxTagName>(&tag);
  return tname ? tname->GetValueUTF8() : std::string{""};
}

std::string
get_simple_value(const KaxTagSimple &tag) {
  KaxTagString *tstring = FindChild<KaxTagString>(&tag);
  return tstring ? tstring->GetValueUTF8() : std::string{""};
}

void
set_simple_name(KaxTagSimple &tag,
                const std::string &name) {
  GetChild<KaxTagName>(tag).SetValueUTF8(name);
}

void
set_simple_value(KaxTagSimple &tag,
                 const std::string &value) {
  GetChild<KaxTagString>(tag).SetValueUTF8(value);
}

void
set_simple(KaxTagSimple &tag,
           const std::string &name,
           const std::string &value) {
  set_simple_name(tag, name);
  set_simple_value(tag, value);
}

int64_t
get_tuid(const KaxTag &tag) {
  auto targets = FindChild<KaxTagTargets>(&tag);
  if (!targets)
    return -1;

  auto tuid = FindChild<KaxTagTrackUID>(targets);
  if (!tuid)
    return -1;

  return tuid->GetValue();
}

int64_t
get_cuid(const KaxTag &tag) {
  auto targets = FindChild<KaxTagTargets>(&tag);
  if (!targets)
    return -1;

  auto cuid = FindChild<KaxTagChapterUID>(targets);
  if (!cuid)
    return -1;

  return cuid->GetValue();
}

/** \brief Convert older tags to current specs
*/
void
convert_old(KaxTags &tags) {
  bool has_level_type = false;
  try {
    find_simple("LEVEL_TYPE", tags);
    has_level_type = true;
  } catch (...) {
  }

  size_t tags_idx;
  for (tags_idx = 0; tags_idx < tags.ListSize(); tags_idx++) {
    if (!Is<KaxTag>(tags[tags_idx]))
      continue;

    KaxTag &tag = *static_cast<KaxTag *>(tags[tags_idx]);

    auto target_type_value = Track;
    size_t tag_idx         = 0;
    while (tag_idx < tag.ListSize()) {
      tag_idx++;
      if (!Is<KaxTagSimple>(tag[tag_idx - 1]))
        continue;

      KaxTagSimple &simple = *static_cast<KaxTagSimple *>(tag[tag_idx - 1]);

      std::string name  = get_simple_name(simple);
      std::string value = get_simple_value(simple);

      if (name == "CATALOG")
        set_simple_name(simple, "CATALOG_NUMBER");

      else if (name == "DATE")
        set_simple_name(simple, "DATE_RELEASED");

      else if (name == "LEVEL_TYPE") {
        if (value == "MEDIA")
          target_type_value = Album;
        tag_idx--;
        delete tag[tag_idx];
        tag.Remove(tag_idx);
      }
    }

    if (!has_level_type)
      continue;

    auto targets = FindChild<KaxTagTargets>(&tag);
    if (targets)
      GetChild<KaxTagTargetTypeValue>(*targets).SetValue(target_type_value);
  }
}

int
count_simple(EbmlMaster &master) {
  int count = 0;

  for (auto child : master)
    if (Is<KaxTagSimple>(child))
      ++count;

    else if (dynamic_cast<EbmlMaster *>(child))
      count += count_simple(*static_cast<EbmlMaster *>(child));

  return count;
}

void
remove_track_uid_targets(EbmlMaster *tag) {
  for (auto el : *tag) {
    if (!Is<KaxTagTargets>(el))
      continue;

    KaxTagTargets *targets = static_cast<KaxTagTargets *>(el);
    size_t idx_target      = 0;

    while (targets->ListSize() > idx_target) {
      EbmlElement *uid_el = (*targets)[idx_target];
      if (Is<KaxTagTrackUID>(uid_el)) {
        targets->Remove(idx_target);
        delete uid_el;

      } else
        ++idx_target;
    }
  }
}

void
set_simple(KaxTag &tag,
           std::string const &name,
           std::string const &value,
           std::string const &language) {
  KaxTagSimple *k_simple_tag = nullptr;

  for (auto const &element : tag) {
    auto s_tag = dynamic_cast<KaxTagSimple *>(element);
    if (!s_tag || (to_utf8(FindChildValue<KaxTagName>(s_tag)) != name))
      continue;

    k_simple_tag = s_tag;
    break;
  }

  if (!k_simple_tag) {
    k_simple_tag = new KaxTagSimple;
    tag.PushElement(*k_simple_tag);
  }

  GetChild<KaxTagName>(k_simple_tag).SetValueUTF8(name);
  GetChild<KaxTagString>(k_simple_tag).SetValueUTF8(value);

  if (!language.empty())
    GetChild<KaxTagLangue>(k_simple_tag).SetValue(language);
}

void
set_target_type(KaxTag &tag,
                target_type_e target_type_value,
                std::string const &target_type) {
  auto &targets = GetChild<KaxTagTargets>(tag);

  GetChild<KaxTagTargetTypeValue>(targets).SetValue(target_type_value);
  GetChild<KaxTagTargetType>(targets).SetValue(target_type);
}

void
remove_elements_unsupported_by_webm(EbmlMaster &master) {
  static auto s_supported_elements = std::map<uint32, bool>{};

  if (s_supported_elements.empty()) {
#define add(ref) s_supported_elements[ EBML_ID_VALUE(EBML_ID(ref)) ] = true;
    add(KaxTags);
    add(KaxTag);
    add(KaxTagTargets);
    add(KaxTagTargetTypeValue);
    add(KaxTagTargetType);
    add(KaxTagTrackUID);
    add(KaxTagSimple);
    add(KaxTagName);
    add(KaxTagLangue);
    add(KaxTagDefault);
    add(KaxTagString);
    add(KaxTagBinary);
#undef add
  }

  auto is_simple = Is<KaxTagSimple>(master);
  auto idx       = 0u;

  while (idx < master.ListSize()) {
    auto e = master[idx];

    if (e && s_supported_elements[ EBML_ID_VALUE(EbmlId(*e)) ] && !(is_simple && Is<KaxTagSimple>(e))) {
      ++idx;

      auto sub_master = dynamic_cast<EbmlMaster *>(e);
      if (sub_master)
        remove_elements_unsupported_by_webm(*sub_master);

    } else
      master.Remove(idx);
  }
}

bool
remove_track_statistics(KaxTags *tags,
                        boost::optional<uint64_t> track_uid) {
  if (!tags)
    return false;

  auto tags_to_discard = std::set<std::string>{
    "_STATISTICS_TAGS",
    "_STATISTICS_WRITING_APP",
    "_STATISTICS_WRITING_DATE_UTC",
  };

  auto const wanted_target_type = static_cast<unsigned int>(mtx::tags::Movie);

  for (auto const &tag_elt : *tags) {
    auto tag = dynamic_cast<KaxTag *>(tag_elt);
    if (!tag)
      continue;

    auto targets = FindChild<KaxTagTargets>(tag);
    if (!targets || (FindChildValue<KaxTagTargetTypeValue>(targets, wanted_target_type) != wanted_target_type))
      continue;

    for (auto const &simple_tag_elt : *tag) {
      auto simple_tag = dynamic_cast<KaxTagSimple *>(simple_tag_elt);
      if (!simple_tag)
        continue;

      auto simple_tag_name = mtx::tags::get_simple_name(*simple_tag);
      if (simple_tag_name != "_STATISTICS_TAGS")
        continue;

      auto all_to_discard = split(mtx::tags::get_simple_value(*simple_tag), boost::regex{"\\s+", boost::regex::perl});
      for (auto const &to_discard : all_to_discard)
        tags_to_discard.insert(to_discard);
    }
  }

  auto removed_something = false;

  for (auto const &tag_name : tags_to_discard) {
    auto removed_something_here = mtx::tags::remove_simple_tags_for<KaxTagTrackUID>(*tags, track_uid, tag_name);
    removed_something           = removed_something || removed_something_here;
  }

  return removed_something;
}

}}
