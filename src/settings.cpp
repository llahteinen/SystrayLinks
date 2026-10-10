#include "settings.hpp"
#include <QSettings>
#include <QCoreApplication>


Settings::Settings(QObject* parent)
    : QObject{parent}
    , appFont{}
    , m_pers{}
{
    appFont.setFamily("Segoe UI Variable Text");
//    appFont.setVariableAxis("wght", 600); /// Use variable font such as Segoe UI Variable Text *Semibold*
    appFont.setPixelSize(14);
}

void Settings::setAppFont(const QFont& font)
{
    appFont = font;
}

QFont Settings::getAppFont() const
{
    return appFont;
}

void Settings::persistToStore() const
{
    QSettings settings;
    settings.setValue("settingsVersion",            SETTINGS_VERSION);
    settings.setValue("applicationVersion",         QCoreApplication::applicationVersion());
    settings.setValue("Settings/linkPath",          m_pers.linkPath);
    settings.setValue("Settings/iconGlyph",         m_pers.iconGlyph.unicode()); /// Store char16_t
    settings.setValue("Settings/itemLimit",         m_pers.itemLimit);
    settings.setValue("Settings/sortCondition",     m_pers.sortCondition);
    settings.setValue("Settings/sortDirection",     static_cast<int>(m_pers.sortDirection));
    settings.setValue("Settings/menuFontFamily",    m_pers.menuFont.family());
    settings.setValue("Settings/menuFontPixelSize", m_pers.menuFont.pixelSize());
}

void Settings::loadFromStore()
{
    const QSettings settings;
    m_pers.linkPath = settings.value("Settings/linkPath", m_pers.linkPath).toString();
    const uint glyph = settings.value("Settings/iconGlyph", m_pers.iconGlyph.unicode()).toUInt(); /// Stored char16_t
    m_pers.iconGlyph = QChar(glyph);
    m_pers.itemLimit = settings.value("Settings/itemLimit", m_pers.itemLimit).toInt();
    m_pers.sortCondition = settings.value("Settings/sortCondition", m_pers.sortCondition).toInt();
    m_pers.sortDirection = static_cast<Qt::SortOrder>(settings.value("Settings/sortDirection", static_cast<int>(m_pers.sortDirection)).toInt());

    const QString fontFamily = settings.value("Settings/menuFontFamily", m_pers.menuFont.family()).toString();
    const int fontPixelSize = settings.value("Settings/menuFontPixelSize", m_pers.menuFont.pixelSize()).toInt();
    if(!fontFamily.isEmpty())
    {
        m_pers.menuFont.setFamily(fontFamily);
    }
    if(fontPixelSize > 0)
    {
        m_pers.menuFont.setPixelSize(fontPixelSize);
    }
    emitAllSettingsChanged();
}

void Settings::emitAllSettingsChanged()
{
    emit linkPathChanged(m_pers.linkPath);
    emit iconChanged(m_pers.iconGlyph);
    emit itemLimitChanged(m_pers.itemLimit);
    emit sortConditionChanged(m_pers.sortCondition);
    emit sortDirectionChanged(m_pers.sortDirection);
    emit menuFontChanged(m_pers.menuFont);
}

void Settings::setLinkPath(const QString& path)
{
    if(m_pers.linkPath != path)
    {
        m_pers.linkPath = path;
        emit linkPathChanged(m_pers.linkPath);
    }
}

const QString& Settings::getLinkPath() const
{
    return m_pers.linkPath;
}

void Settings::setIcon(QChar glyph)
{
    m_pers.iconGlyph = glyph;
    emit iconChanged(m_pers.iconGlyph);
}

QChar Settings::getIcon() const
{
    return m_pers.iconGlyph;
}

void Settings::setItemLimit(int newItemLimit)
{
    m_pers.itemLimit = newItemLimit;
    emit itemLimitChanged(m_pers.itemLimit);
}

int Settings::getItemLimit() const
{
    return m_pers.itemLimit;
}

void Settings::setSortCondition(int newSortCondition)
{
    m_pers.sortCondition = newSortCondition;
    emit sortConditionChanged(m_pers.sortCondition);
}

int Settings::getSortCondition() const
{
    return m_pers.sortCondition;
}

void Settings::setSortDirection(Qt::SortOrder newSortDirection)
{
    m_pers.sortDirection = newSortDirection;
    emit sortDirectionChanged(m_pers.sortDirection);
}

Qt::SortOrder Settings::getSortDirection() const
{
    return m_pers.sortDirection;
}

void Settings::setMenuFont(const QFont& font)
{
    m_pers.menuFont = font;
    emit menuFontChanged(m_pers.menuFont);
}

void Settings::setMenuFontPxSize(int size)
{
    m_pers.menuFont.setPixelSize(size);
    emit menuFontChanged(m_pers.menuFont);
}

QFont Settings::getMenuFont() const
{
    return m_pers.menuFont;
}

