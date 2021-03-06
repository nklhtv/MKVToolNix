#pragma once

#include "common/common_pch.h"

#include <QDialog>

#include "mkvtoolnix-gui/util/settings.h"

class QItemSelection;
class QListWidget;
class QModelIndex;

namespace mtx::gui {

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog {
  Q_OBJECT

public:
  enum class Page {
    Gui,
    OftenUsedSelections,
    Merge,
    PredefinedValues,
    DefaultValues,
    DeriveTrackLanguage,
    Output,
    EnablingTracks,
    Playlists,
    Info,
    HeaderEditor,
    ChapterEditor,
    Jobs,
    RunPrograms,

    PreviouslySelected,
  };

protected:
  static Page ms_previouslySelectedPage;

  // UI stuff:
  std::unique_ptr<Ui::PreferencesDialog> ui;
  Util::Settings &m_cfg;
  QString const m_previousUiLocale;
  bool const m_previousDisableToolTips;
  double m_previousProbeRangePercentage;
  QMap<Page, int> m_pageIndexes;
  bool m_ignoreNextCurrentChange;

public:
  explicit PreferencesDialog(QWidget *parent, Page pageToShow);
  ~PreferencesDialog();

  void save();
  bool uiLocaleChanged() const;
  bool disableToolTipsChanged() const;
  bool probeRangePercentageChanged() const;

public Q_SLOTS:
  void editDefaultAdditionalCommandLineOptions();
  void enableOutputFileNameControls();
  void browseMediaInfoExe();
  void browseFixedOutputDirectory();
  void pageSelectionChanged(QItemSelection const &selection);
  void addProgramToExecute();
  void removeProgramToExecute(int index);
  void setSendersTabTitleForRunProgramWidget();
  void adjustPlaylistControls();
  void adjustRemoveOldJobsControls();
  void revertDeriveTrackLanguageFromFileNameChars();
  void setupCommonLanguages(bool withISO639_3);

  void enableOftendUsedLanguagesOnly();
  void enableOftendUsedRegionsOnly();
  void enableOftendUsedCharacterSetsOnly();

  virtual void accept() override;
  virtual void reject() override;

protected:
  void setupPageSelector(Page pageToShow);
  void setupToolTips();
  void setupConnections();

  void setupBCP47LanguageEditMode();
  void setupInterfaceLanguage();
  void setupTabPositions();
  void setupDerivingTrackLanguagesFromFileName();
  void setupWhenToSetDefaultLanguage();
  void setupJobRemovalPolicy();
  void setupCommonRegions();
  void setupCommonCharacterSets();
  void setupProcessPriority();
  void setupPlaylistScanningPolicy();
  void setupOutputFileNamePolicy();
  void setupRecentDestinationDirectoryList();
  void setupTrackPropertiesLayout();
  void setupEnableMuxingTracksByType();
  void setupEnableMuxingTracksByLanguage();
  void setupMergeAddingAppendingFilesPolicy();
  void setupMergeWarnMissingAudioTrack();
  void setupMergePredefinedItems();
  void setupHeaderEditorDroppedFilesPolicy();
  void setupJobsRunPrograms();
  void setupFontAndScaling();

  void showPage(Page page);

  void setTabTitleForRunProgramWidget(int tabIdx, QString const &title);

  QModelIndex modelIndexForPage(int pageIndex);

  bool verifyDeriveTrackLanguageSettings();
  bool verifyRunProgramConfigurations();

  void rememberCurrentlySelectedPage();
};

}
