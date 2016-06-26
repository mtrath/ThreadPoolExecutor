#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <memory>

#include <QtCore/QObject>
#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>

class ThreadPoolExecutor final : public QObject
{
    Q_OBJECT

public:
    enum class Scheduler {
        Main, Computation, Default, Io, Network
    };

    template<typename Func>
    static void addTask(Func&& f, ThreadPoolExecutor::Scheduler sched = Scheduler::Default) {
        auto func = std::move(f);
        auto& ex = getInstance();
        switch (sched) {
        case Scheduler::Main:
            ex._ioServiceMain.post([func] {
                try {
                    func();
                } catch (...) { }
            });
            emit ex.mainQueuePosted(QPrivateSignal());
            break;
        case Scheduler::Computation:
            ex._ioServiceComputation.post([func] {
                try {
                    func();
                } catch (...) { }
            });
            break;
        case Scheduler::Default:
            ex._ioServiceDefault.post([func] {
                try {
                    func();
                } catch (...) { }
            });
            break;
        case Scheduler::Io:
            ex._ioServiceIo.post([func] {
                try {
                    func();
                } catch (...) { }
            });
            break;
        case Scheduler::Network:
            ex._ioServiceNetwork.post([func] {
                try {
                    func();
                } catch (...) { }
            });
            break;

        default:
            break;
        }
    }

signals:
    void mainQueuePosted(QPrivateSignal);

private slots:
    void runMainQueue();

private:
    static ThreadPoolExecutor& getInstance();

    ThreadPoolExecutor();
    ~ThreadPoolExecutor();

    using work_t = boost::asio::io_service::work;

    boost::asio::io_service _ioServiceMain;

    boost::asio::io_service _ioServiceComputation;
    std::unique_ptr<work_t> _workComputation{new work_t(_ioServiceComputation)};
    boost::thread_group _threadsComputation;

    boost::asio::io_service _ioServiceDefault;
    std::unique_ptr<work_t> _workDefault{new work_t(_ioServiceDefault)};
    boost::thread_group _threadsDefault;

    boost::asio::io_service _ioServiceIo;
    std::unique_ptr<work_t> _workIo{new work_t(_ioServiceIo)};
    boost::thread_group _threadsIo;

    boost::asio::io_service _ioServiceNetwork;
    std::unique_ptr<work_t> _workNetwork{new work_t(_ioServiceNetwork)};
    boost::thread_group _threadsNetwork;
};

#endif // THREADPOOL_H
