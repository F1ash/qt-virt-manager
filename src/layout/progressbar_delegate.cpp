#include "progressbar_delegate.h"

ProgressBarDelegate::ProgressBarDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}
void ProgressBarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 2) {
        int progress = 0;
        QString State;
        bool ok = false;
        index.data().toInt(&ok);
        if ( ok ) {
            progress = index.data().toInt();
            State = QString::number(progress) + "%";
        } else
            State = index.data().toString();

        QStyleOptionProgressBar progressBarOption;
        progressBarOption.rect = option.rect;
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.progress = progress;
        progressBarOption.text = State;
        progressBarOption.textVisible = true;

        QApplication::style()->drawControl(QStyle::CE_ProgressBar,
                                           &progressBarOption, painter);
    } else
        QStyledItemDelegate::paint(painter, option, index);
}
