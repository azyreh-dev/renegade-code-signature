#include "RenegadeTemplate.h"

#include <QDate>

RenegadeTemplate::CommentStyle RenegadeTemplate::styleForExtension(const QString& extension)
{
    const QString ext = extension.toLower();

    if (ext == "cpp" ||
        ext == "c" ||
        ext == "h" ||
        ext == "hpp" ||
        ext == "cs" ||
        ext == "java" ||
        ext == "js" ||
        ext == "ts" ||
        ext == "css" ||
        ext == "scss" ||
        ext == "rs" ||
        ext == "go")
    {
        return CommentStyle::CStyleBlock;
    }

    if (ext == "py" ||
        ext == "sh" ||
        ext == "ps1" ||
        ext == "rb" ||
        ext == "pl" ||
        ext == "yml" ||
        ext == "yaml" ||
        ext == "toml")
    {
        return CommentStyle::HashLine;
    }

    if (ext == "html" ||
        ext == "xml" ||
        ext == "xaml" ||
        ext == "svg")
    {
        return CommentStyle::HtmlBlock;
    }

    if (ext == "sql")
    {
        return CommentStyle::SqlLine;
    }

    return CommentStyle::Unknown;
}

QString RenegadeTemplate::buildHeader(
    const QString& extension,
    const QString& projectName,
    const QString& author,
    const QString& brand,
    const QString& license
)
{
    const CommentStyle style = styleForExtension(extension);

    switch (style)
    {
    case CommentStyle::CStyleBlock:
        return buildCStyleBlock(projectName, author, brand, license);

    case CommentStyle::HashLine:
        return buildHashLine(projectName, author, brand, license);

    case CommentStyle::HtmlBlock:
        return buildHtmlBlock(projectName, author, brand, license);

    case CommentStyle::SqlLine:
        return buildSqlLine(projectName, author, brand, license);

    case CommentStyle::Unknown:
    default:
        return QString();
    }
}

QStringList RenegadeTemplate::supportedExtensions()
{
    return {
        "cpp",
        "h",
        "hpp",
        "c",
        "cs",
        "java",
        "js",
        "ts",
        "css",
        "scss",
        "py",
        "sh",
        "ps1",
        "rb",
        "html",
        "xml",
        "xaml",
        "sql",
        "yml",
        "yaml",
        "toml",
        "rs",
        "go"
    };
}

QString RenegadeTemplate::buildCStyleBlock(
    const QString& projectName,
    const QString& author,
    const QString& brand,
    const QString& license
)
{
    const QString year = QString::number(QDate::currentDate().year());

    return QString(
        "/*\n"
        " * Proiect: %1\n"
        " * Autor: %2\n"
        " * Brand: %3\n"
        " * Licență: %4\n"
        " * Drepturi de autor: %5 %2\n"
        " * Semnătură generată cu Renegade\n"
        " */\n\n"
    ).arg(projectName, author, brand, license, year);
}

QString RenegadeTemplate::buildHashLine(
    const QString& projectName,
    const QString& author,
    const QString& brand,
    const QString& license
)
{
    const QString year = QString::number(QDate::currentDate().year());

    return QString(
        "# Proiect: %1\n"
        "# Autor: %2\n"
        "# Brand: %3\n"
        "# Licență: %4\n"
        "# Drepturi de autor: %5 %2\n"
        "# Semnătură generată cu Renegade\n\n"
    ).arg(projectName, author, brand, license, year);
}

QString RenegadeTemplate::buildHtmlBlock(
    const QString& projectName,
    const QString& author,
    const QString& brand,
    const QString& license
)
{
    const QString year = QString::number(QDate::currentDate().year());

    return QString(
        "<!--\n"
        "Proiect: %1\n"
        "Autor: %2\n"
        "Brand: %3\n"
        "Licență: %4\n"
        "Drepturi de autor: %5 %2\n"
        "Semnătură generată cu Renegade\n"
        "-->\n\n"
    ).arg(projectName, author, brand, license, year);
}

QString RenegadeTemplate::buildSqlLine(
    const QString& projectName,
    const QString& author,
    const QString& brand,
    const QString& license
)
{
    const QString year = QString::number(QDate::currentDate().year());

    return QString(
        "-- Proiect: %1\n"
        "-- Autor: %2\n"
        "-- Brand: %3\n"
        "-- Licență: %4\n"
        "-- Drepturi de autor: %5 %2\n"
        "-- Semnătură generată cu Renegade\n\n"
    ).arg(projectName, author, brand, license, year);
}