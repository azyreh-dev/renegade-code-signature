#pragma once

#include <QString>
#include <QStringList>

struct StampResult
{
    int totalFiles = 0;
    int modifiedFiles = 0;
    int skippedFiles = 0;
    int failedFiles = 0;
    QStringList messages;
};

class Engine
{
public:
    static StampResult applyStamp(
        const QStringList& files,
        const QString& projectName,
        const QString& author,
        const QString& brand,
        const QString& license,
        bool createBackups
    );

    static bool fileAlreadyStamped(const QString& content);

private:
    static bool stampSingleFile(
        const QString& filePath,
        const QString& projectName,
        const QString& author,
        const QString& brand,
        const QString& license,
        bool createBackups,
        QString& errorMessage
    );

    static bool createBackupFile(
        const QString& filePath,
        QString& errorMessage
    );

    static int insertionPositionForContent(const QString& content);
};