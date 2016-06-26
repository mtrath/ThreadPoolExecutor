#ifndef MAINVIEWQMLCONTEXT_H
#define MAINVIEWQMLCONTEXT_H

#include <QtCore/QObject>

class MainViewQmlContext : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString title READ getTitle CONSTANT)

public:
    explicit MainViewQmlContext(QObject *parent = 0);

signals:

public slots:
    void doAsync() const;
    void doLater() const;

private:
    QString getTitle() const;
};

#endif // MAINVIEWQMLCONTEXT_H
