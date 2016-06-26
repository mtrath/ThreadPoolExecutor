#include <chrono>
#include <thread>

#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtWidgets/QApplication>

#include "MainViewQmlContext.h"
#include "PrintThread.h"
#include "Task.h"
#include "ThreadPoolExecutor.h"

int main(int argc, char* argv[])
{
    printThread("Main thread");

    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    MainViewQmlContext context;
    engine.rootContext()->setContextProperty(QStringLiteral("mainViewContext"), &context);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    // example 1
    Task<void>([]() {
        // ...
    })
    .start();

    // example 2
    Task<void>([]() {
        // ...
    })
    .then([]() {
        // ...
    })
    .start();

    // example 3
    Task<void>([]() {
        // ...
        throw std::exception();
    })
    .then([](std::future<void> future) {
        // ...
        try {
            future.get();
        } catch (std::exception& e) {
            printThread("catch example 3");
        }
    })
    .start();

    // example 4
    Task<int>([]() {
        //throw std::exception();
        return 4;
    })
    .then([](std::future<int> future) {
        try {
            auto value = future.get();
            printThread(std::string("Got value: ") + std::to_string(value));
        } catch (std::exception e) {
            printThread(std::string("Caught exception what: ") + e.what());
        }
    })
    .start();

    // example 5
    ThreadPoolExecutor::addTask([] {
        // ...

        ThreadPoolExecutor::addTask([] {
            // ...
        }, Scheduler::Main);

        // ...
    }); //, Scheduler::Default);

    return app.exec();
}
