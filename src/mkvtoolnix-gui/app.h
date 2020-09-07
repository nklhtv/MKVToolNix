#pragma once

#include "common/common_pch.h"

#include <QApplication>
#include <QStringList>

#include "common/qt.h"
#include "mkvtoolnix-gui/gui_cli_parser.h"

class QLocalServer;
class QThread;

namespace mtx::gui {

namespace Util {
class MediaPlayer;
class NetworkAccessManager;
}

namespace Jobs {
class ProgramRunner;
}

using Iso639Language     = std::pair<QString, QString>;
using Iso639LanguageList = std::vector<Iso639Language>;
using Region             = std::pair<QString, QString>;
using RegionList         = std::vector<Region>;
using CharacterSetList   = std::vector<QString>;

class AppPrivate;
class App : public QApplication {
  Q_OBJECT

protected:
  MTX_DECLARE_PRIVATE(AppPrivate)

  std::unique_ptr<AppPrivate> const p_ptr;

  explicit App(AppPrivate &p);


protected:

  explicit App(AppPrivate &d, QWidget *parent);

protected:
  enum class CliCommand {
    OpenConfigFiles,
    AddToMerge,
    EditChapters,
    EditHeaders,
  };

public:
  App(int &argc, char **argv);
  virtual ~App();

  void retranslateUi();
  void initializeLocale(QString const &requestedLocale = QString{});

  bool parseCommandLineArguments(QStringList const &args);
  void handleCommandLineArgumentsLocally();

  bool isOtherInstanceRunning() const;
  void sendCommandLineArgumentsToRunningInstance();
  void sendArgumentsToRunningInstance(QStringList const &args);
  void raiseAndActivateRunningInstance();

  void run();

  void disableDarkMode();
  void enableDarkMode();

  Util::NetworkAccessManager &networkAccessManager();

Q_SIGNALS:
  void addingFilesToMergeRequested(QStringList const &fileNames);
  void editingChaptersRequested(QStringList const &fileNames);
  void editingHeadersRequested(QStringList const &fileNames);
  void openConfigFilesRequested(QStringList const &fileNames);
  void runningInfoOnRequested(QStringList const &fileNames);
  void toolRequested(ToolBase *tool);

public Q_SLOTS:
  void saveSettings() const;
  void receiveInstanceCommunication();
  void setupAppearance();

protected:
  void setupInstanceCommunicator();
  void setupNetworkAccessManager();
  Util::MediaPlayer &setupMediaPlayer();
  Jobs::ProgramRunner &setupProgramRunner();
  void setupColorMode();
  void setupUiFont();

public:
  static App *instance();
  static Util::MediaPlayer &mediaPlayer();
  static Jobs::ProgramRunner &programRunner();

  static Iso639LanguageList const &iso639Languages();
  static Iso639LanguageList const &commonIso639Languages();
  static RegionList const &regions();
  static RegionList const &commonRegions();
  static QString const &descriptionForRegion(QString const &code);
  static CharacterSetList const &characterSets();
  static CharacterSetList const &commonCharacterSets();

  static void reinitializeLanguageLists();
  static void initializeLanguageLists();
  static void initializeRegions();
  static void initializeIso639Languages();
  static void initializeCharacterSets();

  static bool isInstalled();

  static QString communicatorSocketName();
  static QString settingsBaseGroupName();

  static void fixLockFileHostName(QString const &lockFilePath);
};

}
