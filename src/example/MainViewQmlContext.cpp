#include "MainViewQmlContext.h"

#include <iostream>
#include <thread>

#include "PrintThread.h"
#include "ThreadPoolExecutor.h"

MainViewQmlContext::MainViewQmlContext(QObject *parent) : QObject(parent)
{ }

void MainViewQmlContext::doAsync() const
{
    printThread("doAsync begin");
    ThreadPoolExecutor::addTask([]{
        printThread("doAsync");
    });
    printThread("doAsync end");
}

void MainViewQmlContext::doLater() const
{
    printThread("doLater begin");
    ThreadPoolExecutor::addTask([]{
        printThread("doLater");
    }, ThreadPoolExecutor::Scheduler::Main);
    printThread("doLater end");
}

QString MainViewQmlContext::getTitle() const
{
    return QStringLiteral("Hello world!");
}
