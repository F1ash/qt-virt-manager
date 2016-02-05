#include "path_to_file.h"

Path_To_File::Path_To_File(QWidget *parent) :
    _Changed(parent)
{
    button = new QPushButton(
                QIcon::fromTheme("edit-find"),
                "",
                this);
    path = new QLineEdit(this);
    commonLayout = new QHBoxLayout(this);
    commonLayout->addWidget(button, 0, Qt::AlignLeft);
    commonLayout->addWidget(path);
    setLayout(commonLayout);
    connect(button, SIGNAL(clicked()),
            this, SLOT(searchPath()));
    connect(path, SIGNAL(textChanged(QString)),
            this, SLOT(stateChanged()));
}

/* public slots */
QString Path_To_File::getPath() const
{
    return path->text();
}
void Path_To_File::setPath(QString &_path)
{
    path->setText(_path);
}
void Path_To_File::setPlaceholderText(QString &_text)
{
    path->setPlaceholderText(_text);
}
void Path_To_File::clear()
{
    path->clear();
}

/* private slots */
void Path_To_File::searchPath()
{
    QString _path = QFileDialog::getOpenFileName(
                this,
                "Get path",
                "~");
    if ( !_path.isEmpty() ) path->setText(_path);
}
