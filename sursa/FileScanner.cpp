#include "FileScanner.h"

#include <QDir>
#include <QDirIterator>
#include <QFileInfo>

QStringList FileScanner::scanFiles(
    const QString& rootFolder,
    const QStringList& extensions,
    const QStringList& excludedFolders
)
{
    QStringList result;

    if (rootFolder.trimmed().isEmpty())
    {
        return result;
    }

    QDir root(rootFolder);

    if (!root.exists())
    {
        return result;
    }

    QStringList normalizedExtensions;

    for (const QString& extension : extensions)
    {
        normalizedExtensions.append(normalizedExtension(extension));
    }

    QDirIterator iterator(
        rootFolder,
        QDir::Files,
        QDirIterator::Subdirectories
    );

    while (iterator.hasNext())
    {
        const QString filePath = iterator.next();

        if (shouldSkipPath(filePath, excludedFolders))
        {
            continue;
        }

        QFileInfo info(filePath);
        const QString extension = info.suffix().toLower();

        if (normalizedExtensions.contains(extension))
        {
            result.append(filePath);
        }
    }

    result.sort(Qt::CaseInsensitive);

    return result;
}

bool FileScanner::shouldSkipPath(
    const QString& path,
    const QStringList& excludedFolders
)
{
    const QString normalizedPath = QDir::fromNativeSeparators(path).toLower();

    for (const QString& folder : excludedFolders)
    {
        const QString token = "/" + folder.toLower() + "/";

        if (normalizedPath.contains(token))
        {
            return true;
        }
    }

    return false;
}

QString FileScanner::normalizedExtension(const QString& extension)
{
    QString normalized = extension.trimmed().toLower();

    if (normalized.startsWith('.'))
    {
        normalized.remove(0, 1);
    }

    return normalized;
}