#ifndef TRAY_HPP
#define TRAY_HPP

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <memory>

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE


class Tray : public QObject
{
    Q_OBJECT
public:
    Tray(QObject* parent = nullptr);
    ~Tray() override = default;
    Q_DISABLE_COPY_MOVE(Tray)

    QSystemTrayIcon& trayIcon() { return* m_trayIcon; }

    void createTrayIcon();

signals:
    void restoreTriggered();
    void quitTriggered();

private slots:
    void onActivated(QSystemTrayIcon::ActivationReason reason);

private:
    QSystemTrayIcon* m_trayIcon{};
    QAction* m_restoreAction{};
    QAction* m_quitAction{};
    std::unique_ptr<QMenu> m_linkMenu{};
    std::unique_ptr<QMenu> m_contextMenu{};
};

#endif // TRAY_HPP
