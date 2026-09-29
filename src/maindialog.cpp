#include "maindialog.hpp"
#include "ui_maindialog.h"
#include <QCloseEvent>
#include <QFileSystemModel>
#include <QDesktopServices>


MainDialog::MainDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::MainDialog)
    , m_tray{} /// No parent for value member QObject
    , m_linksMenu{}
    , m_model{new QFileSystemModel()}
{
    ui->setupUi(this);

    m_model->setRootPath(R"(E:\Kikkareita\linkit)");
    m_model->setFilter(//QDir::AllEntries | QDir::NoDotAndDotDot);
            QDir::AllEntries
            | QDir::NoDotAndDotDot
            | QDir::System /// This allows displaying broken shortcut links
        );

    setupTrayIcon();
    m_tray.trayIcon().show();
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
    if(m_tray.trayIcon().isVisible())
    {
        hide();
        event->ignore();
    }
}

void MainDialog::updateLinksMenu()
{
    auto& menu = m_linksMenu;
    menu.clear();

    const QModelIndex rootIndex = m_model->index(m_model->rootPath());
    for(int row = 0; row < m_model->rowCount(rootIndex); ++row)
    {
        const QModelIndex idx = m_model->index(row, 0, rootIndex);
        const QString name = m_model->fileName(idx); /// Contains the name a shortcut points to
        const QString path = m_model->filePath(idx);
        /// QIcon uses implicit sharing so we shouldn't be wasting meaningful amounts of ram when
        /// creating a new icon object for each and every menu action
        const QIcon icon = m_model->fileIcon(idx);
        const bool isDir = m_model->isDir(idx);
        const bool hasChildren = m_model->hasChildren(idx);
        qInfo() << name << path;
        qInfo() << "isDir, hasChildren" << isDir << hasChildren;

        QFileInfo entry(path);
//        qDebug() << entry;
//        qDebug() << entry.canonicalFilePath();
        const QString canonical_path = entry.canonicalFilePath(); /// This seems to resolve through Windows .lnk shortcuts

        QString file_name;
        static constexpr bool setting_hideLnkExtension = true;
        if(setting_hideLnkExtension &&
            entry.suffix().toLower() == "lnk")
        {
            file_name = entry.completeBaseName();
        }
        else
        {
            file_name = entry.fileName(); /// The name of the shortcut
//            file_name = name; /// The name a shortcut points to
        }

        const bool broken_link = !entry.exists() && entry.isShortcut();

        if(entry.isFile() || entry.isDir()) /// Not sure if this works with symlinks
        {
            QAction* action = menu.addAction(file_name);
            action->setIcon(icon);

            QObject::connect(action, &QAction::triggered,
                    &menu, [canonical_path] {
                    qDebug() << "Selected file:" << canonical_path;
                    QDesktopServices::openUrl(QUrl::fromLocalFile(canonical_path));
                    });
        }
        else if(broken_link) /// Show broken links greyed out
        {
            QAction* action = menu.addAction(file_name);
            action->setEnabled(false);
            action->setIcon(icon);

            /// Grayed out icon
            QPixmap disabled = icon.pixmap(QSize(16, 16), QIcon::Disabled);
            QIcon d_icon(disabled);
            action->setIcon(d_icon);
        }
    }
}

void MainDialog::setupTrayIcon()
{
    connect(&m_tray, &Tray::activated, this, [this]() {
        qDebug() << "activated";
        updateLinksMenu();
        m_linksMenu.popup(QCursor::pos());
    });

    connect(&m_tray, &Tray::restoreTriggered, this, [this]() {
        qDebug() << "restoreTriggered";
        showNormal(); /// Window "restore" action
        activateWindow(); /// Make the dialog foreground if user had clicked something else
    });

    connect(&m_tray, &Tray::quitTriggered, qApp, &QCoreApplication::quit);

    QIcon icon = QIcon::fromTheme("emblem-favorite");
    m_tray.trayIcon().setIcon(icon);
    setWindowIcon(icon);
}
