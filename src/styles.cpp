#include "styles.hpp"
#include <QString>
#include <QIcon>
#include <QDebug>
#include <array>


namespace
{
constexpr auto styleSheetLinksMenu = R"(
QMenu {
    border-radius: 1px;
    padding: 3px; /* QMenu::item margin will affect whole menu padding look as well */
/* Background should be 249, 249, 249 but is slightly darker by default */
/*    background-color: rgb(249, 249, 249);*/
/*    background: palette(window);*/ /* Should follow dark mode, but just need to re-set the stylesheet */
/* Set font programmatically */
/*    font-family: "Segoe UI Variable Text";*/ /**/
/*    font-family: "Segoe UI Variable Display";*/ /**/
/*    font-family: "Segoe UI Variable Small Semibold";*/ /**/
/*    font-size: 10pt;*/        /* 10.5pt looks most like Notepad */
/*    color: rgb(26, 26, 26);*/ /* Can be used here even if font is set in cpp */
    color: palette(text);   /* For some reason this gives the correct (lighter than black) color while the default without this is black */
/*    icon-size: 24px;*/        /* Works fine */
}
QMenu::icon {
/*    width: 36px;*/        /* Doesn't have any effect */
    padding: 12px;          /* left padding is ok with this, but the text that comes after goes weird, it starts from the center of the icon */
/*    padding-left: 12px;*/ /* left padding does not work correctly with this */
/*    padding-right breaks the text that comes after */
/* seems that padding-left etc don't work properly at all */
}
QMenu::item {
    padding-left: 20px;     /* Padding starting from the CENTER of the icon or indicator/checkmark, or from left of menu if the whole menu has no icons or indicators */
    padding-right: 16px;    /* Padding right of the shortcut key */
    padding-top: 4px;
    padding-bottom: 4px;
    min-height: 20px;       /* Maybe this works with variable font size? Buut paddign affects it??? */
/*    min-width: 100px;*/       /* Can be used to make more space between text and shortcut. Seems to affect the text portion itself, the shortcut and icon sections excluded */
    border-radius: 4px;     /* Works fine */
    margin: 2px;            /* Vertical spacing between items is correct with 2px here. Border looks near enough if QMenu padding is 2px with it */
}
QMenu::item:selected {    /* Hover effect */
    /* Darker background on hover */
    background-color: rgba(0, 0, 0, 12);
/*    background-color: rgba(255, 255, 255, 16);*/ /* For dark mode */
}
)";

constexpr auto styleSheetWindow = R"(
QToolTip {
/* Can't seem to get different horizontal and vertical padding. Compromise. */
    padding: 6px;
    background-color: rgb(249, 249, 249);   /* LayerFillColorDefaultBrush */
/*    border: 1px solid rgb(229, 0, 229);*/     /* No effect. ControlElevationBorderBrush */
/*    border: 0px;*/    /* No effect either */
}
QLabel[role="subtitle"] {
    margin-top: 8px;
    margin-bottom: 6px; /* plus layout spacing == 8 */
}
QFrame[role="card"] {
    background: rgb(251, 251, 251);         /* CardBackgroundFillColorDefaultBrush */
    border: 1px solid rgb(229, 229, 229);   /* ControlElevationBorderBrush */
    border-radius: 4px;
    margin-top: 0px;
    margin-bottom: 0px;     /* plus layout spacing == 3px */
    margin-left: 2px;       /* With this, it's same level as buttons */
    margin-right: 2px;
    padding: 16px;
    padding-right: 12px;
}
/* Works for the dropdown menu itself, but it seems to be quite OK by default already */
/* This can't seem to be able to be adjusted in proxy style. Maybe polish QComboBox QListView setViewportMargins, but it's protected method */
QComboBox QAbstractItemView {
    padding: 4px;
}
)";

/// These are glyph codes in the Fluent UI / WinUI 3 icon set, Segoe Fluent Icons
constexpr std::array iconKeys =
{
    u'\uEB52', /// Default = HeartFill
    u'\uEB51', /// Heart
    u'\uEB52', /// HeartFill
    u'\uE734', /// FavoriteStar
    u'\uE735', /// FavoriteStarFill
    u'\uE728', /// FavoriteList
    u'\uE840', /// Pinned
    u'\uE723', /// Attach
    u'\uE8B7', /// Folder
    u'\uE838', /// FolderOpen
    u'\uEC6C', /// Favicon2
    u'\uE8F1', /// Library
    u'\uE896', /// Download
    u'\uEBD3', /// CloudDownload
    u'\uF0E2', /// GridView
    u'\uE71D', /// AllApps
    u'\uEA37', /// List
    u'\uE9A4', /// TextBulletListSquare
    u'\uE9A3', /// TextBulletListSquareSparkle
    u'\uF6B8', /// ExpressiveInputEntry
    u'\uEA38', /// Asterisk
    u'\uEDAD', /// AsteriskBadge12
    u'\uEDD7', /// CaretUp8
    u'\uEDDB', /// CaretUpSolid8
    u'\uF090', /// CaretSolidUp
};
/* Too smol but otherwise OK icons
u'\uE71B', /// Link
u'\uE753', /// Cloud
u'\uE70D', /// ChevronDown
u'\uE70E', /// ChevronUp
u'\uE96D', /// ChevronUpSmall
u'\uE96E', /// ChevronDownSmall
u'\uE971', /// ChevronUpMed
*/
static_assert(static_cast<std::size_t>(Styles::Icons::_END) == iconKeys.size());
}

QChar Styles::fluentIconGlyph(Icons index)
{
    Q_ASSERT(static_cast<std::size_t>(index) < iconKeys.size());
    if(index < Icons::_END)
    {
        return iconKeys[static_cast<std::size_t>(index)];
    }
    return iconKeys[static_cast<std::size_t>(Icons::Default)];
}

Styles::Icons Styles::fluentIconEnum(QChar glyph)
{
    for(std::size_t i = std::size_t(Icons::Default) + 1; i < iconKeys.size(); ++i) /// Skip default, it is duplicate of some other icon
    {
        if(iconKeys[i] == glyph.unicode())
        {
            return static_cast<Icons>(i);
        }
    }
    return Icons::Default; /// fallback
}

QIcon Styles::fluentIcon(QChar glyph)
{
    return QIcon::fromTheme(glyph);
}

QIcon Styles::fluentIcon(QChar glyph, const QIcon& fallback)
{
    return QIcon::fromTheme(glyph, fallback);
}

QIcon Styles::fluentIcon(Icons name, const QIcon& fallback)
{
    const size_t index = static_cast<size_t>(name);
    const auto fluent_key = index < iconKeys.size() ? iconKeys[index] : iconKeys[0];

//    bool isWindows11orAbove = true;
//    const QString assetFontName = isWindows11orAbove ? QStringLiteral("Segoe Fluent Icons") : QStringLiteral("Segoe MDL2 Assets");
//    QIcon::setThemeName(assetFontName);
    return QIcon::fromTheme(QChar(fluent_key), fallback);
}

QList<QIcon> Styles::fluentIconsList()
{
    QList<QIcon> list;
    for(const auto& key : iconKeys)
    {
        const auto icon = fluentIcon(QChar(key));
        list.append(icon);
    }
    return list;
}

const QString& Styles::styleSheetLinksMenu()
{
    static const QString style_str{::styleSheetLinksMenu};
    return style_str;
}

QString Styles::styleSheetWindow()
{
    return {::styleSheetWindow};
}

