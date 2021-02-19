#include "interface_list_delegate.h"

IfaceListDelegate::IfaceListDelegate(
        QObject *parent) :
    QStyledItemDelegate(parent)
{
}
void IfaceListDelegate::paint(
        QPainter *painter,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    if ( index.column() > 1 ) {
        QIcon icon = qvariant_cast<QIcon>(
                    index.model()->data(
                        index, Qt::DecorationRole));
        icon.paint(painter, option.rect, Qt::AlignCenter);
    } else if ( index.column() < 2 ) {
#if (QT_VERSION > QT_VERSION_CHECK(5, 0, 0))
        //QStyleOptionViewItem _option = option;
        //_option.rect = option.rect;
        //_option.displayAlignment = Qt::AlignLeft;
        //_option.text = index.model()->data(
        //            index, Qt::DisplayRole).toString();
        QStyledItemDelegate::paint(painter, option, index);

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
