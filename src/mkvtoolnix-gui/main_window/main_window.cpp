#include "common/common_pch.h"

#include "common/qt.h"
#include "mkvtoolnix-gui/forms/main_window.h"
#include "mkvtoolnix-gui/job_widget/job_widget.h"
#include "mkvtoolnix-gui/main_window/main_window.h"
#include "mkvtoolnix-gui/merge_widget/merge_widget.h"
#include "mkvtoolnix-gui/util/settings.h"
#include "mkvtoolnix-gui/util/util.h"

#include <QIcon>
#include <QLabel>
#include <QStaticText>
#include <QVBoxLayout>

MainWindow *MainWindow::ms_mainWindow = nullptr;

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow{parent}
  , ui{new Ui::MainWindow}
{
  ms_mainWindow = this;

  // Setup UI controls.
  ui->setupUi(this);

  setupToolSelector();

  // Setup window properties.
  setWindowIcon(Util::loadIcon(Q("mkvmergeGUI.png"), QList<int>{} << 32 << 48 << 64 << 128 << 256));
}

MainWindow::~MainWindow() {
  delete ui;
}

void
MainWindow::setStatusBarMessage(QString const &message) {
  ui->statusBar->showMessage(message, 3000);
}

Ui::MainWindow *
MainWindow::getUi() {
  return ui;
}

QWidget *
MainWindow::createNotImplementedWidget() {
  auto widget   = new QWidget{ui->tool};
  auto vlayout  = new QVBoxLayout{widget};
  auto hlayout  = new QHBoxLayout;
  auto text     = new QLabel{widget};

  text->setText(QY("This has not been implemented yet."));

  hlayout->addItem(new QSpacerItem{1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum});
  hlayout->addWidget(text);
  hlayout->addItem(new QSpacerItem{1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum});

  vlayout->addItem(new QSpacerItem{1, 1, QSizePolicy::Minimum,   QSizePolicy::Expanding});
  vlayout->addItem(hlayout);
  vlayout->addItem(new QSpacerItem{1, 1, QSizePolicy::Minimum,   QSizePolicy::Expanding});

  return widget;
}

void
MainWindow::setupToolSelector() {
  // ui->tool->setIconSize(QSize{48, 48});

  auto toolMerge = new MergeWidget{ui->tool};
  auto toolJobs  = new JobWidget{ui->tool};

  ui->tool->insertTab(0, toolMerge,                    QIcon{":/icons/48x48/merge.png"},                      QY("merge"));
  ui->tool->insertTab(1, createNotImplementedWidget(), QIcon{":/icons/48x48/split.png"},                      QY("extract"));
  ui->tool->insertTab(2, createNotImplementedWidget(), QIcon{":/icons/48x48/document-preview-archive.png"},   QY("info"));
  ui->tool->insertTab(3, createNotImplementedWidget(), QIcon{":/icons/48x48/document-edit.png"},              QY("edit headers"));
  ui->tool->insertTab(4, createNotImplementedWidget(), QIcon{":/icons/48x48/story-editor.png"},               QY("edit chapters"));
  ui->tool->insertTab(5, createNotImplementedWidget(), QIcon{":/icons/48x48/document-edit-sign-encrypt.png"}, QY("edit tags"));
  ui->tool->insertTab(6, toolJobs,                     QIcon{":/icons/48x48/view-task.png"},                  QY("job queue"));

  for (int i = 0; 6 >= i; i++)
    ui->tool->setTabEnabled(i, true);

  ui->tool->setCurrentIndex(0);
}

MainWindow *
MainWindow::get() {
  return ms_mainWindow;
}
