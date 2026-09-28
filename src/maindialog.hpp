#ifndef MAINDIALOG_HPP
#define MAINDIALOG_HPP

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

protected:
    void closeEvent(QCloseEvent* event) override;

private:

    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void createTrayIcon();

    Ui::MainDialog* ui;

    QSystemTrayIcon* m_trayIcon{};
    QMenu* m_trayIconLinkMenu{};
    QMenu* m_trayIconContextMenu{};
    QAction* m_restoreAction{};
    QAction* m_quitAction{};
};
#endif // MAINDIALOG_HPP
