#include "MainWindow.h"
#include <QKeySequence>
#include <QIcon>
#include <QSize>
#include <QStatusBar>
#include <QPalette>
#include <QTextCodec>

#include <QDebug>

MainWindow::MainWindow() :
    m_pFindDlg(new FindDialog(this, &mainEditor)),
    m_pReplaceDlg(new ReplaceDialog(this, &mainEditor)),
    m_pPageSetupDlg(new QPageSetupDialog(this))
{
    m_filePath = "";
    m_isTextChanged = false; 
}

bool MainWindow::construct()
{
    bool ret = true;
    AppConfig config;

    ret = ret && initMenuBar();
    ret = ret && initToolBar();
    ret = ret && initStatusBar();
    ret = ret && initMainEditor();

    setWindowTitle("新建文本文档 - 记事本");
    setWindowIcon(QIcon(":/Res/pic/logo.png"));
    setAcceptDrops(true);

    if( ret && config.isVilid() )
    {
        mainEditor.setFont(config.editFont());

        if( !config.isAutoWrap() )
        {
            mainEditor.setLineWrapMode(QPlainTextEdit::NoWrap);

            findMenuBarAction("自动换行")->setChecked(false);
            findToolBarAction("自动换行")->setChecked(false);
        }

        if( !config.isToolBarVisible() )
        {
            toolBar()->setVisible(false);

            findMenuBarAction("工具栏")->setChecked(false);
            findToolBarAction("工具栏")->setChecked(false);
        }

        if( !config.isStatusVisible() )
        {
            statusBar()->setVisible(false);

            findMenuBarAction("状态栏")->setChecked(false);
            findToolBarAction("状态栏")->setChecked(false);
        }

        move(config.mainWindowPoint());
        resize(config.mainWindowSize());
    }

    return ret;
}

MainWindow* MainWindow::NewInstance()
{
    MainWindow* ret = new MainWindow();

    if( (ret == nullptr) || !ret->construct() )
    {
        delete ret;

        ret = nullptr;
    }

    return ret;
}

//-----------------------------
bool MainWindow::initMenuBar()
{
    QMenuBar* mb = menuBar();
    bool ret = (mb != nullptr);

    ret = ret && initFileMenu(mb);
    ret = ret && initEditMenu(mb);
    ret = ret && initFormatMenu(mb);
    ret = ret && initViewMenu(mb);
    ret = ret && initHelpMenu(mb);

    return ret;
}

bool MainWindow::initFileMenu(QMenuBar* mb)
{
    QMenu* menu = new QMenu("文件(&F)", mb);
    bool ret = (menu != nullptr);

    if( ret )
    {
        QAction* action = nullptr;

        ret = ret && makeAction(action, menu, "新建(&N)", Qt::CTRL + Qt::Key_N);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onFileNew()));
            menu->addAction(action);
        }

        ret = ret && makeAction(action, menu, "打开(&O)...", Qt::CTRL + Qt::Key_O);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onFileOpen()));
            menu->addAction(action);
        }

        ret = ret && makeAction(action, menu,  "保存(&S)", Qt::CTRL + Qt::Key_S);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onFileSave()));
            menu->addAction(action);
        }

        ret = ret && makeAction(action, menu,  "另存为(&A)...", 0);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onFileSaveAs()));
            menu->addAction(action);
        }

        menu->addSeparator();

        ret = ret && makeAction(action, menu,  "页面设置(&U)...", Qt::CTRL + Qt::Key_U);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onFilePageSetup()));
            menu->addAction(action);
        }

        ret = ret && makeAction(action, menu,  "打印(&P)...", Qt::CTRL + Qt::Key_P);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onFilePrint()));
            menu->addAction(action);
        }

        menu->addSeparator();

        ret = ret && makeAction(action, menu,  "退出(&X)", 0);
        if( ret )
        {
            menu->addAction(action);
        }
    }

    if( ret )
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }

    return ret;
}

bool MainWindow::initEditMenu(QMenuBar* mb)
{
    QMenu* menu = new QMenu("编辑(&E)", mb);
    bool ret = (menu != nullptr);

    if( ret )
    {
        QAction* action = nullptr;

        ret = ret && makeAction(action, menu,  "撤销(&U)", Qt::CTRL + Qt::Key_Z);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), &mainEditor, SLOT(undo()));
            action->setEnabled(false);
            menu->addAction(action);
        }

        ret = ret && makeAction(action, menu,  "剪切(&T)", Qt::CTRL + Qt::Key_X);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), &mainEditor, SLOT(redo()));
            action->setEnabled(false);
            menu->addAction(action);
        }

        ret = ret && makeAction(action, menu,  "复制(&C)", Qt::CTRL + Qt::Key_C);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), &mainEditor, SLOT(copy()));
            action->setEnabled(false);
            menu->addAction(action);
        }

        ret = ret && makeAction(action, menu,  "粘贴(&P)", Qt::CTRL + Qt::Key_V);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), &mainEditor, SLOT(paste()));
            menu->addAction(action);
        }

        ret = ret && makeAction(action, menu,  "删除(&L)....", Qt::Key_Delete);
        if( ret )
        { 
            connect(action, SIGNAL(triggered()), this, SLOT(onEditDelete()));
            menu->addAction(action);
        }

        menu->addSeparator();

        ret = ret && makeAction(action, menu,  "查找(&F)...", Qt::CTRL + Qt::Key_F);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onEditFind()));
            menu->addAction(action);
        }

        ret = ret && makeAction(action, menu,  "查找下一个(&N)", Qt::Key_F3);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onEditFindNext()));
            menu->addAction(action);
        }

        ret = ret && makeAction(action, menu,  "替换(&R)...",  Qt::CTRL + Qt::Key_H);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onEditReplace()));
            menu->addAction(action);
        }

        ret = ret && makeAction(action, menu,  "转到(&G)...",  Qt::CTRL + Qt::Key_G);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onEditGoto()));
            menu->addAction(action);
        }

        menu->addSeparator();

        ret = ret && makeAction(action, menu,  "全选(&A)",  Qt::CTRL + Qt::Key_A);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onEditSelectAll()));
            menu->addAction(action);
        }

        ret = ret && makeAction(action, menu,  "时间/日期(&D)", Qt::Key_F5);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onEditDate()));
            menu->addAction(action);
        }

        menu->addSeparator();

        ret = ret && makeAction(action, menu, "计算器...", 0);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onEditCalculator()));
            menu->addAction(action);
        }

        ret = ret && makeAction(action, menu, "画图...", 0);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onEditmspaint()));
            menu->addAction(action);
        }
    }

    if( ret )
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }

    return ret;
}

bool MainWindow::initFormatMenu(QMenuBar* mb)
{
    QMenu* menu = new QMenu("格式(&O)", mb);
    bool ret = (menu != nullptr);

    if( ret )
    {
        QAction* action = nullptr;

        ret = ret && makeAction(action, menu,  "自动换行(&W)", 0);
        if( ret )
        {
            action->setCheckable(true);
            action->setChecked(true);
            connect(action, SIGNAL(triggered()), this, SLOT(FormatWrap()));
            menu->addAction(action);
        }

        ret = ret && makeAction(action, menu,  "字体(&F)...", 0);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(FormatFont()));
            menu->addAction(action);
        }
    }

    if( ret )
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }

    return ret;
}

bool MainWindow::initViewMenu(QMenuBar* mb)
{
    QMenu* menu = new QMenu("查看(&V)", mb);
    bool ret = (menu != nullptr);

    if( ret )
    {
        QAction* action = nullptr;

        ret = ret && makeAction(action, menu,  "工具栏(&T)", 0);
        if( ret )
        {
            action->setCheckable(true);
            action->setChecked(true);
            connect(action, SIGNAL(triggered()), this, SLOT(onViewToolBar()));

            menu->addAction(action);
        }

        ret = ret && makeAction(action, menu,  "状态栏(&S)", 0);
        if( ret )
        {
            action->setCheckable(true);
            action->setChecked(true);
            connect(action, SIGNAL(triggered()), this, SLOT(onViewStatusBar()));

            menu->addAction(action);
        }
    }

    if( ret )
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }

    return ret;
}

bool MainWindow::initHelpMenu(QMenuBar* mb)
{
    QMenu* menu = new QMenu("帮助(&H)", mb);
    bool ret = (menu != nullptr);

    if( ret )
    {
        QAction* action = nullptr;

        ret = ret && makeAction(action, menu,  "查看帮助(&H)", 0);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onHelpManual()));
            menu->addAction(action);
        }

        ret = ret && makeAction(action, menu,  "关于记事本(&A)", 0);
        if( ret )
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onHelpAbort()));
            menu->addAction(action);
        }
    }

    if( ret )
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }

    return ret;
}

bool MainWindow::makeAction(QAction*& action, QWidget* parent, QString text, int key)
{
    bool ret = true;

    action = new QAction(text, parent);

    if( action != nullptr )
    {
        action->setShortcut(QKeySequence(key));
    }
    else
    {
        ret = false;
    }

    return ret;
}

//------------------------------
bool MainWindow::initToolBar()
{
    QToolBar* tb = addToolBar("工具栏");
    bool ret = true;

    tb->setIconSize(QSize(16, 16));
    tb->setFloatable(false);
    tb->setMovable(false);

    ret = ret && initFileToolItem(tb);

    tb->addSeparator();

    ret = ret && initEditToolItem(tb);

    tb->addSeparator();

    ret = ret && initFormatToolItem(tb);

    tb->addSeparator();

    ret = ret && initViewToolItem(tb);

    return ret;
}

bool MainWindow::initFileToolItem(QToolBar* tb)
{
    QAction* action = nullptr;
    bool ret = true;

    ret = ret && makeAction(action, tb, "新建", ":/Res/pic/new.png");
    if( ret )
    {
        connect(action, SIGNAL(triggered()), this, SLOT(onFileNew()));
        tb->addAction(action);
    }

    ret = ret && makeAction(action, tb, "打开", ":/Res/pic/open.png");
    if( ret )
    {
        connect(action, SIGNAL(triggered()), this, SLOT(onFileOpen()));
        tb->addAction(action);
    }

    ret = ret && makeAction(action, tb, "保存", ":/Res/pic/save.png");
    if( ret )
    {
        connect(action, SIGNAL(triggered()), this, SLOT(onFileSave()));
        tb->addAction(action);
    }

    ret = ret && makeAction(action, tb, "另存为", ":/Res/pic/saveas.png");
    if( ret )
    {
        connect(action, SIGNAL(triggered()), this, SLOT(onFileSaveAs()));
        tb->addAction(action);
    }

    ret = ret && makeAction(action, tb, "打印", ":/Res/pic/print.png");
    if( ret )
    {
        connect(action, SIGNAL(triggered()), this, SLOT(onFilePrint()));
        tb->addAction(action);
    }

    return ret;
}

bool MainWindow::initEditToolItem(QToolBar* tb)
{
    QAction* action = nullptr;
    bool ret = true;

    ret = ret && makeAction(action, tb, "撤销", ":/Res/pic/undo.png");
    if( ret )
    {
        connect(action, SIGNAL(triggered()), &mainEditor, SLOT(undo()));
        action->setEnabled(false);
        tb->addAction(action);
    }

    ret = ret && makeAction(action, tb, "恢复", ":/Res/pic/redo.png");
    if( ret )
    {
        connect(action, SIGNAL(triggered()), &mainEditor, SLOT(redo()));
        action->setEnabled(false);
        tb->addAction(action);
    }

    ret = ret && makeAction(action, tb, "剪切", ":/Res/pic/cut.png");
    if( ret )
    {
        connect(action, SIGNAL(triggered()), &mainEditor, SLOT(cut()));
        action->setEnabled(false);
        tb->addAction(action);
    }

    ret = ret && makeAction(action, tb, "复制", ":/Res/pic/paste.png");
    if( ret )
    {
        connect(action, SIGNAL(triggered()), &mainEditor, SLOT(copy()));
        action->setEnabled(false);
        tb->addAction(action);
    }

    ret = ret && makeAction(action, tb, "粘贴", ":/Res/pic/paste.png");
    if( ret )
    {
        connect(action, SIGNAL(triggered()), &mainEditor, SLOT(paste()));
        tb->addAction(action);
    }

    ret = ret && makeAction(action, tb, "查找", ":/Res/pic/find.png");
    if( ret )
    {
        connect(action, SIGNAL(triggered()), this, SLOT(onEditFind()));
        tb->addAction(action);
    }

    ret = ret && makeAction(action, tb, "替换", ":/Res/pic/replace.png");
    if( ret )
    {
        connect(action, SIGNAL(triggered()), this, SLOT(onEditReplace()));
        tb->addAction(action);
    }

    ret = ret && makeAction(action, tb, "转到", ":/Res/pic/goto.png");
    if( ret )
    {
        connect(action, SIGNAL(triggered()), this, SLOT(onEditGoto()));
        tb->addAction(action);
    }

    return ret;
}

bool MainWindow::initFormatToolItem(QToolBar* tb)
{
    QAction* action = nullptr;
    bool ret = true;

    ret = ret && makeAction(action, tb, "自动换行", ":/Res/pic/wrap.png");
    if( ret )
    {
        action->setCheckable(true);
        action->setChecked(true);
        connect(action, SIGNAL(triggered()), this, SLOT(FormatWrap()));
        tb->addAction(action);
    }

    ret = ret && makeAction(action,  tb,"字体", ":/Res/pic/font.png");
    if( ret )
    {
        connect(action, SIGNAL(triggered()), this, SLOT(FormatFont()));
        tb->addAction(action);
    }

    return ret;
}

bool MainWindow::initViewToolItem(QToolBar* tb)
{
    QAction* action = nullptr;
    bool ret = true;

    ret = ret && makeAction(action,  tb,"工具栏", ":/Res/pic/tool.png");
    if( ret )
    {
        action->setCheckable(true);
        action->setChecked(true);
        connect(action, SIGNAL(triggered()), this, SLOT(onViewToolBar()));
        tb->addAction(action);
    }

    ret = ret && makeAction(action,  tb,"状态栏", ":/Res/pic/status.png");
    if( ret )
    {
        action->setCheckable(true);
        action->setChecked(true);
        connect(action, SIGNAL(triggered()), this, SLOT(onViewStatusBar()));
        tb->addAction(action);
    }

    return ret;
}

bool MainWindow::makeAction(QAction*& action, QWidget* parent, QString tip, QString icon)
{
   bool ret = true;

   action = new QAction("", parent);

   if( action != nullptr )
   {
       action->setToolTip(tip);
       action->setIcon(QIcon(icon));
   }
   else
   {
       ret = false;
   }

   return ret;
}

//-----------------------------------
bool MainWindow::initStatusBar()
{
    QStatusBar* sb = statusBar();
    QLabel* label = new QLabel("D.T.TianSong");
    bool ret = true;

    if( label != nullptr )
    {
        sb->addPermanentWidget(new QLabel());

        statusLabel.setMinimumWidth(150);
        statusLabel.setAlignment(Qt::AlignCenter);
        statusLabel.setText("length: " + QString::number(0) + "    lines: " + QString::number(1));
        sb->addPermanentWidget(&statusLabel);

        statusCursorLabel.setMinimumWidth(150);
        statusCursorLabel.setAlignment(Qt::AlignCenter);
        statusCursorLabel.setText("Ln: " + QString::number(1) + "    Col: " + QString::number(1));
        sb->addPermanentWidget(&statusCursorLabel);

        label->setMinimumWidth(150);
        label->setAlignment(Qt::AlignCenter);
        sb->addPermanentWidget(label);
    }
    else
    {
        ret = false;
    }

    return ret;
}

//---------------------------------
bool MainWindow::initMainEditor()
{
    bool ret = true;

    QPalette p = mainEditor.palette();
    p.setColor(QPalette::Inactive, QPalette::Highlight, p.color(QPalette::Active, QPalette::Highlight));
    p.setColor(QPalette::Inactive, QPalette::HighlightedText, p.color(QPalette::Active, QPalette::HighlightedText));
    mainEditor.setPalette(p);

    mainEditor.setParent(this);
    mainEditor.setAcceptDrops(false);
    setCentralWidget(&mainEditor);

    connect(&mainEditor, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
    connect(&mainEditor, SIGNAL(copyAvailable(bool)), this, SLOT(onCopyAvailable(bool)));
    connect(&mainEditor, SIGNAL(redoAvailable(bool)), this, SLOT(onRedoAvailable(bool)));
    connect(&mainEditor, SIGNAL(undoAvailable(bool)), this, SLOT(onUndoAvailable(bool)));
    connect(&mainEditor, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorPositionChanged()));

    return ret;
}

QToolBar* MainWindow::toolBar()
{
    QToolBar* ret = nullptr;

    const QObjectList& list = children();

    for(int i=0; i<list.count(); i++)
    {
        QToolBar* toolBar = dynamic_cast<QToolBar*>(list[i]);

        if( toolBar != nullptr )
        {
            ret =toolBar;
            break;
        }
    }

    return ret;
}

MainWindow::~MainWindow()
{
}
