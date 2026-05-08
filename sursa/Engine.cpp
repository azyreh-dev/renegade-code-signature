#include "Engine.h"

#include "RenegadeTemplate.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

StampResult Engine::applyStamp(
    const QStringList& files,
    const QString& projectName,
    const QString& author,
    const QString& brand,
    const QString& license,
    bool createBackups
)
{
    StampResult result;
    result.totalFiles = files.size();

    for (const QString& filePath : files)
    {
        QString errorMessage;

        const bool success = stampSingleFile(
            filePath,
            projectName,
            author,
            brand,
            license,
            createBackups,
            errorMessage
        );

        if (success)
        {
            result.modifiedFiles++;
            result.messages.append("Modificat: " + filePath);
        }
        else
        {
            if (errorMessage == "FISIER_DEJA_SEMNAT")
            {
                result.skippedFiles++;
                result.messages.append("Sărit, are deja semnătură: " + filePath);
            }
            else
            {
                result.failedFiles++;
                result.messages.append("Eroare: " + filePath + " | " + errorMessage);
            }
        }
    }

    return result;
}

bool Engine::fileAlreadyStamped(const QString& content)
{
    return content.contains("Semnătură generată cu Renegade", Qt::CaseInsensitive);
}

bool Engine::stampSingleFile(
    const QString& filePath,
    const QString& projectName,
    const QString& author,
    const QString& brand,
    const QString& license,
    bool createBackups,
    QString& errorMessage
)
{
    QFileInfo info(filePath);
    const QString extension = info.suffix().toLower();

    const QString header = RenegadeTemplate::buildHeader(
        extension,
        projectName,
        author,
        brand,
        license
    );

    if (header.isEmpty())
    {
        errorMessage = "Extensie nesuportată.";
        return false;
    }

    QFile inputFile(filePath);

    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        errorMessage = "Fișierul nu poate fi deschis pentru citire.";
        return false;
    }

    QTextStream inputStream(&inputFile);
    inputStream.setEncoding(QStringConverter::Utf8);

    const QString originalContent = inputStream.readAll();

    inputFile.close();

    if (fileAlreadyStamped(originalContent))
    {
        errorMessage = "FISIER_DEJA_SEMNAT";
        return false;
    }

    if (createBackups)
    {
        if (!createBackupFile(filePath, errorMessage))
        {
            return false;
        }
    }

    const int insertPosition = insertionPositionForContent(originalContent);

    QString newContent = originalContent;
    newContent.insert(insertPosition, header);

    QFile outputFile(filePath);

    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        errorMessage = "Fișierul nu poate fi deschis pentru scriere.";
        return false;
    }

    QTextStream outputStream(&outputFile);
    outputStream.setEncoding(QStringConverter::Utf8);
    outputStream << newContent;

    outputFile.close();

    return true;
}

bool Engine::createBackupFile(
    const QString& filePath,
    QString& errorMessage
)
{
    const QString backupPath = filePath + ".renegade.bak";

    if (QFile::exists(backupPath))
    {
        QFile::remove(backupPath);
    }

    if (!QFile::copy(filePath, backupPath))
    {
        errorMessage = "Copia de siguranță nu a putut fi creată.";
        return false;
    }

    return true;
}

int Engine::insertionPositionForContent(const QString& content)
{
    int position = 0;

    if (content.startsWith("#!"))
    {
        const int newlineIndex = content.indexOf('\n');

        if (newlineIndex >= 0)
        {
            position = newlineIndex + 1;
        }
    }

    const QString remainingContent = content.mid(position);

    if (remainingContent.startsWith("# -*- coding:") ||
        remainingContent.startsWith("# coding:"))
    {
        const int newlineIndex = content.indexOf('\n', position);

        if (newlineIndex >= 0)
        {
            position = newlineIndex + 1;
        }
    }

    return position;
}