#ifndef WORKER_HH
# define WORKER_HH

# include <QThread>

# include "Minestorm.hh"
# include "BaseSocket.hh"

/**
 * @brief The Worker class handle connection received from the Server in a separated thread
 */
class Worker : public QThread
{
    Q_OBJECT

private:
    BaseSocket          _socket;

public:
    /**
     * @brief Worker construct the Worker
     * @param socketFd is the socket descriptor
     */
    Worker(qint32 socketFd, QObject *parent = nullptr);
    ~Worker();

    qint32              socketFd() const;
    const BaseSocket    *socket() const;

signals:
    void                clientDisconnected(qint32 socketFd);

private slots:
    /**
     * @brief disconnected is received when the socket got disconnected
     */
    void                disconnected();

    // QThread interface
protected:
    void                run() Q_DECL_OVERRIDE;

};

#endif // WORKER_HH
