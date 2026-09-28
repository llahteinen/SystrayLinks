#include "maindialog.hpp"
#include "ui_maindialog.h"
#include <QMenu>
#include <QCloseEvent>


MainDialog::MainDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::MainDialog)
{
    ui->setupUi(this);

    createTrayIcon();
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainDialog::onTrayIconActivated);
    m_trayIcon->show();
}

MainDialog::~MainDialog()
{
    delete ui;
}

/// This only happens when the main window is closed, but not when the app is quit from the system tray
void MainDialog::closeEvent(QCloseEvent* event)
{
    qDebug() << "MainDialog::closeEvent";
    /// Keep app running, just close the dialog
    if(m_trayIcon->isVisible())
    {
        hide();
        event->ignore();
    }
}

void MainDialog::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    qDebug() << "onTrayIconActivated" << reason;
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

void MainDialog::createTrayIcon()
{
    Q_ASSERT(m_trayIconLinkMenu == nullptr); /// Don't call this more than once

    m_trayIconLinkMenu = new QMenu(this);

    m_trayIconContextMenu = new QMenu(this);

    m_restoreAction = new QAction(tr("&Settings"), this);
    connect(m_restoreAction, &QAction::triggered, this, [this]() {
        showNormal(); /// Window "restore" action
        activateWindow(); /// Make the dialog foreground if user had clicked something else
    });

    m_quitAction = new QAction(tr("&Quit"), this);
    connect(m_quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    m_trayIconContextMenu->addAction(m_restoreAction);
    m_trayIconContextMenu->addSeparator();
    m_trayIconContextMenu->addAction(m_quitAction);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setContextMenu(m_trayIconContextMenu);

    QIcon icon = QIcon::fromTheme("emblem-favorite");
    m_trayIcon->setIcon(icon);
    setWindowIcon(icon);
}
