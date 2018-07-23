#ifndef NWFILTER_LISTD_ELEGATE_H
#define NWFILTER_LISTD_ELEGATE_H

#include <QStyledItemDelegate>

class NWFilterListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit NWFilterListDelegate(QObject *parent = Q_NULLPTR);

private slots:
    void paint(
            QPainter *painter,
            const QStyleOptionViewItem &option,
            const QModelIndex &index) const;
};

#endif // NWFILTER_LISTD_ELEGATE_H
