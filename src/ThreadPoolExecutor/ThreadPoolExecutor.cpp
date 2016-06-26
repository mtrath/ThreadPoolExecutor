#include "ThreadPoolExecutor.h"

#include <QtCore/QThread>

void ThreadPoolExecutor::runMainQueue()
{
    if (_ioServiceMain.stopped())
        _ioServiceMain.reset();

    _ioServiceMain.run();
}

ThreadPoolExecutor& ThreadPoolExecutor::getInstance()
{
    static ThreadPoolExecutor executor;
    return executor;
}

ThreadPoolExecutor::ThreadPoolExecutor()
{
    const auto idealCount = std::max(QThread::idealThreadCount(), 2);
    for (int var = 0; var < idealCount; ++var) {
        _threadsComputation.create_thread([this] {
            _ioServiceComputation.run();
        });
    }
    for (int var = 0; var < idealCount; ++var) {
        _threadsDefault.create_thread([this] {
            _ioServiceDefault.run();
        });
    }
    for (int var = 0; var < 2 * idealCount; ++var) {
        _threadsIo.create_thread([this] {
            _ioServiceIo.run();
        });
    }
    for (int var = 0; var < 2 * idealCount; ++var) {
        _threadsNetwork.create_thread([this] {
            _ioServiceNetwork.run();
        });
    }

    connect(this, &ThreadPoolExecutor::mainQueuePosted,
            this, &ThreadPoolExecutor::runMainQueue,
            Qt::QueuedConnection);
}

ThreadPoolExecutor::~ThreadPoolExecutor()
{
    _workDefault.reset();
    _workIo.reset();

    _threadsIo.join_all();
    _threadsDefault.join_all();
}
