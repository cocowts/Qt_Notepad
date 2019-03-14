#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QPointer>
#include <QPlainTextEdit>

class FindDialog : public QDialog
{
    Q_OBJECT

protected:
    QGroupBox m_radioGrpBx;

    QGridLayout m_layout;
    QHBoxLayout m_hbLayout;

    QLabel m_findLbl;
    QLineEdit m_findEdit;
    QPushButton m_findBtn;
    QPushButton m_cancelBtn;
    QCheckBox m_matchChkBx;
    QRadioButton m_upwardBtn;
    QRadioButton m_downwardBtn;

    QPointer<QPlainTextEdit> m_pText;

    void initControl();
    void connectSlot();

public slots:
    void onFindClicked();
    void onCancelClicked();

public:
    FindDialog(QWidget* parent = nullptr, QPlainTextEdit* pText = nullptr);
    void setPlainTextEdit(QPlainTextEdit* pText);
    QPlainTextEdit* getPlainTextEdit();
    bool event(QEvent* e);
    ~FindDialog();
};

#endif // FINDDIALOG_H
