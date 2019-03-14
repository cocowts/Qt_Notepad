#include "AppConfig.h"
#include <QFile>
#include <QDataStream>
#include <QApplication>

AppConfig::AppConfig(QObject *parent) : QObject(parent)
{
    m_isVilid = restore();
}

AppConfig::AppConfig(QFont font, QSize size, QPoint point, bool isWrap, bool tbvisible, bool sbVisible, QObject *parent) : QObject(parent)
{
    m_editFont = font;
    m_mainWindowSize = size;
    m_mainWindowPoint = point;
    m_isAutoWrap = isWrap;
    m_isToolBarVisible = tbvisible;
    m_isStatusVisible = sbVisible;

    m_isVilid = true;
}

bool AppConfig::restore()
{
    bool ret = true;
    QFile file(QApplication::applicationDirPath() + "/app.config");

    if( file.open(QIODevice::ReadOnly) )
    {
        QDataStream in(&file);

        in >> m_editFont;
        in >> m_mainWindowSize;
        in >> m_mainWindowPoint;
        in >> m_isAutoWrap;
        in >> m_isToolBarVisible;
        in >> m_isStatusVisible;

        file.close();
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool AppConfig::store()
{
    bool ret = true;
    QFile file(QApplication::applicationDirPath() + "/app.config");

    if( file.open(QIODevice::WriteOnly) )
    {
        QDataStream out(&file);

        out << m_editFont;
        out << m_mainWindowSize;
        out << m_mainWindowPoint;
        out << m_isAutoWrap;
        out << m_isToolBarVisible;
        out << m_isStatusVisible;

        file.close();
    }
    else
    {
        ret = false;
    }

    return ret;
}

QFont AppConfig::editFont()
{
    return m_editFont;
}

QSize AppConfig::mainWindowSize()
{
    return m_mainWindowSize;
}

QPoint AppConfig::mainWindowPoint()
{
    return m_mainWindowPoint;
}

bool AppConfig::isAutoWrap()
{
    return m_isAutoWrap;
}

bool AppConfig::isToolBarVisible()
{
    return m_isToolBarVisible;
}

bool AppConfig::isStatusVisible()
{
    return m_isStatusVisible;
}

bool AppConfig::isVilid()
{
    return m_isVilid;
}
