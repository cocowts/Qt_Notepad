#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QLabel>
#include <QPlainTextEdit>
#include <QString>
#include <QFileDialog>
#include <QSharedPointer>
#include <QPageSetupDialog>
#include "FindDialog.h"
#include "ReplaceDialog.h"
#include "AbortDialog.h"
#include "AppConfig.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QPlainTextEdit mainEditor;
    QLabel statusCursorLabel;
    QLabel statusLabel;
    QString m_filePath;
    bool m_isTextChanged;

    QSharedPointer<FindDialog> m_pFindDlg;
    QSharedPointer<ReplaceDialog> m_pReplaceDlg;
    QSharedPointer<QPageSetupDialog> m_pPageSetupDlg;

    MainWindow();
    MainWindow(const MainWindow&);
    MainWindow& operator = (const MainWindow);

    bool construct();

    bool initMenuBar();
    bool initFileMenu(QMenuBar* mb);
    bool initEditMenu(QMenuBar* mb);
    bool initFormatMenu(QMenuBar* mb);
    bool initViewMenu(QMenuBar* mb);
    bool initHelpMenu(QMenuBar* mb);
    bool makeAction(QAction*& action, QWidget* parent, QString text, int key);

    bool initToolBar();
    bool initFileToolItem(QToolBar* tb);
    bool initEditToolItem(QToolBar* tb);
    bool initFormatToolItem(QToolBar* tb);
    bool initViewToolItem(QToolBar* tb);
    bool makeAction(QAction*& action, QWidget* parent, QString tip, QString icon);

    bool initStatusBar();
    bool initMainEditor();

    QString showFileDialog(QFileDialog::AcceptMode mode, QString title, QString icon);
    void showErrorMessage(QString message);
    int showQueryMessage(QString message);
    QString saveCurrentData(QString title = "", QString path = "");
    void preEditChange();
    void openFileEditor(QString path);

    QAction* findMenuBarAction(QString text);
    QAction* findToolBarAction(QString text);

protected:
    void closeEvent(QCloseEvent *event);
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

private slots:
    void onFileNew();
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
    void onFilePageSetup();
    void onFilePrint();

    void onTextChanged();
    void onEditDelete();
    void onEditFind();
    void onEditFindNext();
    void onEditReplace();
    void onEditGoto();
    void onEditSelectAll();
    void onEditDate();
    void onEditCalculator();
    void onEditmspaint();

    void FormatFont();
    void FormatWrap();

    void onViewToolBar();
    void onViewStatusBar();

    void onHelpManual();
    void onHelpAbort();

    void onCopyAvailable(bool available);
    void onRedoAvailable(bool available);
    void onUndoAvailable(bool available);

    void onCursorPositionChanged();

public:
    static MainWindow* NewInstance();
    QToolBar* toolBar();
    void openFile(QString path);
    ~MainWindow();
};

#endif // MAINWINDOW_H
