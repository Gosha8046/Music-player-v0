#include "PlaylistModel.h"

#include <QDir>
#include <QDirIterator>

PlaylistModel::PlaylistModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_urls.size();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_urls.size())
        return {};

    if (role == Qt::DisplayRole)
        return m_titles.at(index.row());

    if (role == Qt::ToolTipRole)
        return m_urls.at(index.row()).toLocalFile();

    return {};
}

void PlaylistModel::addFiles(const QList<QUrl> &urls)
{
    for (const QUrl &url : urls) {
        QFileInfo fi(url.toLocalFile());
        if (!isSupportedAudio(fi.suffix())) continue;
        beginInsertRows({}, m_urls.size(), m_urls.size());
        m_urls.append(url);
        m_titles.append(fi.completeBaseName());
        endInsertRows();
    }
}

void PlaylistModel::addFolder(const QString &folderPath)
{
    QDirIterator it(folderPath, QDir::Files, QDirIterator::Subdirectories);
    QList<QUrl> urls;
    while (it.hasNext()) {
        it.next();
        if (isSupportedAudio(it.fileInfo().suffix()))
            urls.append(QUrl::fromLocalFile(it.filePath()));
    }
    addFiles(urls);
}

void PlaylistModel::removeTrack(int index)
{
    if (index < 0 || index >= m_urls.size()) return;
    beginRemoveRows({}, index, index);
    m_urls.removeAt(index);
    m_titles.removeAt(index);
    endRemoveRows();
}

void PlaylistModel::clear()
{
    beginResetModel();
    m_urls.clear();
    m_titles.clear();
    endResetModel();
}

QUrl PlaylistModel::urlAt(int index) const
{
    if (index < 0 || index >= m_urls.size()) return {};
    return m_urls.at(index);
}

int PlaylistModel::count() const
{
    return m_urls.size();
}

bool PlaylistModel::isSupportedAudio(const QString &suffix)
{
    static const QStringList supported = {
        "mp3", "flac", "wav", "ogg", "aac", "m4a", "wma", "opus"
    };
    return supported.contains(suffix.toLower());
}
