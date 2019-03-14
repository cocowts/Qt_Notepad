#include "FindDialog.h"
#include <QEvent>
#include <QTextCursor>
#include <QMessageBox>

FindDialog::FindDialog(QWidget* parent, QPlainTextEdit* pText) : QDialog (parent, Qt::WindowCloseButtonHint | Qt::Drawer)
{
    initControl();
    connectSlot();

    setLayout(&m_layout);
    setFixedSize(450, 120);
    setWindowTitle("查找");

    setPlainTextEdit(pText);
}

void FindDialog::initControl()
{
    m_findLbl.setText("查找目标:");
    m_findBtn.setText("查找下一个(&F)");
    m_cancelBtn.setText("取消");
    m_matchChkBx.setText("区分大小写(&C)");
    m_radioGrpBx.setTitle("方向");
    m_upwardBtn.setText("向上(&U)");
    m_downwardBtn.setText("向下(&D)");
    m_downwardBtn.setChecked(true);

    m_hbLayout.addWidget(&m_upwardBtn);
    m_hbLayout.addWidget(&m_downwardBtn);
    m_radioGrpBx.setLayout(&m_hbLayout);

    m_layout.addWidget(&m_findLbl, 0, 0);
    m_layout.addWidget(&m_findEdit, 0, 1);
    m_layout.addWidget(&m_findBtn, 0, 2);

    m_layout.addWidget(&m_matchChkBx, 1, 0);
    m_layout.addWidget(&m_radioGrpBx, 1, 1);
    m_layout.addWidget(&m_cancelBtn, 1, 2);
}

void FindDialog::connectSlot()
{
    connect(&m_findBtn, SIGNAL(clicked()), this, SLOT(onFindClicked()));
    connect(&m_cancelBtn, SIGNAL(clicked()), this, SLOT(onCancelClicked()));
}

void FindDialog::setPlainTextEdit(QPlainTextEdit* pText)
{
    m_pText = pText;
}

QPlainTextEdit* FindDialog::getPlainTextEdit()
{
    return m_pText;
}

void FindDialog::onFindClicked()
{
    QString target = m_findEdit.text();

    if( (m_pText != nullptr) && (target != "") )
    {
        QString text = m_pText->toPlainText();
        QTextCursor c = m_pText->textCursor();
        int index = -1;

        if( m_downwardBtn.isChecked() )
        {
            index = text.indexOf(target, c.position(), m_matchChkBx.isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

            if( index >= 0 )
            {
                c.setPosition(index);
                c.setPosition(index + target.length(), QTextCursor::KeepAnchor);

                m_pText->setTextCursor(c);
            }
        }

        if( m_upwardBtn.isChecked() )
        {
            index = text.lastIndexOf(target, c.position() - text.length() - 1, m_matchChkBx.isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

            if( index >=0 )
            {
                c.setPosition(index + target.length());
                c.setPosition(index, QTextCursor::KeepAnchor);

                m_pText->setTextCursor(c);
            }
        }

        if( index < 0 )
        {
            QMessageBox msg(this);

            msg.setWindowTitle("记事本");
            msg.setText(QString("找不到 ") + "\"" + target + "\"");
            msg.setWindowFlag(Qt::Drawer);
            msg.setIcon(QMessageBox::Information);
            msg.setStandardButtons(QMessageBox::Ok);

            msg.exec();
        }
    }
}

void FindDialog::onCancelClicked()
{
    close();
}

bool FindDialog::event(QEvent* e)
{
    if( e->type() == QEvent::Close )
    {
        hide();

        return true;
    }

    return QDialog::event(e);
}

FindDialog::~FindDialog()
{

}
