#include "linksmenu.hpp"
#include "settings.hpp"
#include "sortproxy.hpp"
#include <QMouseEvent>
#include <QDesktopServices>
#include <QToolTip>


LinksMenu::LinksMenu(Settings& settings)
    : QMenu()
    , m_settings{settings}
    , m_model{new QFileSystemModel(this)}
    , m_sortProxy{new SortProxy(m_model)}
{
    m_model->setFilter(
        QDir::AllEntries
        | QDir::NoDotAndDotDot
        | QDir::System /// This allows displaying broken shortcut links
        );

    m_sortProxy->setSourceModel(m_model);
    /// Sort so that capital and non-capital file names get mixed together (Windows style)
    m_sortProxy->setSortCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
}

void LinksMenu::setRootPath(const QString& path)
{
    m_model->setRootPath(path);
}

void LinksMenu::updateMenu()
{
    auto& menu = *this;
    menu.clear();
    /// Should set maximum size for the menu so that the bottom edge doesn't overlap the icon

    m_sortProxy->sort(m_settings.getSortCondition(), m_settings.getSortDirection());
    /// I think it sorts by the target of shortcuts not the link itself

    const QModelIndex rootIndex = m_model->index(m_model->rootPath());
    const QModelIndex proxyRoot = m_sortProxy->mapFromSource(rootIndex);
    const int item_count = m_sortProxy->rowCount(proxyRoot);
    const int max_count = m_settings.getItemLimit();

    for(int row = 0; row < item_count && row < max_count; ++row)
    {
        QModelIndex pIdx = m_sortProxy->index(row, 0, proxyRoot);
        QModelIndex idx = m_sortProxy->mapToSource(pIdx);
        const QString name = m_model->fileName(idx); /// Contains the name a shortcut points to
        const QString path = m_model->filePath(idx);
        /// QIcon uses implicit sharing so we shouldn't be wasting meaningful amounts of ram when
        /// creating a new icon object for each and every menu action
        const QIcon icon = m_model->fileIcon(idx);
//        const bool isDir = m_model->isDir(idx);
//        const bool hasChildren = m_model->hasChildren(idx);
        qDebug() << name << path;
//        qDebug() << "isDir, hasChildren" << isDir << hasChildren;

        QFileInfo entry(path);
        /// On Windows, shortcuts (.lnk files) are currently treated as symlinks. As on Unix systems, the property getters return the size of
        /// the target,not the .lnk file itself. This behavior is deprecated and will likely be removed in a future version of Qt, after
        /// which .lnk files will be treated as regular files.
        /// entry.symLinkTarget(); /// Should return the target of .lnk files
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
            action->setData(canonical_path);
            action->setToolTip(canonical_path);

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
            action->setToolTip(canonical_path);

            /// Grayed out icon
            QPixmap disabled = icon.pixmap(QSize(16, 16), QIcon::Disabled);
            QIcon d_icon(disabled);
            action->setIcon(d_icon);
        }
    }

    if(item_count > max_count)
    {
        QAction* limit_action = menu.addAction(tr("%1 items not shown").arg(item_count - max_count));
        auto font = limit_action->font();
//        font.setBold(true);
        font.setItalic(true);
//        font.setFamily("Segoe UI Variable Small Semibold");
        limit_action->setFont(font);
        limit_action->setToolTip(m_model->rootPath());

        QObject::connect(limit_action, &QAction::triggered,
            &menu, [this] {
                qDebug() << "Root path:" << m_model->rootPath();
                QDesktopServices::openUrl(QUrl::fromLocalFile(m_model->rootPath()));
            });
    }
}

void LinksMenu::mouseReleaseEvent(QMouseEvent* e)
{
    if(e->button() == Qt::RightButton)
    {
        qDebug() << "mouseReleaseEvent Qt::RightButton";
        if(auto* action = activeAction())
        {
            qDebug() << action->data();
            const QString path = action->data().toString();
            if(!path.isEmpty())
            {
                /// PLACEHOLDER implementation
                /// BTW tooltips should be styled
                QToolTip::showText(
                    e->globalPosition().toPoint(),
                    action->toolTip(),
                    this
                    );
                /// Open OS file context menu
//                emit contextMenuRequested(action, e->globalPosition().toPoint());
            }
            /// Ehkä voisi olla oma QAction jolla action->rightTriggered tai triggered(RightClick)
            /// jolloin voisi tehdä actionin connection kautta hommat
        }
        return; /// Prevent the default behavior
    }
    QMenu::mouseReleaseEvent(e);
}

