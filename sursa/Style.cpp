#include "Style.h"

QString Style::darkTheme()
{
    return R"(
        QWidget {
            background-color: #050B12;
            color: #BDEFFF;
            font-family: "Segoe UI";
            font-size: 9pt;
        }

        QMainWindow,
        QWidget#CentralWidget {
            background-color: #050B12;
        }

        QLabel {
            color: #BDEFFF;
            background: transparent;
        }

        QLabel#TitleLabel {
            color: #DDF7FF;
            font-size: 33pt;
            font-style: italic;
            font-weight: 800;
            letter-spacing: 0px;
        }

        QLabel#SubtitleLabel {
            color: #C8D6E5;
            font-size: 10pt;
        }

        QLabel#SectionTitleLabel {
            color: #19DFFF;
            font-size: 9pt;
            font-weight: 800;
        }

        QLabel#StatusText {
            color: #BDEFFF;
            font-size: 9pt;
            font-weight: 700;
        }

        QFrame#HeaderFrame {
            background-color: #050B12;
            border: none;
        }

        QFrame#CardFrame {
            background-color: #071422;
            border: 1px solid #145178;
            border-radius: 8px;
        }

        QFrame#StatusStrip {
            background-color: #071422;
            border-top: 1px solid #0EA7D6;
            border-left: 1px solid #0D344E;
            border-right: 1px solid #0D344E;
            border-top-left-radius: 8px;
            border-top-right-radius: 8px;
        }

        QLineEdit,
        QComboBox {
            background-color: #081420;
            border: 1px solid #1B425E;
            border-radius: 5px;
            color: #D8F3FF;
            min-height: 21px;
            padding: 2px 10px;
            selection-background-color: #19DFFF;
            selection-color: #021018;
        }

        QLineEdit:hover,
        QComboBox:hover {
            border: 1px solid #19DFFF;
        }

        QLineEdit:focus,
        QComboBox:focus {
            border: 1px solid #45EAFF;
            background-color: #0A1A28;
        }

        QComboBox::drop-down {
            border: none;
            width: 24px;
        }

        QComboBox QAbstractItemView {
            background-color: #081420;
            border: 1px solid #145178;
            color: #D8F3FF;
            selection-background-color: #123A56;
        }

        QPlainTextEdit,
        QTextEdit {
            background-color: #050A13;
            border: 1px solid #1A3C58;
            border-radius: 5px;
            color: #74E052;
            font-family: "Cascadia Code", "Consolas", "JetBrains Mono";
            font-size: 9pt;
            padding: 6px;
            selection-background-color: #19DFFF;
            selection-color: #031017;
        }

        QPlainTextEdit#CodePreview {
            color: #74E052;
            padding-left: 0px;
        }

        QTextEdit#LogOutput {
            color: #BDEFFF;
        }

        QTableWidget {
            background-color: #050A13;
            alternate-background-color: #050A13;
            border: 1px solid #1A3C58;
            border-radius: 5px;
            color: #D8F3FF;
            gridline-color: #1A3C58;
            selection-background-color: #0E3954;
            selection-color: #FFFFFF;
        }

        QTableWidget::item {
            padding: 2px 7px;
            border: none;
        }

        QHeaderView::section {
            background-color: #0B1724;
            color: #CFEFFF;
            border: none;
            border-right: 1px solid #1A3C58;
            border-bottom: 1px solid #1A3C58;
            padding: 4px 8px;
            font-weight: 600;
        }

        QPushButton {
            background-color: #0B1A2A;
            border: 1px solid #1D5675;
            border-radius: 5px;
            color: #D8F3FF;
            font-weight: 700;
            padding: 7px 12px;
            text-align: center;
        }

        QPushButton:hover {
            border: 1px solid #19DFFF;
            background-color: #102A42;
        }

        QPushButton:pressed {
            background-color: #06101B;
        }

        QPushButton#PrimaryButton {
            background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #083662, stop:0.55 #0D65B8, stop:1 #072642);
            border: 1px solid #19DFFF;
            color: #DDF7FF;
        }

        QPushButton#PrimaryButton:hover {
            background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #0B477E, stop:0.55 #1280E8, stop:1 #083457);
            border: 1px solid #74F2FF;
        }

        QPushButton#SecondaryButton {
            background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #2A0B4E, stop:0.55 #4B1480, stop:1 #1A0832);
            border: 1px solid #A94DFF;
            color: #F2DFFF;
        }

        QPushButton#SecondaryButton:hover {
            background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #3A106B, stop:0.55 #641BB0, stop:1 #250B48);
            border: 1px solid #D38DFF;
        }

        QToolButton {
            background-color: transparent;
            border: 1px solid transparent;
            border-radius: 15px;
            padding: 4px;
        }

        QToolButton:hover {
            background-color: #0B2132;
            border: 1px solid #19DFFF;
        }

        QCheckBox {
            color: #D8F3FF;
            spacing: 7px;
            background: transparent;
        }

        QCheckBox::indicator {
            width: 13px;
            height: 13px;
            border-radius: 3px;
            border: 1px solid #2B4D63;
            background-color: #081420;
        }

        QCheckBox::indicator:checked {
            background-color: #19DFFF;
            border: 1px solid #58F0FF;
        }

        QCheckBox#ExtensionCheck {
            background-color: #081420;
            border: 1px solid #18334A;
            border-radius: 5px;
            padding: 4px 8px;
        }

        QCheckBox#ExtensionCheck:hover {
            border: 1px solid #19DFFF;
            background-color: #0A1C2B;
        }

        QSplitter::handle {
            background-color: #081420;
            margin: 0px 2px;
        }

        QSplitter::handle:hover {
            background-color: #19DFFF;
        }

        QMessageBox,
        QFileDialog {
            background-color: #071422;
            color: #BDEFFF;
        }

        QScrollBar:vertical {
            background-color: #06101A;
            width: 10px;
            margin: 2px 0px;
            border-radius: 5px;
        }

        QScrollBar::handle:vertical {
            background-color: #1B425E;
            min-height: 24px;
            border-radius: 5px;
        }

        QScrollBar::handle:vertical:hover {
            background-color: #19DFFF;
        }

        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical,
        QScrollBar::add-line:horizontal,
        QScrollBar::sub-line:horizontal {
            width: 0px;
            height: 0px;
        }

        QScrollBar::add-page:vertical,
        QScrollBar::sub-page:vertical,
        QScrollBar::add-page:horizontal,
        QScrollBar::sub-page:horizontal {
            background: transparent;
        }

        QScrollBar:horizontal {
            background-color: #06101A;
            height: 10px;
            margin: 0px 2px;
            border-radius: 5px;
        }

        QScrollBar::handle:horizontal {
            background-color: #1B425E;
            min-width: 24px;
            border-radius: 5px;
        }

        QScrollBar::handle:horizontal:hover {
            background-color: #19DFFF;
        }
    )";
}
