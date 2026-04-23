#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QUrl>
#include <QFileInfo>

class PlaylistModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit PlaylistModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addFiles(const QList<QUrl> &urls);
    void addFolder(const QString &folderPath);
    void removeTrack(int index);
    void clear();

    QUrl urlAt(int index) const;
    int count() const;

private:
    QList<QUrl>     m_urls;
    QList<QString>  m_titles;

    static bool isSupportedAudio(const QString &suffix);
};
