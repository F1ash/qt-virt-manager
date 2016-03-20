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
    } else if ( index.column() < 2 ) {
#if QT_VERSION > QT_VERSION_CHECK(5, 0, 0)
        QStyleOptionViewItem _option = option;
        _option.rect = option.rect;
        _option.displayAlignment = Qt::AlignLeft;
        _option.text = index.model()->data(
                    index, Qt::DisplayRole).toString();
        QStyledItemDelegate::paint(painter, _option, index);

        QIcon icon = qvariant_cast<QIcon>(
                    index.model()->data(
                        index, Qt::DecorationRole));
        icon.paint(painter, option.rect, Qt::AlignLeft);
#else
        QStyledItemDelegate::paint(painter, option, index);
#endif
    } else
        QStyledItemDelegate::paint(painter, option, index);
}
