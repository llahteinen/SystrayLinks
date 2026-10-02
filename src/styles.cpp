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
QComboBox QAbstractItemView {
    padding: 4px;
}
QComboBox QAbstractItemView::item {
    padding: 2px 8px; /* V H gives 33px total height */
}
)";

/// These are glyph codes in the Fluent UI / WinUI 3 icon set, Segoe Fluent Icons
constexpr std::array iconKeys =
{
    "\uEB52", /// Default = HeartFill
    "\uEB51", /// Heart
    "\uEB52", /// HeartFill
    "\uE728", /// FavoriteList
    "\uE734", /// FavoriteStar
    "\uE735", /// FavoriteStarFill
};
static_assert(static_cast<std::size_t>(Styles::Icons::_END) == iconKeys.size());
}

QIcon Styles::fluentIcon(Icons name, const QIcon& fallback)
{
    const size_t index = static_cast<size_t>(name);
    const auto fluent_key = index < iconKeys.size() ? iconKeys[index] : iconKeys[0];

//    bool isWindows11orAbove = true;
//    const QString assetFontName = isWindows11orAbove ? QStringLiteral("Segoe Fluent Icons") : QStringLiteral("Segoe MDL2 Assets");
//    QIcon::setThemeName(assetFontName);
    return QIcon::fromTheme(fluent_key, fallback);
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

