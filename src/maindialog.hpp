#ifndef MAINDIALOG_HPP
#define MAINDIALOG_HPP

#include "tray.hpp"
#include <QDialog>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainDialog; }
class QMenu;
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

    /// Call this once!
    void setupTrayIcon();

    Ui::MainDialog* ui;

    Tray m_tray;

};
#endif // MAINDIALOG_HPP
