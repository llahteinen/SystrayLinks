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

