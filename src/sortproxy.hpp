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

protected:
    bool lessThan(const QModelIndex& left, const QModelIndex& right) const override
    {
        auto column = left.column();
        switch(column)
        {
        case 3: /// by modified date
            {
                auto* fsm = qobject_cast<QFileSystemModel*>(sourceModel()); /// This should probably be moved to init or so
                /// compare full QDateTime, not the formatted string
                return fsm->fileInfo(left).lastModified() < fsm->fileInfo(right).lastModified();
            }
        case 0:
        case 1:
        case 2:
        default:
            return QSortFilterProxyModel::lessThan(left, right);
            break;
        }
    }
};

#endif // SORTPROXY_HPP

