#include "tray.hpp"


Tray::Tray(QObject* parent)
    : QObject(parent)
{
    createTrayIcon();

    /// Public signals forwarding
    connect(m_restoreAction, &QAction::triggered, this, &Tray::restoreTriggered);
    connect(m_quitAction, &QAction::triggered, this, &Tray::quitTriggered);

    /// Private slots
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &Tray::onActivated);
}

void Tray::onActivated(QSystemTrayIcon::ActivationReason reason)
{
    qDebug() << "onActivated" << reason;
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
        {
            break;
        }
    case QSystemTrayIcon::DoubleClick:
        {
            m_restoreAction->trigger();
            break;
        }
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        break;
    }
}

void Tray::createTrayIcon()
{
    Q_ASSERT(m_trayIcon == nullptr); /// Don't call this more than once

    m_trayIcon = new QSystemTrayIcon(this);

    /// Use unique pointer because QMenu needs QWidget parent, QObject isn't enough.
    /// We would use the parent only for lifetime management anyway.
    m_linkMenu = std::make_unique<QMenu>();
    m_contextMenu = std::make_unique<QMenu>();

    m_restoreAction = new QAction(tr("&Settings"), this);

    m_quitAction = new QAction(tr("&Quit"), this);

    m_contextMenu->addAction(m_restoreAction);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(m_quitAction);

    m_trayIcon->setContextMenu(m_contextMenu.get()); /// The system tray icon does not take ownership of the menu
}
