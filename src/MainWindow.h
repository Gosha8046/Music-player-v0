#pragma once

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>

class PlaylistModel;
class PlayerControls;
class QListView;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void openFiles();
    void openFolder();
    void removeSelected();
    void clearPlaylist();
    void playIndex(int index);
    void onPlaybackStateChanged(QMediaPlayer::PlaybackState state);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void togglePlayPause();
    void stop();
    void prev();
    void next();

private:
    void setupUi();
    void setupMenu();
    void applyStyle();
    void updateNowPlaying();

    QMediaPlayer   *m_player    = nullptr;
    QAudioOutput   *m_audio     = nullptr;
    PlaylistModel  *m_model     = nullptr;
    PlayerControls *m_controls  = nullptr;
    QListView      *m_listView  = nullptr;
    QLabel         *m_nowPlaying = nullptr;

    int m_currentIndex = -1;
};
