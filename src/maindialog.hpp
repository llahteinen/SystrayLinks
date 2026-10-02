#ifndef MAINDIALOG_HPP
#define MAINDIALOG_HPP

#include "settings.hpp"
#include "tray.hpp"
#include "linksmenu.hpp"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainDialog; }
class QFileSystemModel;
QT_END_NAMESPACE


class MainDialog : public QWidget
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget* parent = nullptr);
    ~MainDialog() override;
    Q_DISABLE_COPY_MOVE(MainDialog)

protected:
    void closeEvent(QCloseEvent* event) override;

private:

    void recalculateUiSizes();
    void connectSettings();
    void connectUi();

    void closeLinksMenu(bool check = true);

    /// Call this once!
    void setupTrayIcon();

    Ui::MainDialog* ui;

    Settings m_settings;
    Tray m_tray;
    LinksMenu m_linksMenu;
    QFileSystemModel* m_model;

    bool m_linksMenuIsOpen;     ///< Used to track whether the menu should be opened or closed on tray clicks

};
#endif // MAINDIALOG_HPP
