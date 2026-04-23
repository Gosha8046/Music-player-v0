#include "MainWindow.h"
#include "PlaylistModel.h"
#include "PlayerControls.h"

#include <QListView>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("MusicPlayer");
    resize(700, 520);

    m_player = new QMediaPlayer(this);
    m_audio  = new QAudioOutput(this);
    m_player->setAudioOutput(m_audio);
    m_audio->setVolume(0.7f);

    m_model = new PlaylistModel(this);

    setupUi();
    setupMenu();
    applyStyle();

    connect(m_player, &QMediaPlayer::playbackStateChanged,
            this, &MainWindow::onPlaybackStateChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged,
            this, &MainWindow::onMediaStatusChanged);
    connect(m_player, &QMediaPlayer::durationChanged,
            m_controls, &PlayerControls::setDuration);
    connect(m_player, &QMediaPlayer::positionChanged,
            m_controls, &PlayerControls::setPosition);

    connect(m_controls, &PlayerControls::playPauseClicked, this, &MainWindow::togglePlayPause);
    connect(m_controls, &PlayerControls::stopClicked,      this, &MainWindow::stop);
    connect(m_controls, &PlayerControls::prevClicked,      this, &MainWindow::prev);
    connect(m_controls, &PlayerControls::nextClicked,      this, &MainWindow::next);
    connect(m_controls, &PlayerControls::seekRequested,    m_player, &QMediaPlayer::setPosition);
    connect(m_controls, &PlayerControls::volumeChanged, this, [this](int v) {
        m_audio->setVolume(v / 100.0f);
    });

    connect(m_listView, &QListView::doubleClicked, this, [this](const QModelIndex &idx) {
        playIndex(idx.row());
    });
}

void MainWindow::setupUi()
{
    auto *central = new QWidget(this);
    setCentralWidget(central);

    m_nowPlaying = new QLabel("No track selected", central);
    m_nowPlaying->setAlignment(Qt::AlignCenter);
    m_nowPlaying->setWordWrap(true);

    m_listView = new QListView(central);
    m_listView->setModel(m_model);
    m_listView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_listView->setAlternatingRowColors(true);

    m_controls = new PlayerControls(central);

    auto *layout = new QVBoxLayout(central);
    layout->addWidget(m_nowPlaying);
    layout->addWidget(m_listView, 1);
    layout->addWidget(m_controls);
}

void MainWindow::setupMenu()
{
    auto *fileMenu = menuBar()->addMenu("File");

    auto *actOpen   = fileMenu->addAction("Add files...");
    auto *actFolder = fileMenu->addAction("Add folder...");
    fileMenu->addSeparator();
    auto *actRemove = fileMenu->addAction("Remove selected");
    auto *actClear  = fileMenu->addAction("Clear playlist");
    fileMenu->addSeparator();
    auto *actQuit   = fileMenu->addAction("Quit");

    connect(actOpen,   &QAction::triggered, this, &MainWindow::openFiles);
    connect(actFolder, &QAction::triggered, this, &MainWindow::openFolder);
    connect(actRemove, &QAction::triggered, this, &MainWindow::removeSelected);
    connect(actClear,  &QAction::triggered, this, &MainWindow::clearPlaylist);
    connect(actQuit,   &QAction::triggered, this, &QWidget::close);
}

void MainWindow::applyStyle()
{
    setStyleSheet(R"(
        QMainWindow, QWidget {
            background: #1e1e2e;
            color: #cdd6f4;
            font-family: Segoe UI, sans-serif;
            font-size: 13px;
        }
        QMenuBar {
            background: #181825;
        }
        QMenuBar::item:selected {
            background: #313244;
        }
        QMenu {
            background: #181825;
            border: 1px solid #313244;
        }
        QMenu::item:selected {
            background: #313244;
        }
        QListView {
            background: #181825;
            border: 1px solid #313244;
            border-radius: 6px;
            outline: none;
        }
        QListView::item {
            padding: 6px 10px;
            border-radius: 4px;
        }
        QListView::item:selected {
            background: #89b4fa;
            color: #1e1e2e;
        }
        QListView::item:alternate {
            background: #1e1e2e;
        }
        QPushButton {
            background: #313244;
            border: none;
            border-radius: 6px;
            color: #cdd6f4;
            font-size: 16px;
        }
        QPushButton:hover {
            background: #45475a;
        }
        QPushButton:pressed {
            background: #89b4fa;
            color: #1e1e2e;
        }
        QSlider::groove:horizontal {
            height: 4px;
            background: #313244;
            border-radius: 2px;
        }
        QSlider::handle:horizontal {
            background: #89b4fa;
            width: 12px;
            height: 12px;
            margin: -4px 0;
            border-radius: 6px;
        }
        QSlider::sub-page:horizontal {
            background: #89b4fa;
            border-radius: 2px;
        }
        QLabel#nowPlaying {
            font-size: 14px;
            font-weight: bold;
            color: #89b4fa;
            padding: 8px;
        }
    )");
    m_nowPlaying->setObjectName("nowPlaying");
}

void MainWindow::openFiles()
{
    auto files = QFileDialog::getOpenFileUrls(
        this, "Add music files", {},
        "Audio files (*.mp3 *.flac *.wav *.ogg *.aac *.m4a *.wma *.opus)"
    );
    if (!files.isEmpty())
        m_model->addFiles(files);
}

void MainWindow::openFolder()
{
    auto folder = QFileDialog::getExistingDirectory(this, "Add music folder");
    if (!folder.isEmpty())
        m_model->addFolder(folder);
}

void MainWindow::removeSelected()
{
    auto idx = m_listView->currentIndex();
    if (idx.isValid())
        m_model->removeTrack(idx.row());
}

void MainWindow::clearPlaylist()
{
    m_player->stop();
    m_model->clear();
    m_currentIndex = -1;
    m_nowPlaying->setText("No track selected");
}

void MainWindow::playIndex(int index)
{
    if (index < 0 || index >= m_model->count()) return;
    m_currentIndex = index;
    m_player->setSource(m_model->urlAt(index));
    m_player->play();
    updateNowPlaying();

    auto idx = m_model->index(index);
    m_listView->setCurrentIndex(idx);
}

void MainWindow::onPlaybackStateChanged(QMediaPlayer::PlaybackState state)
{
    m_controls->setPlaying(state == QMediaPlayer::PlayingState);
}

void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia)
        next();
}

void MainWindow::togglePlayPause()
{
    if (m_player->playbackState() == QMediaPlayer::PlayingState) {
        m_player->pause();
    } else if (m_player->source().isEmpty() && m_model->count() > 0) {
        playIndex(0);
    } else {
        m_player->play();
    }
}

void MainWindow::stop()
{
    m_player->stop();
}

void MainWindow::prev()
{
    if (m_currentIndex > 0)
        playIndex(m_currentIndex - 1);
}

void MainWindow::next()
{
    if (m_currentIndex < m_model->count() - 1)
        playIndex(m_currentIndex + 1);
}

void MainWindow::updateNowPlaying()
{
    if (m_currentIndex < 0) return;
    auto url = m_model->urlAt(m_currentIndex);
    m_nowPlaying->setText(QFileInfo(url.toLocalFile()).completeBaseName());
}
