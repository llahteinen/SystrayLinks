#ifndef LINKSMENU_HPP
#define LINKSMENU_HPP

#include <QMenu>

class Settings;
class SortProxy;
class QFileSystemModel;


class LinksMenu : public QMenu
{
public:
    using QMenu::QMenu;
    Q_DISABLE_COPY_MOVE(LinksMenu)

    LinksMenu(Settings& settings);

    void setRootPath(const QString& path);
    void updateMenu();

protected:
    void mouseReleaseEvent(QMouseEvent* e) override;

private:
    const Settings& m_settings;
    QFileSystemModel* const m_model;
    SortProxy* const m_sortProxy;
};

#endif // LINKSMENU_HPP

