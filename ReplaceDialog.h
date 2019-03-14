#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include "FindDialog.h"

class ReplaceDialog : public FindDialog
{
    Q_OBJECT
protected:
    QLabel m_replaceLbl;
    QLineEdit m_replaceEdit;
    QPushButton m_replaceBtn;
    QPushButton m_replaceAllBtn;

    void initControl();
    void connectSlot();

protected slots:
    void onReplaceClicked();
    void onReplaceAllClicked();

public:
    ReplaceDialog(QWidget* parent = nullptr, QPlainTextEdit* pText = nullptr);
};

#endif // REPLACEDIALOG_H
