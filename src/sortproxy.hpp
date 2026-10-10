#ifndef SORTPROXY_HPP
#define SORTPROXY_HPP

#include <QSortFilterProxyModel>
#include <QFileSystemModel>

/* qfilesystemmodel.cpp
case QFileSystemModelPrivate::NameColumn: return d->displayName(index);
case QFileSystemModelPrivate::SizeColumn: return d->size(index);
case QFileSystemModelPrivate::TypeColumn: return d->type(index);
case QFileSystemModelPrivate::TimeColumn: return d->time(index);
*/

class SortProxy : public QSortFilterProxyModel
{
public:
    using QSortFilterProxyModel::QSortFilterProxyModel;

    void setSourceModel(QAbstractItemModel*) override
    {
        Q_UNREACHABLE(); /// SortProxy only accepts QFileSystemModel, use the overload
    }
    void setSourceModel(QFileSystemModel* sourceModel)
    {
        m_fsm = sourceModel;
        QSortFilterProxyModel::setSourceModel(sourceModel);
    }

protected:
    const QFileSystemModel* m_fsm{};

    bool lessThan(const QModelIndex& left, const QModelIndex& right) const override
    {
        const auto column = left.column();
        switch(column)
        {
        case 1: /// by size
            {
                return m_fsm->fileInfo(left).size() < m_fsm->fileInfo(right).size();
            }
        case 2: /// by type
            {
                const QFileInfo left_info = m_fsm->fileInfo(left);
                const QFileInfo right_info = m_fsm->fileInfo(right);
                /// Dirs first
                if(left_info.isDir() != right_info.isDir())
                {
                    return left_info.isDir();
                }
                /// By filename within the same file type
                const QString left_suffix = left_info.suffix().toLower();
                const QString right_suffix = right_info.suffix().toLower();
                if(left_suffix == right_suffix)
                {
                    return left_info.fileName() < right_info.fileName();
                }
                /// This sorts .lnk files just like any other file extension
                return left_suffix.toLower() < right_suffix.toLower();
            }
        case 3: /// by modified date
            {
                return m_fsm->fileInfo(left).lastModified() < m_fsm->fileInfo(right).lastModified();
            }
        case 0: /// by name
        default:
            /// alphabetical sort
            return QSortFilterProxyModel::lessThan(left, right);
        }
    }
};

#endif // SORTPROXY_HPP

