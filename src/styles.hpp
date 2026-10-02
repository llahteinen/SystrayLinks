#ifndef STYLES_HPP
#define STYLES_HPP

#include <QObject>
class QString;
class QIcon;


namespace Styles
{
    Q_NAMESPACE

    enum class Icons
    {
        Default = 0,
        Heart,
        HeartFill,
        FavoriteList,
        FavoriteStar,
        FavoriteStarFill,
        _END
    };
    Q_ENUM_NS(Icons);

    QIcon fluentIcon(Icons name, const QIcon& fallback);

    const QString& styleSheetLinksMenu();

};

#endif // STYLES_HPP
