#ifndef MAINDIALOG_HPP
#define MAINDIALOG_HPP

#include "settings.hpp"
#include "tray.hpp"
#include <QDialog>
#include <QSystemTrayIcon>
#include <QMenu>

QT_BEGIN_NAMESPACE
namespace Ui { class MainDialog; }
class QFileSystemModel;
QT_END_NAMESPACE


class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget* parent = nullptr);
    ~MainDialog() override;
    Q_DISABLE_COPY_MOVE(MainDialog)

protected:
    void closeEvent(QCloseEvent* event) override;

private:

    void updateLinksMenu();

    void closeLinksMenu(bool check = true);

    /// Call this once!
    void setupTrayIcon();

    Ui::MainDialog* ui;

    Settings m_settings;
    Tray m_tray;
    QMenu m_linksMenu;
    QFileSystemModel* m_model;

    bool m_linksMenuIsOpen;     ///< Used to track whether the menu should be opened or closed on tray clicks

};
#endif // MAINDIALOG_HPP
