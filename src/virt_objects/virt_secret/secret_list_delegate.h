#ifndef SECRET_LISTD_ELEGATE_H
#define SECRET_LISTD_ELEGATE_H

#include <QStyledItemDelegate>

class SecretListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SecretListDelegate(QObject *parent = Q_NULLPTR);

private slots:
    void paint(
            QPainter *painter,
            const QStyleOptionViewItem &option,
            const QModelIndex &index) const;
};

#endif // SECRET_LISTD_ELEGATE_H
