#pragma once

#include <QString>
#include <QStringList>

class FileScanner
{
public:
    static QStringList scanFiles(
        const QString& rootFolder,
        const QStringList& extensions,
        const QStringList& excludedFolders
    );

private:
    static bool shouldSkipPath(
        const QString& path,
        const QStringList& excludedFolders
    );

    static QString normalizedExtension(const QString& extension);
};