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
        Default = 0, /// HeartFill
        Heart,
        HeartFill,
        FavoriteStar,
        FavoriteStarFill,
        FavoriteList,
        Pinned,
        Attach,
        Folder,
        FolderOpen,
        Favicon2,
        Library,
        Download,
        CloudDownload,
        GridView,
        AllApps,
        List,
        TextBulletListSquare,
        TextBulletListSquareSparkle,
        ExpressiveInputEntry,
        Asterisk,
        AsteriskBadge12,
        CaretUp8,
        CaretUpSolid8,
        CaretSolidUp,
        _END
    };
    Q_ENUM_NS(Icons);

    QChar fluentIconGlyph(Icons index);
    Icons fluentIconEnum(QChar glyph);
    QIcon fluentIcon(QChar glyph);
    QIcon fluentIcon(QChar glyph, const QIcon& fallback);
    QIcon fluentIcon(Icons name, const QIcon& fallback);
    /// Returns all icons including the Default
    QList<QIcon> fluentIconsList();

    const QString& styleSheetLinksMenu();
    QString styleSheetWindow();

};

#endif // STYLES_HPP
