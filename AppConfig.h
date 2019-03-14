#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QObject>
#include <QFont>
#include <QPoint>
#include <QSize>

class AppConfig : public QObject
{

protected:
    QFont m_editFont;
    QSize m_mainWindowSize;
    QPoint m_mainWindowPoint;
    bool m_isAutoWrap;
    bool m_isToolBarVisible;
    bool m_isStatusVisible;
    bool m_isVilid;

    bool restore();

public:
    explicit AppConfig(QObject *parent = nullptr);
    explicit AppConfig(QFont font, QSize size, QPoint point, bool isWrap, bool tbvisible, bool sbVisible, QObject *parent = nullptr);
    bool store();
    QFont editFont();
    QSize mainWindowSize();
    QPoint mainWindowPoint();
    bool isAutoWrap();
    bool isToolBarVisible();
    bool isStatusVisible();
    bool isVilid();
};

#endif // APPCONFIG_H
