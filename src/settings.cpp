#include "settings.hpp"
#include <QSettings>


Settings::Settings(QObject* parent)
    : QObject{parent}
    , m_pers{}
{}

void Settings::persistToStore()
{
    QSettings settings;
    settings.setValue("settingsVersion",            SETTINGS_VERSION);
    settings.setValue("Settings/linkPath",          m_pers.linkPath);
}

void Settings::loadFromStore()
{
    QSettings settings;
    m_pers.linkPath = settings.value("Settings/linkPath", m_pers.linkPath).toString();
}

void Settings::setLinkPath(const QString& path)
{
    m_pers.linkPath = path;
}

const QString& Settings::getLinkPath() const
{
    return m_pers.linkPath;
}

void Settings::setItemLimit(int newItemLimit)
{
    m_pers.itemLimit = newItemLimit;
}

int Settings::getItemLimit() const
{
    return m_pers.itemLimit;
}

void Settings::setSortCondition(int newSortCondition)
{
    m_pers.sortCondition = newSortCondition;
}

int Settings::getSortCondition() const
{
    return m_pers.sortCondition;
}

void Settings::setSortDirection(Qt::SortOrder newSortDirection)
{
    m_pers.sortDirection = newSortDirection;
}

Qt::SortOrder Settings::getSortDirection() const
{
    return m_pers.sortDirection;
}

void Settings::setFont(const QFont& font)
{
    m_pers.menuFont = font;
}

QFont Settings::getFont() const
{
    return m_pers.menuFont;
}

