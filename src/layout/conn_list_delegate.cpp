#include "conn_list_delegate.h"
#include <QDebug>

ConnListDelegate::ConnListDelegate(
        QObject *parent) :
    QStyledItemDelegate(parent)
{
}
void ConnListDelegate::paint(
        QPainter *painter,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    if ( index.column() == 2 ) {
        QIcon onView = qvariant_cast<QIcon>(
                    index.model()->data(
                        index, Qt::UserRole));
        QIcon icon = qvariant_cast<QIcon>(
                    index.model()->data(
                        index, Qt::DecorationRole));
        icon.paint(painter, option.rect, Qt::AlignCenter);
        onView.paint(painter, option.rect, Qt::AlignLeading);
    } else
        QStyledItemDelegate::paint(painter, option, index);
}
