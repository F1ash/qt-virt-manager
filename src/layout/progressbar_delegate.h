#ifndef PROGRESSBAR_DELEGATE_H
#define PROGRESSBAR_DELEGATE_H

#include <QStyledItemDelegate>
#include <QApplication>

class ProgressBarDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ProgressBarDelegate(QObject *parent = 0);

signals:

private slots:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

};

#endif // PROGRESSBAR_DELEGATE_H
