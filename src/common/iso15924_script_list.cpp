/*
   mkvmerge -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL v2
   see the file COPYING for details
   or visit https://www.gnu.org/licenses/old-licenses/gpl-2.0.html

   ISO 15924 script list

   Written by Moritz Bunkus <moritz@bunkus.org>.
*/

// -------------------------------------------------------------------------
// NOTE: this file is auto-generated by the "dev:iso15924_list" rake target.
// -------------------------------------------------------------------------

#include "common/common_pch.h"

#include "common/iso15924.h"

namespace mtx::iso15924 {

std::vector<script_t> g_scripts;

void
init() {
  g_scripts.reserve(203);

  g_scripts.emplace_back("Adlm"s, 166, u8"Adlam"s                                                                                          );
  g_scripts.emplace_back("Afak"s, 439, u8"Afaka"s                                                                                          );
  g_scripts.emplace_back("Aghb"s, 239, u8"Caucasian Albanian"s                                                                             );
  g_scripts.emplace_back("Ahom"s, 338, u8"Ahom, Tai Ahom"s                                                                                 );
  g_scripts.emplace_back("Arab"s, 160, u8"Arabic"s                                                                                         );
  g_scripts.emplace_back("Aran"s, 161, u8"Arabic (Nastaliq variant)"s                                                                      );
  g_scripts.emplace_back("Armi"s, 124, u8"Imperial Aramaic"s                                                                               );
  g_scripts.emplace_back("Armn"s, 230, u8"Armenian"s                                                                                       );
  g_scripts.emplace_back("Avst"s, 134, u8"Avestan"s                                                                                        );
  g_scripts.emplace_back("Bali"s, 360, u8"Balinese"s                                                                                       );
  g_scripts.emplace_back("Bamu"s, 435, u8"Bamum"s                                                                                          );
  g_scripts.emplace_back("Bass"s, 259, u8"Bassa Vah"s                                                                                      );
  g_scripts.emplace_back("Batk"s, 365, u8"Batak"s                                                                                          );
  g_scripts.emplace_back("Beng"s, 325, u8"Bengali (Bangla)"s                                                                               );
  g_scripts.emplace_back("Bhks"s, 334, u8"Bhaiksuki"s                                                                                      );
  g_scripts.emplace_back("Blis"s, 550, u8"Blissymbols"s                                                                                    );
  g_scripts.emplace_back("Bopo"s, 285, u8"Bopomofo"s                                                                                       );
  g_scripts.emplace_back("Brah"s, 300, u8"Brahmi"s                                                                                         );
  g_scripts.emplace_back("Brai"s, 570, u8"Braille"s                                                                                        );
  g_scripts.emplace_back("Bugi"s, 367, u8"Buginese"s                                                                                       );
  g_scripts.emplace_back("Buhd"s, 372, u8"Buhid"s                                                                                          );
  g_scripts.emplace_back("Cakm"s, 349, u8"Chakma"s                                                                                         );
  g_scripts.emplace_back("Cans"s, 440, u8"Unified Canadian Aboriginal Syllabics"s                                                          );
  g_scripts.emplace_back("Cari"s, 201, u8"Carian"s                                                                                         );
  g_scripts.emplace_back("Cham"s, 358, u8"Cham"s                                                                                           );
  g_scripts.emplace_back("Cher"s, 445, u8"Cherokee"s                                                                                       );
  g_scripts.emplace_back("Chrs"s, 109, u8"Chorasmian"s                                                                                     );
  g_scripts.emplace_back("Cirt"s, 291, u8"Cirth"s                                                                                          );
  g_scripts.emplace_back("Copt"s, 204, u8"Coptic"s                                                                                         );
  g_scripts.emplace_back("Cpmn"s, 402, u8"Cypro-Minoan"s                                                                                   );
  g_scripts.emplace_back("Cprt"s, 403, u8"Cypriot syllabary"s                                                                              );
  g_scripts.emplace_back("Cyrl"s, 220, u8"Cyrillic"s                                                                                       );
  g_scripts.emplace_back("Cyrs"s, 221, u8"Cyrillic (Old Church Slavonic variant)"s                                                         );
  g_scripts.emplace_back("Deva"s, 315, u8"Devanagari (Nagari)"s                                                                            );
  g_scripts.emplace_back("Diak"s, 342, u8"Dives Akuru"s                                                                                    );
  g_scripts.emplace_back("Dogr"s, 328, u8"Dogra"s                                                                                          );
  g_scripts.emplace_back("Dsrt"s, 250, u8"Deseret (Mormon)"s                                                                               );
  g_scripts.emplace_back("Dupl"s, 755, u8"Duployan shorthand, Duployan stenography"s                                                       );
  g_scripts.emplace_back("Egyd"s,  70, u8"Egyptian demotic"s                                                                               );
  g_scripts.emplace_back("Egyh"s,  60, u8"Egyptian hieratic"s                                                                              );
  g_scripts.emplace_back("Egyp"s,  50, u8"Egyptian hieroglyphs"s                                                                           );
  g_scripts.emplace_back("Elba"s, 226, u8"Elbasan"s                                                                                        );
  g_scripts.emplace_back("Elym"s, 128, u8"Elymaic"s                                                                                        );
  g_scripts.emplace_back("Ethi"s, 430, u8"Ethiopic (Ge??ez)"s                                                                               );
  g_scripts.emplace_back("Geok"s, 241, u8"Khutsuri (Asomtavruli and Nuskhuri)"s                                                            );
  g_scripts.emplace_back("Geor"s, 240, u8"Georgian (Mkhedruli and Mtavruli)"s                                                              );
  g_scripts.emplace_back("Glag"s, 225, u8"Glagolitic"s                                                                                     );
  g_scripts.emplace_back("Gong"s, 312, u8"Gunjala Gondi"s                                                                                  );
  g_scripts.emplace_back("Gonm"s, 313, u8"Masaram Gondi"s                                                                                  );
  g_scripts.emplace_back("Goth"s, 206, u8"Gothic"s                                                                                         );
  g_scripts.emplace_back("Gran"s, 343, u8"Grantha"s                                                                                        );
  g_scripts.emplace_back("Grek"s, 200, u8"Greek"s                                                                                          );
  g_scripts.emplace_back("Gujr"s, 320, u8"Gujarati"s                                                                                       );
  g_scripts.emplace_back("Guru"s, 310, u8"Gurmukhi"s                                                                                       );
  g_scripts.emplace_back("Hanb"s, 503, u8"Han with Bopomofo (alias for Han + Bopomofo)"s                                                   );
  g_scripts.emplace_back("Hang"s, 286, u8"Hangul (Hang??l, Hangeul)"s                                                                       );
  g_scripts.emplace_back("Hani"s, 500, u8"Han (Hanzi, Kanji, Hanja)"s                                                                      );
  g_scripts.emplace_back("Hano"s, 371, u8"Hanunoo (Hanun??o)"s                                                                              );
  g_scripts.emplace_back("Hans"s, 501, u8"Han (Simplified variant)"s                                                                       );
  g_scripts.emplace_back("Hant"s, 502, u8"Han (Traditional variant)"s                                                                      );
  g_scripts.emplace_back("Hatr"s, 127, u8"Hatran"s                                                                                         );
  g_scripts.emplace_back("Hebr"s, 125, u8"Hebrew"s                                                                                         );
  g_scripts.emplace_back("Hira"s, 410, u8"Hiragana"s                                                                                       );
  g_scripts.emplace_back("Hluw"s,  80, u8"Anatolian Hieroglyphs (Luwian Hieroglyphs, Hittite Hieroglyphs)"s                                );
  g_scripts.emplace_back("Hmng"s, 450, u8"Pahawh Hmong"s                                                                                   );
  g_scripts.emplace_back("Hmnp"s, 451, u8"Nyiakeng Puachue Hmong"s                                                                         );
  g_scripts.emplace_back("Hrkt"s, 412, u8"Japanese syllabaries (alias for Hiragana + Katakana)"s                                           );
  g_scripts.emplace_back("Hung"s, 176, u8"Old Hungarian (Hungarian Runic)"s                                                                );
  g_scripts.emplace_back("Inds"s, 610, u8"Indus (Harappan)"s                                                                               );
  g_scripts.emplace_back("Ital"s, 210, u8"Old Italic (Etruscan, Oscan, etc.)"s                                                             );
  g_scripts.emplace_back("Jamo"s, 284, u8"Jamo (alias for Jamo subset of Hangul)"s                                                         );
  g_scripts.emplace_back("Java"s, 361, u8"Javanese"s                                                                                       );
  g_scripts.emplace_back("Jpan"s, 413, u8"Japanese (alias for Han + Hiragana + Katakana)"s                                                 );
  g_scripts.emplace_back("Jurc"s, 510, u8"Jurchen"s                                                                                        );
  g_scripts.emplace_back("Kali"s, 357, u8"Kayah Li"s                                                                                       );
  g_scripts.emplace_back("Kana"s, 411, u8"Katakana"s                                                                                       );
  g_scripts.emplace_back("Khar"s, 305, u8"Kharoshthi"s                                                                                     );
  g_scripts.emplace_back("Khmr"s, 355, u8"Khmer"s                                                                                          );
  g_scripts.emplace_back("Khoj"s, 322, u8"Khojki"s                                                                                         );
  g_scripts.emplace_back("Kitl"s, 505, u8"Khitan large script"s                                                                            );
  g_scripts.emplace_back("Kits"s, 288, u8"Khitan small script"s                                                                            );
  g_scripts.emplace_back("Knda"s, 345, u8"Kannada"s                                                                                        );
  g_scripts.emplace_back("Kore"s, 287, u8"Korean (alias for Hangul + Han)"s                                                                );
  g_scripts.emplace_back("Kpel"s, 436, u8"Kpelle"s                                                                                         );
  g_scripts.emplace_back("Kthi"s, 317, u8"Kaithi"s                                                                                         );
  g_scripts.emplace_back("Lana"s, 351, u8"Tai Tham (Lanna)"s                                                                               );
  g_scripts.emplace_back("Laoo"s, 356, u8"Lao"s                                                                                            );
  g_scripts.emplace_back("Latf"s, 217, u8"Latin (Fraktur variant)"s                                                                        );
  g_scripts.emplace_back("Latg"s, 216, u8"Latin (Gaelic variant)"s                                                                         );
  g_scripts.emplace_back("Latn"s, 215, u8"Latin"s                                                                                          );
  g_scripts.emplace_back("Leke"s, 364, u8"Leke"s                                                                                           );
  g_scripts.emplace_back("Lepc"s, 335, u8"Lepcha (R??ng)"s                                                                                  );
  g_scripts.emplace_back("Limb"s, 336, u8"Limbu"s                                                                                          );
  g_scripts.emplace_back("Lina"s, 400, u8"Linear A"s                                                                                       );
  g_scripts.emplace_back("Linb"s, 401, u8"Linear B"s                                                                                       );
  g_scripts.emplace_back("Lisu"s, 399, u8"Lisu (Fraser)"s                                                                                  );
  g_scripts.emplace_back("Loma"s, 437, u8"Loma"s                                                                                           );
  g_scripts.emplace_back("Lyci"s, 202, u8"Lycian"s                                                                                         );
  g_scripts.emplace_back("Lydi"s, 116, u8"Lydian"s                                                                                         );
  g_scripts.emplace_back("Mahj"s, 314, u8"Mahajani"s                                                                                       );
  g_scripts.emplace_back("Maka"s, 366, u8"Makasar"s                                                                                        );
  g_scripts.emplace_back("Mand"s, 140, u8"Mandaic, Mandaean"s                                                                              );
  g_scripts.emplace_back("Mani"s, 139, u8"Manichaean"s                                                                                     );
  g_scripts.emplace_back("Marc"s, 332, u8"Marchen"s                                                                                        );
  g_scripts.emplace_back("Maya"s,  90, u8"Mayan hieroglyphs"s                                                                              );
  g_scripts.emplace_back("Medf"s, 265, u8"Medefaidrin (Oberi Okaime, Oberi ??kaim??)"s                                                       );
  g_scripts.emplace_back("Mend"s, 438, u8"Mende Kikakui"s                                                                                  );
  g_scripts.emplace_back("Merc"s, 101, u8"Meroitic Cursive"s                                                                               );
  g_scripts.emplace_back("Mero"s, 100, u8"Meroitic Hieroglyphs"s                                                                           );
  g_scripts.emplace_back("Mlym"s, 347, u8"Malayalam"s                                                                                      );
  g_scripts.emplace_back("Modi"s, 324, u8"Modi, Mo?????"s                                                                                     );
  g_scripts.emplace_back("Mong"s, 145, u8"Mongolian"s                                                                                      );
  g_scripts.emplace_back("Moon"s, 218, u8"Moon (Moon code, Moon script, Moon type)"s                                                       );
  g_scripts.emplace_back("Mroo"s, 264, u8"Mro, Mru"s                                                                                       );
  g_scripts.emplace_back("Mtei"s, 337, u8"Meitei Mayek (Meithei, Meetei)"s                                                                 );
  g_scripts.emplace_back("Mult"s, 323, u8"Multani"s                                                                                        );
  g_scripts.emplace_back("Mymr"s, 350, u8"Myanmar (Burmese)"s                                                                              );
  g_scripts.emplace_back("Nand"s, 311, u8"Nandinagari"s                                                                                    );
  g_scripts.emplace_back("Narb"s, 106, u8"Old North Arabian (Ancient North Arabian)"s                                                      );
  g_scripts.emplace_back("Nbat"s, 159, u8"Nabataean"s                                                                                      );
  g_scripts.emplace_back("Newa"s, 333, u8"Newa, Newar, Newari, Nep??la lipi"s                                                               );
  g_scripts.emplace_back("Nkdb"s,  85, u8"Naxi Dongba (na??????i???? to????ba????, Nakhi Tomba)"s                                                   );
  g_scripts.emplace_back("Nkgb"s, 420, u8"Naxi Geba (na??????i???? g??????ba????, 'Na-'Khi ??Gg??-??baw, Nakhi Geba)"s                                  );
  g_scripts.emplace_back("Nkoo"s, 165, u8"N???Ko"s                                                                                           );
  g_scripts.emplace_back("Nshu"s, 499, u8"N??shu"s                                                                                          );
  g_scripts.emplace_back("Ogam"s, 212, u8"Ogham"s                                                                                          );
  g_scripts.emplace_back("Olck"s, 261, u8"Ol Chiki (Ol Cemet???, Ol, Santali)"s                                                              );
  g_scripts.emplace_back("Orkh"s, 175, u8"Old Turkic, Orkhon Runic"s                                                                       );
  g_scripts.emplace_back("Orya"s, 327, u8"Oriya (Odia)"s                                                                                   );
  g_scripts.emplace_back("Osge"s, 219, u8"Osage"s                                                                                          );
  g_scripts.emplace_back("Osma"s, 260, u8"Osmanya"s                                                                                        );
  g_scripts.emplace_back("Palm"s, 126, u8"Palmyrene"s                                                                                      );
  g_scripts.emplace_back("Pauc"s, 263, u8"Pau Cin Hau"s                                                                                    );
  g_scripts.emplace_back("Perm"s, 227, u8"Old Permic"s                                                                                     );
  g_scripts.emplace_back("Phag"s, 331, u8"Phags-pa"s                                                                                       );
  g_scripts.emplace_back("Phli"s, 131, u8"Inscriptional Pahlavi"s                                                                          );
  g_scripts.emplace_back("Phlp"s, 132, u8"Psalter Pahlavi"s                                                                                );
  g_scripts.emplace_back("Phlv"s, 133, u8"Book Pahlavi"s                                                                                   );
  g_scripts.emplace_back("Phnx"s, 115, u8"Phoenician"s                                                                                     );
  g_scripts.emplace_back("Piqd"s, 293, u8"Klingon (KLI pIqaD)"s                                                                            );
  g_scripts.emplace_back("Plrd"s, 282, u8"Miao (Pollard)"s                                                                                 );
  g_scripts.emplace_back("Prti"s, 130, u8"Inscriptional Parthian"s                                                                         );
  g_scripts.emplace_back("Qaaa"s, 900, u8"Reserved for private use (start)"s                                                               );
  g_scripts.emplace_back("Qabx"s, 949, u8"Reserved for private use (end)"s                                                                 );
  g_scripts.emplace_back("Rjng"s, 363, u8"Rejang (Redjang, Kaganga)"s                                                                      );
  g_scripts.emplace_back("Rohg"s, 167, u8"Hanifi Rohingya"s                                                                                );
  g_scripts.emplace_back("Roro"s, 620, u8"Rongorongo"s                                                                                     );
  g_scripts.emplace_back("Runr"s, 211, u8"Runic"s                                                                                          );
  g_scripts.emplace_back("Samr"s, 123, u8"Samaritan"s                                                                                      );
  g_scripts.emplace_back("Sara"s, 292, u8"Sarati"s                                                                                         );
  g_scripts.emplace_back("Sarb"s, 105, u8"Old South Arabian"s                                                                              );
  g_scripts.emplace_back("Saur"s, 344, u8"Saurashtra"s                                                                                     );
  g_scripts.emplace_back("Sgnw"s,  95, u8"SignWriting"s                                                                                    );
  g_scripts.emplace_back("Shaw"s, 281, u8"Shavian (Shaw)"s                                                                                 );
  g_scripts.emplace_back("Shrd"s, 319, u8"Sharada, ????rad??"s                                                                                );
  g_scripts.emplace_back("Shui"s, 530, u8"Shuishu"s                                                                                        );
  g_scripts.emplace_back("Sidd"s, 302, u8"Siddham, Siddha???, Siddham??t???k??"s                                                                 );
  g_scripts.emplace_back("Sind"s, 318, u8"Khudawadi, Sindhi"s                                                                              );
  g_scripts.emplace_back("Sinh"s, 348, u8"Sinhala"s                                                                                        );
  g_scripts.emplace_back("Sogd"s, 141, u8"Sogdian"s                                                                                        );
  g_scripts.emplace_back("Sogo"s, 142, u8"Old Sogdian"s                                                                                    );
  g_scripts.emplace_back("Sora"s, 398, u8"Sora Sompeng"s                                                                                   );
  g_scripts.emplace_back("Soyo"s, 329, u8"Soyombo"s                                                                                        );
  g_scripts.emplace_back("Sund"s, 362, u8"Sundanese"s                                                                                      );
  g_scripts.emplace_back("Sylo"s, 316, u8"Syloti Nagri"s                                                                                   );
  g_scripts.emplace_back("Syrc"s, 135, u8"Syriac"s                                                                                         );
  g_scripts.emplace_back("Syre"s, 138, u8"Syriac (Estrangelo variant)"s                                                                    );
  g_scripts.emplace_back("Syrj"s, 137, u8"Syriac (Western variant)"s                                                                       );
  g_scripts.emplace_back("Syrn"s, 136, u8"Syriac (Eastern variant)"s                                                                       );
  g_scripts.emplace_back("Tagb"s, 373, u8"Tagbanwa"s                                                                                       );
  g_scripts.emplace_back("Takr"s, 321, u8"Takri, ?????kr??, ????????kr??"s                                                                           );
  g_scripts.emplace_back("Tale"s, 353, u8"Tai Le"s                                                                                         );
  g_scripts.emplace_back("Talu"s, 354, u8"New Tai Lue"s                                                                                    );
  g_scripts.emplace_back("Taml"s, 346, u8"Tamil"s                                                                                          );
  g_scripts.emplace_back("Tang"s, 520, u8"Tangut"s                                                                                         );
  g_scripts.emplace_back("Tavt"s, 359, u8"Tai Viet"s                                                                                       );
  g_scripts.emplace_back("Telu"s, 340, u8"Telugu"s                                                                                         );
  g_scripts.emplace_back("Teng"s, 290, u8"Tengwar"s                                                                                        );
  g_scripts.emplace_back("Tfng"s, 120, u8"Tifinagh (Berber)"s                                                                              );
  g_scripts.emplace_back("Tglg"s, 370, u8"Tagalog (Baybayin, Alibata)"s                                                                    );
  g_scripts.emplace_back("Thaa"s, 170, u8"Thaana"s                                                                                         );
  g_scripts.emplace_back("Thai"s, 352, u8"Thai"s                                                                                           );
  g_scripts.emplace_back("Tibt"s, 330, u8"Tibetan"s                                                                                        );
  g_scripts.emplace_back("Tirh"s, 326, u8"Tirhuta"s                                                                                        );
  g_scripts.emplace_back("Toto"s, 294, u8"Toto"s                                                                                           );
  g_scripts.emplace_back("Ugar"s,  40, u8"Ugaritic"s                                                                                       );
  g_scripts.emplace_back("Vaii"s, 470, u8"Vai"s                                                                                            );
  g_scripts.emplace_back("Visp"s, 280, u8"Visible Speech"s                                                                                 );
  g_scripts.emplace_back("Wara"s, 262, u8"Warang Citi (Varang Kshiti)"s                                                                    );
  g_scripts.emplace_back("Wcho"s, 283, u8"Wancho"s                                                                                         );
  g_scripts.emplace_back("Wole"s, 480, u8"Woleai"s                                                                                         );
  g_scripts.emplace_back("Xpeo"s,  30, u8"Old Persian"s                                                                                    );
  g_scripts.emplace_back("Xsux"s,  20, u8"Cuneiform, Sumero-Akkadian"s                                                                     );
  g_scripts.emplace_back("Yezi"s, 192, u8"Yezidi"s                                                                                         );
  g_scripts.emplace_back("Yiii"s, 460, u8"Yi"s                                                                                             );
  g_scripts.emplace_back("Zanb"s, 339, u8"Zanabazar Square (Zanabazarin D??rb??ljin Useg, Xewtee D??rb??ljin Bicig, Horizontal Square Script)"s);
  g_scripts.emplace_back("Zinh"s, 994, u8"Code for inherited script"s                                                                      );
  g_scripts.emplace_back("Zmth"s, 995, u8"Mathematical notation"s                                                                          );
  g_scripts.emplace_back("Zsye"s, 993, u8"Symbols (Emoji variant)"s                                                                        );
  g_scripts.emplace_back("Zsym"s, 996, u8"Symbols"s                                                                                        );
  g_scripts.emplace_back("Zxxx"s, 997, u8"Code for unwritten documents"s                                                                   );
  g_scripts.emplace_back("Zyyy"s, 998, u8"Code for undetermined script"s                                                                   );
  g_scripts.emplace_back("Zzzz"s, 999, u8"Code for uncoded script"s                                                                        );
}

} // namespace mtx::iso15924
