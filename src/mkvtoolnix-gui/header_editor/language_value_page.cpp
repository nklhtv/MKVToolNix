#include "common/common_pch.h"

#include <QAbstractItemView>
#include <QComboBox>

#include "common/iso639.h"
#include "common/qt.h"
#include "mkvtoolnix-gui/header_editor/language_value_page.h"
#include "mkvtoolnix-gui/main_window/main_window.h"
#include "mkvtoolnix-gui/util/language_combo_box.h"
#include "mkvtoolnix-gui/util/widget.h"

namespace mtx::gui::HeaderEditor {

using namespace mtx::gui;

LanguageValuePage::LanguageValuePage(Tab &parent,
                                     PageBase &topLevelPage,
                                     EbmlMaster &master,
                                     EbmlCallbacks const &callbacks,
                                     translatable_string_c const &title,
                                     translatable_string_c const &description)
  : ValuePage{parent, topLevelPage, master, callbacks, ValueType::AsciiString, title, description}
{
}

LanguageValuePage::~LanguageValuePage() {
}

QWidget *
LanguageValuePage::createInputControl() {
  m_originalValue   = m_element ? static_cast<EbmlString *>(m_element)->GetValue() : "eng";
  auto languageOpt  = mtx::iso639::look_up(m_originalValue, true);
  auto currentValue = languageOpt ? languageOpt->alpha_3_code : m_originalValue;

  m_cbValue = new Util::LanguageComboBox{this};
  m_cbValue->setFrame(true);
  m_cbValue->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);

  m_cbValue->setAdditionalItems(Q(currentValue))
    .setup()
    .setCurrentByData(QStringList{} << Q(currentValue) << Q("und"));

  connect(MainWindow::get(), &MainWindow::preferencesChanged, m_cbValue, &Util::ComboBoxBase::reInitialize);

  return m_cbValue;
}

QString
LanguageValuePage::originalValueAsString()
  const {
  return Q(m_originalValue);
}

QString
LanguageValuePage::currentValueAsString()
  const {
  return m_cbValue->currentData().toString();
}

void
LanguageValuePage::resetValue() {
  m_cbValue->setCurrentByData(Q(m_originalValue));
}

bool
LanguageValuePage::validateValue()
  const {
  return true;
}

void
LanguageValuePage::copyValueToElement() {
  static_cast<EbmlString *>(m_element)->SetValue(to_utf8(currentValueAsString()));
}

}
