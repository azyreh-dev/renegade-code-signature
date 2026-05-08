#pragma once

#include <QMainWindow>
#include <QList>
#include <QStringList>

class QCheckBox;
class QLineEdit;
class QComboBox;
class QPlainTextEdit;
class QPushButton;
class QTableWidget;
class QLabel;
class QTextEdit;
class QVBoxLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void browseFolder();
    void scanFiles();
    void updateHeaderPreview();
    void applyStamp();
    void copyPreviewToClipboard();

private:
    void buildUi();
    QWidget* createSectionFrame(const QString& titleText, const QString& iconPath = QString());
    void buildHeaderSection(QVBoxLayout* rootLayout);
    void buildLeftColumn(QVBoxLayout* layout);
    void buildRightColumn(QVBoxLayout* layout);
    void buildProjectSection(QVBoxLayout* parentLayout);
    void buildExtensionSection(QVBoxLayout* parentLayout);
    void buildActionSection(QVBoxLayout* parentLayout);
    void buildPreviewSection(QVBoxLayout* parentLayout);
    void buildFileLogSection(QVBoxLayout* parentLayout);
    void buildStatusStrip(QVBoxLayout* rootLayout);

    QStringList selectedExtensions() const;
    QStringList excludedFolders() const;

    void appendLogLine(const QString& message, const QString& color);
    void refreshFileList(const QStringList& files);
    void setStatusMessage(const QString& message);

private:
    QLineEdit* folderInput = nullptr;
    QLineEdit* projectNameInput = nullptr;
    QLineEdit* authorInput = nullptr;
    QLineEdit* brandInput = nullptr;
    QComboBox* licenseInput = nullptr;

    QPlainTextEdit* headerPreview = nullptr;
    QTextEdit* logOutput = nullptr;

    QTableWidget* fileTable = nullptr;
    QLabel* statusText = nullptr;

    QPushButton* browseButton = nullptr;
    QPushButton* scanButton = nullptr;
    QPushButton* applyButton = nullptr;
    QPushButton* copyPreviewButton = nullptr;

    QCheckBox* backupCheckbox = nullptr;

    QList<QCheckBox*> extensionCheckboxes;

    QStringList scannedFiles;
};
