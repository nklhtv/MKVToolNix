#include "common/common_pch.h"

#include "common/iso639.h"
#include "common/strings/formatting.h"
#include "common/qt.h"
#include "mkvtoolnix-gui/forms/merge/select_playlist_dialog.h"
#include "mkvtoolnix-gui/merge/select_playlist_dialog.h"
#include "mkvtoolnix-gui/merge/track.h"
#include "mkvtoolnix-gui/util/container.h"
#include "mkvtoolnix-gui/util/model.h"
#include "mkvtoolnix-gui/util/settings.h"
#include "mkvtoolnix-gui/util/widget.h"

#include <QPushButton>

namespace mtx::gui::Merge {

class ScannedFileItem: public QTreeWidgetItem {
public:
  SourceFile const *m_file;

public:
  ScannedFileItem(SourceFile const &file, QStringList const &texts);
  virtual bool operator <(QTreeWidgetItem const &cmp) const;

public:
  static ScannedFileItem *create(SourceFile const &file);
};

ScannedFileItem::ScannedFileItem(SourceFile const &file,
                                 QStringList const &texts)
  : QTreeWidgetItem{texts}
  , m_file{&file}
{
}

bool
ScannedFileItem::operator <(QTreeWidgetItem const &cmp)
  const {
  auto otherFile = static_cast<ScannedFileItem const &>(cmp).m_file;
  auto column    = treeWidget()->sortColumn();

  return 0 == column ? m_file->m_fileName         < otherFile->m_fileName
       : 1 == column ? m_file->m_playlistDuration < otherFile->m_playlistDuration
       : 2 == column ? m_file->m_playlistChapters < otherFile->m_playlistChapters
       :               m_file->m_playlistSize     < otherFile->m_playlistSize;
}

ScannedFileItem *
ScannedFileItem::create(SourceFile const &scannedFile) {
  auto item = new ScannedFileItem{ scannedFile, QStringList{
      QFileInfo{scannedFile.m_fileName}.fileName(),
      to_qs(mtx::string::format_timestamp(scannedFile.m_playlistDuration, 0)),
      QString::number(scannedFile.m_playlistChapters),
      to_qs(mtx::string::format_file_size(scannedFile.m_playlistSize)),
    }};

  for (auto column = 1; column <= 3; ++column)
    item->setTextAlignment(column, Qt::AlignRight | Qt::AlignVCenter);

  return item;
}

// ------------------------------------------------------------

class TrackItem: public QTreeWidgetItem {
public:
  Track const *m_track;

public:
  TrackItem(Track const &track, QStringList const &texts);
  virtual bool operator <(QTreeWidgetItem const &cmp) const;

public:
  static TrackItem *create(Track const &track);
};

TrackItem::TrackItem(Track const &track,
                     QStringList const &texts)
  : QTreeWidgetItem{texts}
  , m_track{&track}
{
}

bool
TrackItem::operator <(QTreeWidgetItem const &cmp)
  const {
  auto otherTrack = static_cast<TrackItem const &>(cmp).m_track;
  auto column     = treeWidget()->sortColumn();

  return 1 == column                                     ? m_track->nameForType()          <  otherTrack->nameForType()
       : 2 == column                                     ? m_track->m_codec                <  otherTrack->m_codec
       : 3 == column                                     ? Q(m_track->m_language.format()) <  Q(otherTrack->m_language.format())
       : (m_track->m_id >= 0) && (otherTrack->m_id >= 0) ? m_track->m_id                   <  otherTrack->m_id
       : (m_track->m_id <  0) && (otherTrack->m_id <  0) ? m_track->m_codec                <  otherTrack->m_codec
       :                                                   m_track->m_id                   >= 0;
}

TrackItem *
TrackItem::create(Track const &track) {
  auto item = new TrackItem{ track, QStringList{
      track.m_id >= 0 ? QString::number(track.m_id) : QString{},
      track.nameForType(),
      track.m_codec,
      Q(track.m_language.format()),
    }};

  item->setTextAlignment(0, Qt::AlignRight | Qt::AlignVCenter);

  return item;
}

// ------------------------------------------------------------

class PlaylistItem: public QTreeWidgetItem {
public:
  unsigned int m_order{};
  QString const m_fileName, m_path;

public:
  PlaylistItem(unsigned int order, QFileInfo const &fileInfo);
  virtual bool operator <(QTreeWidgetItem const &cmp) const;

public:
  static PlaylistItem *create(unsigned int order, QFileInfo const &fileInfo);
};

PlaylistItem::PlaylistItem(unsigned int order,
                           QFileInfo const &fileInfo)
  : QTreeWidgetItem{ QStringList{} << QString::number(order) << fileInfo.fileName() << QDir::toNativeSeparators(fileInfo.path()) }
  , m_order{order}
  , m_fileName{fileInfo.fileName()}
  , m_path{fileInfo.path()}
{
}

bool
PlaylistItem::operator <(QTreeWidgetItem const &cmp)
  const {
  auto otherItem = static_cast<PlaylistItem const &>(cmp);
  auto column    = treeWidget()->sortColumn();

  return 0 == column ? m_order    < otherItem.m_order
       : 1 == column ? m_fileName < otherItem.m_fileName
       :               m_path     < otherItem.m_path;
}

PlaylistItem *
PlaylistItem::create(unsigned int order,
                     QFileInfo const &fileInfo) {
  auto item = new PlaylistItem{order, fileInfo};

  item->setTextAlignment(0, Qt::AlignRight | Qt::AlignVCenter);

  return item;
}

// ------------------------------------------------------------

class DiscLibraryItem: public QTreeWidgetItem {
public:
  mtx::bluray::disc_library::info_t m_info;

public:
  DiscLibraryItem(mtx::bluray::disc_library::info_t const &info, QStringList const &texts);
  virtual bool operator <(QTreeWidgetItem const &cmp) const;

public:
  static DiscLibraryItem *create(std::string const &language, mtx::bluray::disc_library::info_t const &info);
};

DiscLibraryItem::DiscLibraryItem(mtx::bluray::disc_library::info_t const &info,
                                 QStringList const &texts)
  : QTreeWidgetItem{texts}
  , m_info{info}
{
}

bool
DiscLibraryItem::operator <(QTreeWidgetItem const &cmp)
  const {
  auto &otherItem = static_cast<DiscLibraryItem const &>(cmp);
  auto column     = treeWidget()->sortColumn();

  if (column != 3)
    return text(column).toLower() < otherItem.text(column).toLower();

  auto mySize    = std::make_pair(          data(3, Qt::UserRole).toUInt(),           data(3, Qt::UserRole + 1).toUInt());
  auto otherSize = std::make_pair(otherItem.data(3, Qt::UserRole).toUInt(), otherItem.data(3, Qt::UserRole + 1).toUInt());

  return mySize < otherSize;
}

DiscLibraryItem *
DiscLibraryItem::create(std::string const &language,
                        mtx::bluray::disc_library::info_t const &info) {
  auto biggestThumbnail = info.m_thumbnails.begin();
  auto end              = info.m_thumbnails.end();

  for (auto currentThumbnail = info.m_thumbnails.begin(); currentThumbnail != end; ++currentThumbnail) {
    auto size = currentThumbnail->m_width * currentThumbnail->m_height;

    if (size > (biggestThumbnail->m_width * biggestThumbnail->m_height))
      biggestThumbnail = currentThumbnail;
  }

  auto languageOpt  = mtx::iso639::look_up(language);
  auto languageName = languageOpt ? Q(languageOpt->english_name) : Q(language);

  auto item = new DiscLibraryItem{info, QStringList{
    languageName,
    Q(info.m_title),
    biggestThumbnail == end ? Q("") : Q(biggestThumbnail->m_file_name.filename().u8string()),
    biggestThumbnail == end ? Q("") : Q("%1x%2").arg(biggestThumbnail->m_width).arg(biggestThumbnail->m_height),
  }};

  item->setData(0, Qt::UserRole, Q(language));

  if (biggestThumbnail != end) {
    item->setTextAlignment(3, Qt::AlignRight | Qt::AlignVCenter);
    item->setData(3, Qt::UserRole,     biggestThumbnail->m_width);
    item->setData(3, Qt::UserRole + 1, biggestThumbnail->m_height);
  }

  return item;
}

// ------------------------------------------------------------

SelectPlaylistDialog::SelectPlaylistDialog(QWidget *parent,
                                           QVector<SourceFilePtr> const &scannedFiles,
                                           std::optional<mtx::bluray::disc_library::disc_library_t> const &discLibrary)
  : QDialog{parent, Qt::Dialog | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint}
  , ui{new Ui::SelectPlaylistDialog}
  , m_scannedFiles{scannedFiles}
  , m_discLibrary{discLibrary}
{
  // Setup UI controls.
  ui->setupUi(this);

  setupUi();
}

SelectPlaylistDialog::~SelectPlaylistDialog() {
  Util::saveWidgetGeometry(this);
}

void
SelectPlaylistDialog::setupScannedFiles() {
  auto items = QList<QTreeWidgetItem *>{};
  for (auto const &scannedFile : m_scannedFiles)
    items << ScannedFileItem::create(*scannedFile);

  ui->scannedFiles->insertTopLevelItems(0, items);
  ui->scannedFiles->setSortingEnabled(true);
  ui->scannedFiles->sortItems(1, Qt::DescendingOrder);

  Util::selectRow(ui->scannedFiles, 0);

  Util::resizeViewColumnsToContents(ui->scannedFiles);
}

void
SelectPlaylistDialog::setupDiscLibrary() {
  auto haveLibrary = m_discLibrary && !m_discLibrary->m_infos_by_language.empty();

  ui->discLibraryStack->setCurrentIndex(haveLibrary ? 0 : 1);

  if (!haveLibrary)
    return;

  auto items = QList<QTreeWidgetItem *>{};
  for (auto const &infoItr : m_discLibrary->m_infos_by_language)
    items << DiscLibraryItem::create(infoItr.first, infoItr.second);

  ui->discLibrary->insertTopLevelItems(0, items);
  ui->discLibrary->setSortingEnabled(true);
  ui->discLibrary->sortItems(0, Qt::AscendingOrder);

  int rowToSelect = 0;
  auto model      = ui->discLibrary->model();

  for (auto row = 0, numRows = model->rowCount(); row < numRows; ++row) {
    auto language = model->data(model->index(row, 0), Qt::UserRole).toString();
    if (language == Q("eng")) {
      rowToSelect = row;
      break;
    }
  }

  Util::selectRow(ui->discLibrary, rowToSelect);

  Util::resizeViewColumnsToContents(ui->discLibrary);
}

void
SelectPlaylistDialog::setupUi() {
  auto &cfg = Util::Settings::get();

  setupScannedFiles();
  setupDiscLibrary();

  cfg.handleSplitterSizes(ui->selectPlaylistDialogSplitter1);
  cfg.handleSplitterSizes(ui->selectPlaylistDialogSplitter2);
  cfg.handleSplitterSizes(ui->selectPlaylistDialogSplitter3);

  Util::restoreWidgetGeometry(this);

  auto okButton = Util::buttonForRole(ui->buttonBox, QDialogButtonBox::AcceptRole);
  okButton->setText(QY("&Add"));
  okButton->setDefault(true);

  connect(ui->buttonBox,    &QDialogButtonBox::accepted,      this, &SelectPlaylistDialog::accept);
  connect(ui->buttonBox,    &QDialogButtonBox::rejected,      this, &SelectPlaylistDialog::reject);
  connect(ui->scannedFiles, &QTreeWidget::currentItemChanged, this, &SelectPlaylistDialog::onScannedFileSelected);
  connect(ui->scannedFiles, &QTreeWidget::itemDoubleClicked,  this, &SelectPlaylistDialog::accept);
}

void
SelectPlaylistDialog::onScannedFileSelected(QTreeWidgetItem *current,
                                            QTreeWidgetItem *) {
  auto selectedItem = static_cast<ScannedFileItem *>(current);
  if (!selectedItem)
    return;

  auto const &file = *selectedItem->m_file;

  ui->duration->setText(to_qs(mtx::string::format_timestamp(file.m_playlistDuration, 0)));
  ui->size->setText(to_qs(mtx::string::format_file_size(file.m_playlistSize)));
  ui->numberOfChapters->setText(QString::number(file.m_playlistChapters));

  ui->tracks->setSortingEnabled(false);
  ui->playlistItems->setSortingEnabled(false);

  ui->tracks->clear();
  ui->playlistItems->clear();

  auto newItems = QList<QTreeWidgetItem *>{};
  for (auto const &track : file.m_tracks)
    newItems << TrackItem::create(*track);

  ui->tracks->insertTopLevelItems(0, newItems);

  newItems.clear();
  unsigned int order{};
  for (auto const &playlistFile : file.m_playlistFiles)
    newItems << PlaylistItem::create(++order, playlistFile);

  ui->playlistItems->insertTopLevelItems(0, newItems);

  ui->tracks->setSortingEnabled(true);
  ui->tracks->sortItems(ui->tracks->sortColumn(), Qt::AscendingOrder);

  ui->playlistItems->setSortingEnabled(true);
  ui->playlistItems->sortItems(ui->playlistItems->sortColumn(), Qt::AscendingOrder);

  Util::resizeViewColumnsToContents(ui->tracks);
  Util::resizeViewColumnsToContents(ui->playlistItems);
}

QVector<SourceFilePtr>
SelectPlaylistDialog::select() {
  if (exec() == QDialog::Rejected)
    return {};

  QVector<SourceFilePtr> selectedSourceFiles;
  Util::withSelectedIndexes(ui->scannedFiles, [this, &selectedSourceFiles](QModelIndex const &sourceFileIdx) {
      if (!sourceFileIdx.isValid())
        return;

      auto fileItem = dynamic_cast<ScannedFileItem *>(ui->scannedFiles->invisibleRootItem()->child(sourceFileIdx.row()));
      if (!fileItem)
        return;

      auto idx = Util::findPtr(fileItem->m_file, m_scannedFiles);
      if (idx < 0)
        return;

      selectedSourceFiles << m_scannedFiles[idx];
    });

  if (selectedSourceFiles.isEmpty())
    return {};

  auto discLibraryIdx = Util::selectedRowIdx(ui->discLibrary);

  if (!discLibraryIdx.isValid())
    return selectedSourceFiles;

  auto libraryItem = dynamic_cast<DiscLibraryItem *>(ui->discLibrary->invisibleRootItem()->child(discLibraryIdx.row()));
  if (libraryItem)
    for (auto &sourceFile : selectedSourceFiles)
      sourceFile->m_discLibraryInfoToAdd = libraryItem->m_info;

  return selectedSourceFiles;
}

}
