#pragma once

#include <QWidget>

class QPushButton;
class QSlider;
class QLabel;

class PlayerControls : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerControls(QWidget *parent = nullptr);

    void setPlaying(bool playing);
    void setDuration(qint64 ms);
    void setPosition(qint64 ms);
    void setVolume(int volume);

signals:
    void playPauseClicked();
    void stopClicked();
    void prevClicked();
    void nextClicked();
    void seekRequested(qint64 ms);
    void volumeChanged(int volume);

private slots:
    void onSeekMoved(int value);

private:
    QPushButton *m_btnPrev   = nullptr;
    QPushButton *m_btnPlay   = nullptr;
    QPushButton *m_btnStop   = nullptr;
    QPushButton *m_btnNext   = nullptr;
    QSlider     *m_seekBar   = nullptr;
    QSlider     *m_volSlider = nullptr;
    QLabel      *m_timeLabel = nullptr;

    qint64 m_duration = 0;
    bool   m_seeking  = false;

    static QString formatTime(qint64 ms);
};
