#include "MainWindow.h"

#include "Engine.h"
#include "FileScanner.h"
#include "RenegadeTemplate.h"

#include <QApplication>
#include <QAbstractItemView>
#include <QCheckBox>
#include <QClipboard>
#include <QComboBox>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QGridLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QPlainTextEdit>
#include <QPixmap>
#include <QPushButton>
#include <QScrollBar>
#include <QSplitter>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextBlock>
#include <QTextEdit>
#include <QTime>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>

namespace
{
QString logPrefix()
{
    return "[" + QTime::currentTime().toString("HH:mm:ss") + "]  ";
}

QGraphicsDropShadowEffect* glowEffect(const QColor& color, int blurRadius, int offset = 0)
{
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(blurRadius);
    effect->setColor(color);
    effect->setOffset(offset, offset);
    return effect;
}

QToolButton* createIconButton(const QString& iconPath, QWidget* parent)
{
    QToolButton* button = new QToolButton(parent);
    button->setIcon(QIcon(iconPath));
    button->setIconSize(QSize(18, 18));
    button->setAutoRaise(true);
    button->setCursor(Qt::PointingHandCursor);
    button->setFixedSize(30, 30);
    return button;
}

QString coloredLogLine(const QString& message, const QString& color)
{
    const QString escaped = message.toHtmlEscaped();
    const int prefixLength = 12;

    if (escaped.startsWith("[") && escaped.size() > prefixLength)
    {
        return QString("<span style=\"color:#19DFFF;\">%1</span><span style=\"color:%2;\">%3</span>")
            .arg(escaped.left(prefixLength), color, escaped.mid(prefixLength));
    }

    return QString("<span style=\"color:%1;\">%2</span>").arg(color, escaped);
}

class HeaderBanner : public QFrame
{
public:
    explicit HeaderBanner(QWidget* parent = nullptr)
        : QFrame(parent)
    {
        setObjectName("HeaderFrame");
        setMinimumHeight(100);
    }

protected:
    void paintEvent(QPaintEvent* event) override
    {
        QFrame::paintEvent(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        const QRect circuitArea(width() * 0.48, 0, width() * 0.50, height());
        QPen cyan(QColor(25, 223, 255, 150), 1.2);
        QPen blue(QColor(47, 121, 255, 90), 1.0);
        QPen purple(QColor(169, 77, 255, 120), 1.1);

        auto drawTrace = [&](const QPen& pen, int y, int xOffset) {
            painter.setPen(pen);
            QPainterPath path;
            path.moveTo(circuitArea.left() + xOffset, y);
            path.lineTo(circuitArea.left() + xOffset + 110, y);
            path.lineTo(circuitArea.left() + xOffset + 138, y - 22);
            path.lineTo(circuitArea.left() + xOffset + 270, y - 22);
            path.lineTo(circuitArea.left() + xOffset + 300, y - 44);
            path.lineTo(circuitArea.right() - 10, y - 44);
            painter.drawPath(path);

            painter.setBrush(pen.color());
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(QPointF(circuitArea.left() + xOffset + 160, y - 22), 1.6, 1.6);
            painter.drawEllipse(QPointF(circuitArea.left() + xOffset + 318, y - 44), 1.6, 1.6);
        };

        drawTrace(cyan, height() - 24, 12);
        drawTrace(blue, height() - 50, 98);
        drawTrace(purple, height() - 8, 210);
    }
};

class CodePreviewEdit;

class LineNumberArea : public QWidget
{
public:
    explicit LineNumberArea(CodePreviewEdit* editor);
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    CodePreviewEdit* codeEditor = nullptr;
};

class CodePreviewEdit : public QPlainTextEdit
{
public:
    explicit CodePreviewEdit(QWidget* parent = nullptr)
        : QPlainTextEdit(parent)
    {
        lineNumberArea = new LineNumberArea(this);

        connect(this, &QPlainTextEdit::blockCountChanged, this, [this]() { updateLineNumberAreaWidth(); });
        connect(this, &QPlainTextEdit::updateRequest, this, [this](const QRect& rect, int dy) {
            if (dy)
            {
                lineNumberArea->scroll(0, dy);
            }
            else
            {
                lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
            }

            if (rect.contains(viewport()->rect()))
            {
                updateLineNumberAreaWidth();
            }
        });

        setReadOnly(true);
        setObjectName("CodePreview");
        updateLineNumberAreaWidth();
    }

    int lineNumberAreaWidth() const
    {
        int digits = 1;
        int max = qMax(1, blockCount());

        while (max >= 10)
        {
            max /= 10;
            digits++;
        }

        return 22 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    }

    void lineNumberAreaPaintEvent(QPaintEvent* event)
    {
        QPainter painter(lineNumberArea);
        painter.fillRect(event->rect(), QColor("#07111D"));
        painter.setPen(QColor("#647589"));

        QTextBlock block = firstVisibleBlock();
        int blockNumber = block.blockNumber();
        int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
        int bottom = top + qRound(blockBoundingRect(block).height());

        while (block.isValid() && top <= event->rect().bottom())
        {
            if (block.isVisible() && bottom >= event->rect().top())
            {
                painter.drawText(
                    0,
                    top,
                    lineNumberArea->width() - 8,
                    fontMetrics().height(),
                    Qt::AlignRight,
                    QString::number(blockNumber + 1)
                );
            }

            block = block.next();
            top = bottom;
            bottom = top + qRound(blockBoundingRect(block).height());
            blockNumber++;
        }
    }

protected:
    void resizeEvent(QResizeEvent* event) override
    {
        QPlainTextEdit::resizeEvent(event);
        const QRect content = contentsRect();
        lineNumberArea->setGeometry(QRect(content.left(), content.top(), lineNumberAreaWidth(), content.height()));
    }

private:
    void updateLineNumberAreaWidth()
    {
        setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
    }

    QWidget* lineNumberArea = nullptr;
};

LineNumberArea::LineNumberArea(CodePreviewEdit* editor)
    : QWidget(editor),
      codeEditor(editor)
{
}

QSize LineNumberArea::sizeHint() const
{
    return QSize(codeEditor->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent* event)
{
    codeEditor->lineNumberAreaPaintEvent(event);
}
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    buildUi();
    updateHeaderPreview();
    setStatusMessage("Gata.");
}

QWidget* MainWindow::createSectionFrame(const QString& titleText, const QString& iconPath)
{
    QFrame* frame = new QFrame(this);
    frame->setObjectName("CardFrame");

    QVBoxLayout* layout = new QVBoxLayout(frame);
    layout->setContentsMargins(16, 10, 16, 14);
    layout->setSpacing(9);

    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setSpacing(8);

    QLabel* icon = new QLabel(frame);
    icon->setFixedSize(20, 20);
    icon->setAlignment(Qt::AlignCenter);

    if (!iconPath.isEmpty())
    {
        icon->setPixmap(QIcon(iconPath).pixmap(18, 18));
    }

    QLabel* title = new QLabel(titleText, frame);
    title->setObjectName("SectionTitleLabel");
    title->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    titleLayout->addWidget(icon);
    titleLayout->addWidget(title, 1);
    layout->addLayout(titleLayout);

    return frame;
}

void MainWindow::buildUi()
{
    QWidget* central = new QWidget(this);
    central->setObjectName("CentralWidget");

    QVBoxLayout* rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(10, 8, 10, 0);
    rootLayout->setSpacing(8);

    buildHeaderSection(rootLayout);

    QSplitter* splitter = new QSplitter(Qt::Horizontal, central);
    splitter->setChildrenCollapsible(false);

    QWidget* leftWidget = new QWidget(splitter);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0, 0, 6, 0);
    leftLayout->setSpacing(8);

    QWidget* rightWidget = new QWidget(splitter);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(6, 0, 0, 0);
    rightLayout->setSpacing(8);

    buildLeftColumn(leftLayout);
    buildRightColumn(rightLayout);

    splitter->addWidget(leftWidget);
    splitter->addWidget(rightWidget);
    splitter->setStretchFactor(0, 41);
    splitter->setStretchFactor(1, 59);

    rootLayout->addWidget(splitter, 1);
    buildStatusStrip(rootLayout);

    setCentralWidget(central);
    setStatusBar(nullptr);
}

void MainWindow::buildHeaderSection(QVBoxLayout* rootLayout)
{
    HeaderBanner* header = new HeaderBanner(this);

    QHBoxLayout* headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(12, 2, 8, 8);
    headerLayout->setSpacing(14);

    QLabel* logoLabel = new QLabel(header);
    QPixmap logo(":/logo_renegade.svg");
    logoLabel->setPixmap(logo.scaled(92, 92, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setFixedSize(98, 92);
    logoLabel->setGraphicsEffect(glowEffect(QColor(25, 223, 255, 180), 28));

    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->setSpacing(2);
    textLayout->setAlignment(Qt::AlignVCenter);

    QLabel* title = new QLabel("Renegade", header);
    title->setObjectName("TitleLabel");

    QLabel* subtitle = new QLabel("Adaugă semnătura ta în fișierele sursă ale proiectelor.", header);
    subtitle->setObjectName("SubtitleLabel");

    textLayout->addStretch();
    textLayout->addWidget(title);
    textLayout->addWidget(subtitle);
    textLayout->addStretch();

    headerLayout->addWidget(logoLabel, 0, Qt::AlignVCenter);
    headerLayout->addLayout(textLayout, 1);

    rootLayout->addWidget(header, 0);
}

void MainWindow::buildLeftColumn(QVBoxLayout* layout)
{
    buildProjectSection(layout);
    buildExtensionSection(layout);
    buildActionSection(layout);
    layout->addStretch();
}

void MainWindow::buildRightColumn(QVBoxLayout* layout)
{
    buildPreviewSection(layout);
    buildFileLogSection(layout);
}

void MainWindow::buildProjectSection(QVBoxLayout* parentLayout)
{
    QWidget* frame = createSectionFrame("Proiect", ":/icon_folder.svg");
    QVBoxLayout* wrapper = qobject_cast<QVBoxLayout*>(frame->layout());

    QGridLayout* grid = new QGridLayout();
    grid->setHorizontalSpacing(10);
    grid->setVerticalSpacing(10);

    folderInput = new QLineEdit(frame);
    folderInput->setText("C:\\Dezvoltare\\ProiectulMeu");

    browseButton = new QPushButton("Alege folder", frame);
    browseButton->setObjectName("PrimaryButton");
    browseButton->setMinimumHeight(26);
    browseButton->setIcon(QIcon(":/icon_folder.svg"));

    projectNameInput = new QLineEdit(frame);
    projectNameInput->setText("ProiectulMeu");

    authorInput = new QLineEdit(frame);
    authorInput->setText("Azyreh");

    brandInput = new QLineEdit(frame);
    brandInput->setText("Renegade");

    licenseInput = new QComboBox(frame);
    licenseInput->addItems({"MIT", "Apache 2.0", "GPLv3", "BSD 3-Clause", "Proprietară"});
    licenseInput->setCurrentText("MIT");

    grid->addWidget(new QLabel("Folder:", frame), 0, 0);
    grid->addWidget(folderInput, 0, 1);
    grid->addWidget(browseButton, 0, 2);

    grid->addWidget(new QLabel("Proiect:", frame), 1, 0);
    grid->addWidget(projectNameInput, 1, 1, 1, 2);

    grid->addWidget(new QLabel("Autor:", frame), 2, 0);
    grid->addWidget(authorInput, 2, 1, 1, 2);

    grid->addWidget(new QLabel("Brand:", frame), 3, 0);
    grid->addWidget(brandInput, 3, 1, 1, 2);

    grid->addWidget(new QLabel("Licență:", frame), 4, 0);
    grid->addWidget(licenseInput, 4, 1, 1, 2);

    grid->setColumnStretch(1, 1);

    wrapper->addLayout(grid);

    connect(browseButton, &QPushButton::clicked, this, &MainWindow::browseFolder);
    connect(projectNameInput, &QLineEdit::textChanged, this, &MainWindow::updateHeaderPreview);
    connect(authorInput, &QLineEdit::textChanged, this, &MainWindow::updateHeaderPreview);
    connect(brandInput, &QLineEdit::textChanged, this, &MainWindow::updateHeaderPreview);
    connect(licenseInput, &QComboBox::currentTextChanged, this, &MainWindow::updateHeaderPreview);

    parentLayout->addWidget(frame);
}

void MainWindow::buildExtensionSection(QVBoxLayout* parentLayout)
{
    QWidget* frame = createSectionFrame("Extensii fișiere", ":/icon_code.svg");
    QVBoxLayout* wrapper = qobject_cast<QVBoxLayout*>(frame->layout());

    QGridLayout* grid = new QGridLayout();
    grid->setHorizontalSpacing(10);
    grid->setVerticalSpacing(10);

    const QStringList extensions = RenegadeTemplate::supportedExtensions();

    int row = 0;
    int col = 0;

    for (const QString& ext : extensions)
    {
        QCheckBox* check = new QCheckBox("." + ext, frame);
        check->setObjectName("ExtensionCheck");
        check->setMinimumHeight(26);

        if (ext == "cpp" || ext == "h" || ext == "hpp" || ext == "py" || ext == "js")
        {
            check->setChecked(true);
        }

        extensionCheckboxes.append(check);
        grid->addWidget(check, row, col);

        col++;
        if (col >= 6)
        {
            col = 0;
            row++;
        }
    }

    wrapper->addLayout(grid);
    parentLayout->addWidget(frame);
}

void MainWindow::buildActionSection(QVBoxLayout* parentLayout)
{
    QWidget* frame = createSectionFrame("Acțiuni", ":/icon_lightning.svg");
    QVBoxLayout* wrapper = qobject_cast<QVBoxLayout*>(frame->layout());

    backupCheckbox = new QCheckBox("Creează copii de siguranță .renegade.bak", frame);
    backupCheckbox->setChecked(true);

    scanButton = new QPushButton("Scanează fișiere", frame);
    scanButton->setObjectName("PrimaryButton");
    scanButton->setIcon(QIcon(":/icon_search.svg"));
    scanButton->setMinimumHeight(38);
    scanButton->setGraphicsEffect(glowEffect(QColor(25, 223, 255, 160), 18));

    applyButton = new QPushButton("Aplică semnătura Renegade", frame);
    applyButton->setObjectName("PrimaryButton");
    applyButton->setIcon(QIcon(":/icon_shield.svg"));
    applyButton->setMinimumHeight(38);
    applyButton->setGraphicsEffect(glowEffect(QColor(25, 223, 255, 160), 18));

    copyPreviewButton = new QPushButton("Copiază previzualizarea", frame);
    copyPreviewButton->setObjectName("SecondaryButton");
    copyPreviewButton->setIcon(QIcon(":/icon_clipboard.svg"));
    copyPreviewButton->setMinimumHeight(38);
    copyPreviewButton->setGraphicsEffect(glowEffect(QColor(169, 77, 255, 150), 18));

    QHBoxLayout* row = new QHBoxLayout();
    row->setSpacing(10);
    row->addWidget(scanButton);
    row->addWidget(applyButton);

    wrapper->addWidget(backupCheckbox);
    wrapper->addLayout(row);
    wrapper->addWidget(copyPreviewButton);

    connect(scanButton, &QPushButton::clicked, this, &MainWindow::scanFiles);
    connect(applyButton, &QPushButton::clicked, this, &MainWindow::applyStamp);
    connect(copyPreviewButton, &QPushButton::clicked, this, &MainWindow::copyPreviewToClipboard);

    parentLayout->addWidget(frame);
}

void MainWindow::buildPreviewSection(QVBoxLayout* parentLayout)
{
    QWidget* frame = createSectionFrame("Previzualizare semnătură", ":/icon_eye.svg");
    QVBoxLayout* wrapper = qobject_cast<QVBoxLayout*>(frame->layout());

    headerPreview = new CodePreviewEdit(frame);
    headerPreview->setReadOnly(true);
    headerPreview->setMinimumHeight(138);

    wrapper->addWidget(headerPreview);
    parentLayout->addWidget(frame);
}

void MainWindow::buildFileLogSection(QVBoxLayout* parentLayout)
{
    QWidget* frame = createSectionFrame("Fișiere găsite și jurnal", ":/icon_table.svg");
    QVBoxLayout* wrapper = qobject_cast<QVBoxLayout*>(frame->layout());

    fileTable = new QTableWidget(frame);
    fileTable->setColumnCount(2);
    fileTable->setHorizontalHeaderLabels({"Fișier", "Cale relativă"});
    fileTable->horizontalHeader()->setStretchLastSection(true);
    fileTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    fileTable->verticalHeader()->setVisible(false);
    fileTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    fileTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    fileTable->setMinimumHeight(126);
    fileTable->setShowGrid(true);
    fileTable->setAlternatingRowColors(false);

    logOutput = new QTextEdit(frame);
    logOutput->setObjectName("LogOutput");
    logOutput->setReadOnly(true);
    logOutput->setMinimumHeight(148);
    logOutput->setAcceptRichText(true);

    wrapper->addWidget(fileTable);
    wrapper->addWidget(logOutput, 1);

    parentLayout->addWidget(frame, 1);
}

void MainWindow::buildStatusStrip(QVBoxLayout* rootLayout)
{
    QFrame* strip = new QFrame(this);
    strip->setObjectName("StatusStrip");
    strip->setFixedHeight(34);

    QHBoxLayout* layout = new QHBoxLayout(strip);
    layout->setContentsMargins(16, 0, 12, 0);
    layout->setSpacing(9);

    QLabel* icon = new QLabel(strip);
    icon->setPixmap(QIcon(":/icon_check.svg").pixmap(20, 20));
    icon->setFixedSize(22, 22);

    statusText = new QLabel("Gata.", strip);
    statusText->setObjectName("StatusText");

    layout->addWidget(icon);
    layout->addWidget(statusText, 1);
    layout->addWidget(createIconButton(":/icon_help.svg", strip));
    layout->addWidget(createIconButton(":/icon_settings.svg", strip));
    layout->addWidget(createIconButton(":/icon_info.svg", strip));

    rootLayout->addWidget(strip);
}

void MainWindow::browseFolder()
{
    const QString folder = QFileDialog::getExistingDirectory(this, "Selectează folderul proiectului");
    if (folder.isEmpty())
    {
        return;
    }

    folderInput->setText(folder);
    setStatusMessage("Folder selectat.");
}

void MainWindow::scanFiles()
{
    const QString folder = folderInput->text().trimmed();

    if (folder.isEmpty())
    {
        QMessageBox::warning(this, "Renegade", "Selectează mai întâi folderul proiectului.");
        return;
    }

    scannedFiles = FileScanner::scanFiles(folder, selectedExtensions(), excludedFolders());
    refreshFileList(scannedFiles);

    appendLogLine(logPrefix() + "Scanare finalizată.", "#74E052");
    appendLogLine(logPrefix() + "Fișiere găsite: " + QString::number(scannedFiles.size()), "#BDEFFF");
    logOutput->append("");

    setStatusMessage("Scanare finalizată.");
}

void MainWindow::updateHeaderPreview()
{
    const QString preview = RenegadeTemplate::buildHeader(
        "cpp",
        projectNameInput ? projectNameInput->text() : "ProiectulMeu",
        authorInput ? authorInput->text() : "Azyreh",
        brandInput ? brandInput->text() : "Renegade",
        licenseInput ? licenseInput->currentText() : "MIT"
    );

    if (headerPreview)
    {
        headerPreview->setPlainText(preview);
    }
}

void MainWindow::applyStamp()
{
    if (scannedFiles.isEmpty())
    {
        QMessageBox::warning(this, "Renegade", "Scanează fișierele înainte de aplicarea semnăturii.");
        return;
    }

    if (QMessageBox::question(
            this,
            "Aplică semnătura Renegade",
            "Această acțiune va modifica fișierele sursă găsite. Continui?",
            QMessageBox::Yes | QMessageBox::No
        ) != QMessageBox::Yes)
    {
        return;
    }

    StampResult result = Engine::applyStamp(
        scannedFiles,
        projectNameInput->text().trimmed(),
        authorInput->text().trimmed(),
        brandInput->text().trimmed(),
        licenseInput->currentText().trimmed(),
        backupCheckbox->isChecked()
    );

    appendLogLine(logPrefix() + "Aplicare finalizată.", "#74E052");
    appendLogLine(logPrefix() + "Modificate: " + QString::number(result.modifiedFiles), "#74E052");
    appendLogLine(logPrefix() + "Sărite: " + QString::number(result.skippedFiles), "#FFD166");
    appendLogLine(logPrefix() + "Eșuate: " + QString::number(result.failedFiles), "#FF4A5F");

    for (const QString& message : result.messages)
    {
        const QString color = message.startsWith("Eroare:")
            ? "#FF4A5F"
            : (message.startsWith("Sărit") ? "#FFD166" : "#BDEFFF");
        appendLogLine(logPrefix() + message, color);
    }

    logOutput->append("");

    QMessageBox::information(
        this,
        "Renegade",
        "Semnătura a fost aplicată.\n\nModificate: " + QString::number(result.modifiedFiles) +
        "\nSărite: " + QString::number(result.skippedFiles) +
        "\nEșuate: " + QString::number(result.failedFiles)
    );

    setStatusMessage("Semnătura a fost aplicată.");
}

void MainWindow::copyPreviewToClipboard()
{
    if (!headerPreview)
    {
        return;
    }

    QApplication::clipboard()->setText(headerPreview->toPlainText());
    setStatusMessage("Previzualizarea a fost copiată.");
}

QStringList MainWindow::selectedExtensions() const
{
    QStringList result;

    for (QCheckBox* check : extensionCheckboxes)
    {
        if (check && check->isChecked())
        {
            QString ext = check->text();
            ext.remove('.');
            result.append(ext);
        }
    }

    return result;
}

QStringList MainWindow::excludedFolders() const
{
    return {
        ".git",
        ".vs",
        "build",
        "cmake-build-debug",
        "cmake-build-release",
        "node_modules",
        "venv",
        ".venv",
        "__pycache__",
        "dist",
        "out",
        "bin",
        "obj",
        "target"
    };
}

void MainWindow::appendLogLine(const QString& message, const QString& color)
{
    if (!logOutput)
    {
        return;
    }

    logOutput->append(coloredLogLine(message, color));
    logOutput->verticalScrollBar()->setValue(logOutput->verticalScrollBar()->maximum());
}

void MainWindow::refreshFileList(const QStringList& files)
{
    fileTable->setRowCount(0);
    const QDir root(folderInput ? folderInput->text().trimmed() : QString());

    for (const QString& filePath : files)
    {
        QFileInfo info(filePath);
        const int row = fileTable->rowCount();
        fileTable->insertRow(row);

        QTableWidgetItem* nameItem = new QTableWidgetItem(QIcon(":/icon_file.svg"), info.fileName());
        const QString displayPath = root.exists() ? root.relativeFilePath(filePath) : filePath;
        QTableWidgetItem* pathItem = new QTableWidgetItem(QDir::toNativeSeparators(displayPath));

        fileTable->setItem(row, 0, nameItem);
        fileTable->setItem(row, 1, pathItem);
    }
}

void MainWindow::setStatusMessage(const QString& message)
{
    if (statusText)
    {
        statusText->setText(message);
    }
}
