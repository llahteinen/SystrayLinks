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


private:
    /// Give sensible default values here
    struct Persistables
    {
        QString linkPath{QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation)}; /// Points to Start Menu\Programs on Windows
    } m_pers;

signals:

};

#endif // SETTINGS_HPP
