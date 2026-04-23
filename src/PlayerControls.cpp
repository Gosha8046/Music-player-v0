#include "PlayerControls.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>

PlayerControls::PlayerControls(QWidget *parent)
    : QWidget(parent)
{
    m_btnPrev   = new QPushButton("⏮", this);
    m_btnPlay   = new QPushButton("▶", this);
    m_btnStop   = new QPushButton("⏹", this);
    m_btnNext   = new QPushButton("⏭", this);
    m_timeLabel = new QLabel("0:00 / 0:00", this);
    m_seekBar   = new QSlider(Qt::Horizontal, this);
    m_volSlider = new QSlider(Qt::Horizontal, this);

    m_volSlider->setRange(0, 100);
    m_volSlider->setValue(70);
    m_volSlider->setMaximumWidth(100);
    m_seekBar->setRange(0, 0);

    for (auto *btn : {m_btnPrev, m_btnPlay, m_btnStop, m_btnNext})
        btn->setFixedSize(40, 40);

    auto *btnRow = new QHBoxLayout;
    btnRow->addWidget(m_btnPrev);
    btnRow->addWidget(m_btnPlay);
    btnRow->addWidget(m_btnStop);
    btnRow->addWidget(m_btnNext);
    btnRow->addStretch();
    btnRow->addWidget(new QLabel("🔊", this));
    btnRow->addWidget(m_volSlider);

    auto *seekRow = new QHBoxLayout;
    seekRow->addWidget(m_seekBar);
    seekRow->addWidget(m_timeLabel);

    auto *root = new QVBoxLayout(this);
    root->addLayout(seekRow);
    root->addLayout(btnRow);

    connect(m_btnPlay,   &QPushButton::clicked, this, &PlayerControls::playPauseClicked);
    connect(m_btnStop,   &QPushButton::clicked, this, &PlayerControls::stopClicked);
    connect(m_btnPrev,   &QPushButton::clicked, this, &PlayerControls::prevClicked);
    connect(m_btnNext,   &QPushButton::clicked, this, &PlayerControls::nextClicked);
    connect(m_volSlider, &QSlider::valueChanged, this, &PlayerControls::volumeChanged);
    connect(m_seekBar,   &QSlider::sliderMoved,  this, &PlayerControls::onSeekMoved);
}

void PlayerControls::setPlaying(bool playing)
{
    m_btnPlay->setText(playing ? "⏸" : "▶");
}

void PlayerControls::setDuration(qint64 ms)
{
    m_duration = ms;
    m_seekBar->setRange(0, static_cast<int>(ms / 1000));
    m_timeLabel->setText("0:00 / " + formatTime(ms));
}

void PlayerControls::setPosition(qint64 ms)
{
    if (!m_seeking)
        m_seekBar->setValue(static_cast<int>(ms / 1000));
    m_timeLabel->setText(formatTime(ms) + " / " + formatTime(m_duration));
}

void PlayerControls::setVolume(int volume)
{
    m_volSlider->setValue(volume);
}

void PlayerControls::onSeekMoved(int value)
{
    emit seekRequested(static_cast<qint64>(value) * 1000);
}

QString PlayerControls::formatTime(qint64 ms)
{
    qint64 s = ms / 1000;
    return QString("%1:%2").arg(s / 60).arg(s % 60, 2, 10, QChar('0'));
}
