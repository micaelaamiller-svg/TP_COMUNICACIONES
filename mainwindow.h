#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QByteArray>
#include <QVector>
#include <QTimer>
#include <QString>
#include <stdint.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;


private:

    Ui::MainWindow *ui;


private slots:

    void connect_tp_server();
    void socket_connect();
    void connection_lost();
    void connection_error();
    uint16_t CRC(uint8_t *data, uint16_t len);
    void send_data();
    void receive_data();
    void configurarIntervalo(const QString &modo);
    void actualizarPuntos();
    void limpiarGrafico();
    void tiempoAgotado();


private:

    QTcpSocket socket;
    QByteArray bufferRx;
    QVector<double> ejeX;
    QVector<QVector<double>> valores;
    int contadorMuestras = 0;
    int cantidadEsperada = 0;
    int slaveEsperado = 0;
    int direccionSolicitada = 0;
    int direccionGrafico = -1;
    int limitePuntos = 100;
    int solicitudPendiente = 0;
    QTimer timer;
    QTimer timerRespuesta;

};

#endif
