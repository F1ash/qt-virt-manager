#include "progressbar_delegate.h"

ProgressBarDelegate::ProgressBarDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}
void ProgressBarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 2) {
        QString State;
        bool ok = false;
        int progress = index.data().toInt(&ok);
        if ( ok ) {
            State = (progress==100)? "Complete":"Connect...";
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
