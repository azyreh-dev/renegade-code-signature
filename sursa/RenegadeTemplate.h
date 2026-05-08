#pragma once

#include <QString>
#include <QStringList>

class RenegadeTemplate
{
public:
    enum class CommentStyle
    {
        CStyleBlock,
        HashLine,
        HtmlBlock,
        SqlLine,
        Unknown
    };

public:
    static CommentStyle styleForExtension(const QString& extension);

    static QString buildHeader(
        const QString& extension,
        const QString& projectName,
        const QString& author,
        const QString& brand,
        const QString& license
    );

    static QStringList supportedExtensions();

private:
    static QString buildCStyleBlock(
        const QString& projectName,
        const QString& author,
        const QString& brand,
        const QString& license
    );

    static QString buildHashLine(
        const QString& projectName,
        const QString& author,
        const QString& brand,
        const QString& license
    );

    static QString buildHtmlBlock(
        const QString& projectName,
        const QString& author,
        const QString& brand,
        const QString& license
    );

    static QString buildSqlLine(
        const QString& projectName,
        const QString& author,
        const QString& brand,
        const QString& license
    );
};
