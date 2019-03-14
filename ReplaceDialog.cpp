#include "ReplaceDialog.h"

ReplaceDialog::ReplaceDialog(QWidget* parent, QPlainTextEdit* pText) : FindDialog (parent, pText)
{
    initControl();
    connectSlot();
}

void ReplaceDialog::initControl()
{
    m_replaceLbl.setText("替换为:");
    m_replaceBtn.setText("替换(&R)");
    m_replaceAllBtn.setText("全部替换(&A)");

    m_layout.removeWidget(&m_matchChkBx);
    m_layout.removeWidget(&m_radioGrpBx);
    m_layout.removeWidget(&m_cancelBtn);

    m_layout.addWidget(&m_replaceLbl, 1, 0);
    m_layout.addWidget(&m_replaceEdit, 1, 1);
    m_layout.addWidget(&m_replaceBtn, 1, 2);

    m_layout.addWidget(&m_matchChkBx, 2, 0);
    m_layout.addWidget(&m_radioGrpBx, 2, 1);
    m_layout.addWidget(&m_replaceAllBtn, 2, 2);

    m_layout.addWidget(&m_cancelBtn, 3, 2);

    setFixedSize(450, 170);
    setWindowTitle("替换");
}

void ReplaceDialog::connectSlot()
{
    connect(&m_replaceBtn, SIGNAL(clicked()), this, SLOT(onReplaceClicked()));
    connect(&m_replaceAllBtn, SIGNAL(clicked()), this, SLOT(onReplaceAllClicked()));
}

void ReplaceDialog::onReplaceClicked()
{
    QString target = m_findEdit.text();
    QString to = m_replaceEdit.text();

    if( (m_pText != nullptr) && (target != "") && (to != "") )
    {
        QString selText = m_pText->textCursor().selectedText();

        if( selText == target )
        {
            m_pText->insertPlainText(to);
        }

        onFindClicked();
    }
}

void ReplaceDialog::onReplaceAllClicked()
{
    QString target = m_findEdit.text();
    QString to = m_replaceEdit.text();

    if( (m_pText != nullptr) && (target != "") && (to != "") )
    {
        QString text = m_pText->toPlainText();

        text.replace(target, to, m_matchChkBx.isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

        m_pText->clear();

        m_pText->insertPlainText(text);
    }
}
