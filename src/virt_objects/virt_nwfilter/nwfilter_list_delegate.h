#ifndef NWFILTER_LISTD_ELEGATE_H
#define NWFILTER_LISTD_ELEGATE_H

#include <QStyledItemDelegate>

class NWFilterListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit NWFilterListDelegate(QObject *parent = nullptr);

private slots:
    void paint(
            QPainter *painter,
            const QStyleOptionViewItem &option,
            const QModelIndex &index) const;
};

#endif // NWFILTER_LISTD_ELEGATE_H
