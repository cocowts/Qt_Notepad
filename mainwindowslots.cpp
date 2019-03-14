#include "MainWindow.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QMap>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QList>
#include <QFileInfo>
#include <QObjectList>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QPrintDialog>
#include <QPageSetupDialog>
#include <QPrinter>
#include <QApplication>
#include <QKeyEvent>
#include <QInputDialog>
#include <QTextCursor>
#include <QToolBar>
#include <QStatusBar>
#include <QDateTime>
#include <QFontDialog>
#include <QFont>
#include <QPlainTextEdit>
#include <QDesktopServices>
#include <QUrl>
#include <QPrintPreviewDialog>
#include <QProcess>
#include <QTextCodec>

#include <QDebug>

QString MainWindow::showFileDialog(QFileDialog::AcceptMode mode, QString title, QString icon)
{
    QFileDialog fd(this);
    QStringList filters;
    QMap<QString, QString> map;
    const char* filterArray[][2] =
    {
        {"文本文档(*.txt)", ".txt"},
        {"所有文件(*.*)"  , ".*"   },
        {nullptr         , nullptr}
    };
    QString ret = "";

    for(int i=0; filterArray[i][0]!=nullptr; i++)
    {
        filters.append(filterArray[i][0]);
        map.insert(filterArray[i][0], filterArray[i][1]);
    }

    fd.setWindowTitle(title);
    fd.setWindowIcon(QIcon(icon));
    fd.setAcceptMode(QFileDialog::AcceptOpen);
    fd.setNameFilters(filters);

    if( mode == QFileDialog::AcceptOpen )
    {
        fd.setFileMode(QFileDialog::ExistingFile);
    }

    if( fd.exec() == QFileDialog::Accepted )
    {
        ret = fd.selectedFiles()[0];

        if( mode == QFileDialog::AcceptSave )
        {
            QString postfix = map[fd.selectedNameFilter()];

            if( (postfix != ".*") && !ret.endsWith(postfix) )
            {
                ret = ret + postfix;
            }
        }
    }

    return ret;
}

void MainWindow::showErrorMessage(QString message)
{
    QMessageBox msg(this);

    msg.setWindowTitle("记事本");
    msg.setWindowFlag(Qt::Drawer);
    msg.setIcon(QMessageBox::Critical);
    msg.setText(message);
    msg.setStandardButtons(QMessageBox::Ok);

    msg.exec();
}

int MainWindow::showQueryMessage(QString message)
{
    QMessageBox msg(this);

    msg.setIcon(QMessageBox::Question);
    msg.setWindowTitle("记事本");
    msg.setWindowFlag(Qt::Drawer);
    msg.setText(message);
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    return msg.exec();
}

void MainWindow::preEditChange()
{
    if( m_isTextChanged )
    {
        QString path = (m_filePath != "") ? m_filePath : "无标题";
        int r = showQueryMessage(QString("是否将更改保存到\n") + "\"" + path + "\" ?");

        switch ( r )
        {
        case QMessageBox::Yes:
            saveCurrentData("保存", m_filePath);
            break;
        case QMessageBox::No:
            m_isTextChanged = false;
            break;
        case QMessageBox::Cancel:
            break;
        }
    }
}

void MainWindow::openFileEditor(QString path)
{
    if( path != "" )
    {
        QFile file(path);

        if( file.open(QIODevice::ReadOnly | QIODevice::Text) )
        {
            QTextStream in(&file);
            in.setCodec("GBK");

            mainEditor.setPlainText(in.readAll());

            file.close();

            m_filePath = path;

            m_isTextChanged = false;

            setWindowTitle(m_filePath + "- 记事本");
        }
        else
        {
            showErrorMessage(QString("打开文件失败!\n\n") + "\"" + path + "\"。");
       }
    }
}

void MainWindow::openFile(QString path)
{
    preEditChange();

    if( !m_isTextChanged )
    {
        openFileEditor(path);
    }
}

void MainWindow::onFileOpen()
{
    preEditChange();

    if( !m_isTextChanged )
    {
        QString path = showFileDialog(QFileDialog::AcceptOpen, "打开", ":/Res/pic/logo.png");

        openFileEditor(path);
    }
}

QString MainWindow::saveCurrentData(QString title, QString path)
{
    QString ret = path;

    if( ret == "" )
    {
        ret = showFileDialog(QFileDialog::AcceptSave, title, ":/Res/pic/logo.png");
    }

    if( ret != "" )
    {
        QFile file(ret);

        if( file.open(QIODevice::WriteOnly | QIODevice::Text) )
        {
            QTextStream out(&file);

            out << mainEditor.toPlainText();

            file.close();

            setWindowTitle(ret + " - 记事本");

            m_isTextChanged = false;
        }
        else
        {
            showErrorMessage(QString("保存文件失败!\n\n") + "\"。" + ret + "\"");
        }
    }

    return ret;
}

void MainWindow::onFileSave()
{
    QString path = saveCurrentData("保存", m_filePath);

    if( path != "" )
    {
        m_filePath = path;
    }
}

void MainWindow::onFileSaveAs()
{
    QString path = saveCurrentData("另存为");

    if( path != "" )
    {
        m_filePath = path;
    }
}

void MainWindow::onFileNew()
{
    preEditChange();

    if( !m_isTextChanged )
    {
        mainEditor.clear();

        m_isTextChanged = false;

        setWindowTitle("新建文本文档 - 记事本");
    }
}

void MainWindow::onFilePageSetup()
{
    m_pPageSetupDlg->exec();
}

void MainWindow::onFilePrint()
{
    QPrintDialog dlg(this);

    dlg.setWindowTitle("打印");

    if( dlg.exec() == QPrintDialog::Accepted )
    {
        QPrinter* p = dlg.printer();

        p->setPageLayout(m_pPageSetupDlg->printer()->pageLayout());

        mainEditor.document()->print(p);
    }
}

void MainWindow::onTextChanged()
{
    if( !m_isTextChanged )
    {
        setWindowTitle("* " + windowTitle());
    }

    m_isTextChanged = true;

    statusLabel.setText("length: " + QString::number(mainEditor.toPlainText().length()) + "    lines: " + QString::number(mainEditor.document()->lineCount()));
}

void MainWindow::onEditDelete()
{
    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier);
    QKeyEvent keyRelease(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier);

    QApplication::sendEvent(&mainEditor, &keyPress);
    QApplication::sendEvent(&mainEditor, &keyRelease);
}

void MainWindow::onEditFind()
{
    m_pFindDlg->show();
}

void MainWindow::onEditFindNext()
{
     m_pFindDlg->onFindClicked();
}

void MainWindow::onEditReplace()
{
    m_pReplaceDlg->show();
}

void MainWindow::onEditGoto()
{
    bool ok = false;
    int ln = QInputDialog::getInt(this, "转到", "行号:", 1, 1, mainEditor.document()->lineCount(), 1, &ok,
                                  Qt::WindowCloseButtonHint | Qt::Drawer);
    if( ok )
    {
        QString text = mainEditor.toPlainText();
        QTextCursor c = mainEditor.textCursor();
        int pos = 0;
        int next = -1;

        for(int i=0; i<ln; i++)
        {
            pos = next + 1;
            next = text.indexOf('\n', pos);
        }

        c.setPosition(pos);

        mainEditor.setTextCursor(c);
    }
}

void MainWindow::onEditSelectAll()
{
    QTextCursor c = mainEditor.textCursor();
    int length = mainEditor.toPlainText().length();

    c.setPosition(0);
    c.setPosition(length, QTextCursor::KeepAnchor);

    mainEditor.setTextCursor(c);
}

void MainWindow::onEditDate()
{
    mainEditor.insertPlainText("[ " + QDateTime().currentDateTime().toString("hh:mm:ss yyyy/MM/dd") + " ]");
}

void MainWindow::onEditmspaint()
{
    static QProcess process(this);
    process.start("C:/WINDOWS/system32/mspaint.exe");
}

void MainWindow::onEditCalculator()
{
    // 方式 1
    static QProcess process(this);
    process.start("C:/WINDOWS/system32/calc.exe");

    //system("calc");
}

void MainWindow::FormatFont()
{
    bool ok = false;

    QFont font = QFontDialog::getFont(&ok, mainEditor.font(), this, "打印");

    if( ok )
    {
        mainEditor.setFont(font);
    }
}

void MainWindow::FormatWrap()
{
    QPlainTextEdit::LineWrapMode mode = mainEditor.lineWrapMode();

    if( mode == QPlainTextEdit::NoWrap )
    {
        mainEditor.setLineWrapMode(QPlainTextEdit::WidgetWidth);

        findMenuBarAction("自动换行")->setChecked(true);
        findToolBarAction("自动换行")->setChecked(true);
    }
    else
    {
        mainEditor.setLineWrapMode(QPlainTextEdit::NoWrap);

        findMenuBarAction("自动换行")->setChecked(false);
        findToolBarAction("自动换行")->setChecked(false);
    }
}

void MainWindow::onViewToolBar()
{
    QToolBar* tb = toolBar();

    bool visible = tb->isVisible();

    tb->setVisible(!visible);

    findMenuBarAction("工具栏")->setChecked(!visible);
    findToolBarAction("工具栏")->setChecked(!visible);
}

void MainWindow::onViewStatusBar()
{
    QStatusBar* sb = statusBar();
    bool visible = sb->isVisible();

    sb->setVisible(!visible);

    findMenuBarAction("状态栏")->setChecked(!visible);
    findToolBarAction("状态栏")->setChecked(!visible);
}

void MainWindow::onHelpManual()
{
    QDesktopServices::openUrl(QUrl("https://segmentfault.com/u/tiansong"));
}

void MainWindow::onHelpAbort()
{
    AbortDialog(this).exec();
}

void MainWindow::onCopyAvailable(bool available)
{
    findMenuBarAction("复制")->setEnabled(available);
    findToolBarAction("复制")->setEnabled(available);
    findMenuBarAction("剪切")->setEnabled(available);
    findToolBarAction("剪切")->setEnabled(available);
}

void MainWindow::onRedoAvailable(bool available)
{
    findToolBarAction("恢复")->setEnabled(available);
}

void MainWindow::onUndoAvailable(bool available)
{
    findMenuBarAction("撤销")->setEnabled(available);
    findToolBarAction("撤销")->setEnabled(available);
}

void MainWindow::onCursorPositionChanged()
{
    int col = 0;
    int ln = 0;
    int flg = -1;
    int pos = mainEditor.textCursor().position();
    QString text = mainEditor.toPlainText();

    for(int i=0; i<pos; i++)
    {
        if( text[i] == '\n' )
        {
            ln ++;
            flg = i;
        }
    }

    flg ++;

    col = pos - flg;

    statusCursorLabel.setText("Ln: " + QString::number(ln + 1) + "    Col: " + QString::number(col + 1));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    preEditChange();

    if( !m_isTextChanged )
    {
        QFont font = mainEditor.font();
        bool isWrap = (mainEditor.lineWrapMode() == QPlainTextEdit::WidgetWidth);
        bool tbVisible = (findMenuBarAction("工具栏")->isCheckable() && findToolBarAction("工具栏")->isChecked());
        bool sbVisible = (findMenuBarAction("状态栏")->isCheckable() && findToolBarAction("状态栏")->isChecked());
        AppConfig config(mainEditor.font(), size(), pos(), isWrap, tbVisible, sbVisible, this);

        config.store();

        QMainWindow::closeEvent(event);
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
    if( event->mimeData()->hasUrls() )
    {
        event->acceptProposedAction();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent* event)
{
    if( event->mimeData()->hasUrls() )
    {
        QList<QUrl> list = event->mimeData()->urls();
        QString path = list[0].toLocalFile();
        QFileInfo fi(path);

        if( fi.isFile() )
        {
            preEditChange();

            if( !m_isTextChanged )
            {
                openFileEditor(path);
            }
        }
        else
        {
            showErrorMessage(QString("对 ") + "\"" + path + "\" 的访问被拒绝。");
        }
    }
    else
    {
       event->ignore();
    }
}

QAction* MainWindow::findMenuBarAction(QString text)
{
    QAction* ret = nullptr;
    const QObjectList& list = menuBar()->children();

    for(int i=0; i<list.count(); i++)
    {
        QMenu* men = dynamic_cast<QMenu*>(list[i]);

        if( men != nullptr )
        {
            QList<QAction*> actions = men->actions();

            for(int j=0; j<actions.count(); j++)
            {
                if( actions[j]->text().startsWith(text) )
                {
                    ret = actions[j];

                    break;
                }
            }
        }
    }

    return ret;
}

QAction* MainWindow::findToolBarAction(QString text)
{
    QAction* ret = nullptr;

    QList<QAction*> actions = toolBar()->actions();

    for(int j=0; j<actions.count(); j++)
    {
        if( actions[j]->toolTip().startsWith(text) )
        {
            ret = actions[j];
            break;
        }
    }

    return ret;
}
