#ifndef DOMAIN_LIST_DELEGATE_H
#define DOMAIN_LIST_DELEGATE_H

#include <QStyledItemDelegate>

class DomainListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DomainListDelegate(QObject *parent = Q_NULLPTR);

private slots:
    void paint(
            QPainter *painter,
            const QStyleOptionViewItem &option,
            const QModelIndex &index) const;
};

#endif // DOMAIN_LIST_DELEGATE_H
