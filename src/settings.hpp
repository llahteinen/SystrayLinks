#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QObject>
#include <QStandardPaths>
#include <QFont>


class Settings : public QObject
{
    Q_OBJECT

    static constexpr int SETTINGS_VERSION = 1;

public:
    explicit Settings(QObject* parent = nullptr);

    void persistToStore();
    void loadFromStore();

    void setLinkPath(const QString& path);
    const QString& getLinkPath() const;

    void setItemLimit(int newItemLimit);
    int getItemLimit() const;

    void setSortCondition(int newSortCondition);
    int getSortCondition() const;

    void setSortDirection(Qt::SortOrder newSortDirection);
    Qt::SortOrder getSortDirection() const;

    void setFont(const QFont& font);
    QFont getFont() const;

private:
    /// Give sensible default values here
    struct Persistables
    {
        QString linkPath{QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation)}; /// Points to Start Menu\Programs on Windows
        int itemLimit{15};
        int sortCondition{3};
        Qt::SortOrder sortDirection{Qt::DescendingOrder};
        QFont menuFont{};

        Persistables()
        {
            /// https://learn.microsoft.com/en-us/windows/apps/design/signature-experiences/typography
            /// The example uses the "Text" variant for 14 to 18 px font. 12 px uses "Small"
            /// "Use Semibold instead of Bold for emphasis."
            menuFont.setFamily("Segoe UI Variable Text");
            menuFont.setPixelSize(14); /// Fluent UI Body text 14 epx. In Qt 6 pixelSize should be safe over different scalings
        }

    } m_pers;

signals:

};

#endif // SETTINGS_HPP
