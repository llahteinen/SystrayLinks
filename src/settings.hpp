#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QObject>
#include <QStandardPaths>


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


private:
    /// Give sensible default values here
    struct Persistables
    {
        QString linkPath{QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation)}; /// Points to Start Menu\Programs on Windows
        int itemLimit{15};
        int sortCondition{3};
        Qt::SortOrder sortDirection{Qt::DescendingOrder};
    } m_pers;

signals:

};

#endif // SETTINGS_HPP
