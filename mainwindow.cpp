#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdint.h>
#include <stdio.h>

#include <QByteArray>
#include <QDebug>
#include <QPushButton>
#include <QComboBox>
#include <QTimer>
#include <QColor>
#include <QPen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->connect, &QPushButton::clicked,
            this, &MainWindow::connect_tp_server);

    connect(ui->request, &QPushButton::clicked,
            this, &MainWindow::send_data);

    connect(ui->update, &QPushButton::clicked,
            this, &MainWindow::actualizarPuntos);

    connect(ui->clear, &QPushButton::clicked,
            this, &MainWindow::limpiarGrafico);

    connect(&socket, &QTcpSocket::connected,
            this, &MainWindow::socket_connect);

    connect(&socket, &QTcpSocket::disconnected,
            this, &MainWindow::connection_lost);

    connect(&socket, &QTcpSocket::errorOccurred,
            this, &MainWindow::connection_error);

    connect(&socket, &QTcpSocket::readyRead,
            this, &MainWindow::receive_data);

    ui->request->setEnabled(false);

    ui->connect->setText("CONNECT");

    ui->connect->setStyleSheet(
        "background-color: red; color: white;"
        );

    ui->comboBox->clear();

    ui->comboBox->addItem("Manual request");
    ui->comboBox->addItem("500 ms");
    ui->comboBox->addItem("1000 ms");
    ui->comboBox->addItem("2000 ms");

    connect(ui->comboBox, &QComboBox::currentTextChanged,
            this, &MainWindow::configurarIntervalo);

    connect(&timer, &QTimer::timeout,
            this, &MainWindow::send_data);

    timerRespuesta.setSingleShot(true);

    connect(&timerRespuesta, &QTimer::timeout,
            this, &MainWindow::tiempoAgotado);

    ui->plot->xAxis->setLabel("Numero de muestra");

    ui->plot->yAxis->setLabel("Valor del registro");

    ui->plot->legend->setVisible(true);

    if(ui->plot_p->text().isEmpty())
    {
        ui->plot_p->setText("100");
    }

    limitePuntos = 100;
}

MainWindow::~MainWindow()
{
    timer.stop();
    timerRespuesta.stop();

    delete ui;
}

void MainWindow::connect_tp_server()
{
    if(socket.state() == QAbstractSocket::UnconnectedState)
    {
        QString ip = ui->s_IP->text();

        int puerto = ui->S_TCP->text().toInt();

        if(ip.isEmpty() || puerto <= 0 || puerto > 65535)
        {
            qDebug() << "IP o puerto incorrecto";
            return;
        }

        socket.connectToHost(ip, puerto);

        ui->connect->setEnabled(false);

        qDebug() << "Intentando conectar al servidor...";
    }

    else if(socket.state() == QAbstractSocket::ConnectedState)
    {
        timer.stop();

        socket.disconnectFromHost();

        ui->connect->setEnabled(false);
    }
}

void MainWindow::socket_connect()
{
    qDebug() << "Conexion exitosa";

    bufferRx.clear();

    solicitudPendiente = 0;

    timerRespuesta.stop();

    ui->connect->setText("DISCONNECT");

    ui->connect->setStyleSheet(
        "background-color: green; color: white;"
        );

    ui->connect->setEnabled(true);

    configurarIntervalo(ui->comboBox->currentText());
}

void MainWindow::connection_lost()
{
    qDebug() << "Desconectado";

    timer.stop();

    timerRespuesta.stop();

    solicitudPendiente = 0;

    bufferRx.clear();

    ui->request->setEnabled(false);

    ui->connect->setText("CONNECT");

    ui->connect->setStyleSheet(
        "background-color: red; color: white;"
        );

    ui->connect->setEnabled(true);
}

void MainWindow::connection_error()
{
    qDebug() << "Error de conexion:"
             << socket.errorString();

    timer.stop();

    timerRespuesta.stop();

    solicitudPendiente = 0;


    if(socket.state() == QAbstractSocket::UnconnectedState)
    {
        ui->request->setEnabled(false);

        ui->connect->setText("CONNECT");

        ui->connect->setStyleSheet(
            "background-color: red; color: white;"
            );

        ui->connect->setEnabled(true);
    }
}

uint16_t MainWindow::CRC(uint8_t *data, uint16_t len)
{
    uint16_t i, j, temp_bit, temp_int, crc;

    crc = 0xFFFF;


    for(i = 0; i < len; i++)
    {
        temp_int = *data++;

        crc ^= temp_int;


        for(j = 0; j < 8; j++)
        {
            temp_bit = crc & 0x0001;

            crc >>= 1;


            if(temp_bit != 0)
            {
                crc ^= 0xA001;
            }
        }
    }


    return crc;
}

void MainWindow::send_data()
{
    if(socket.state() != QAbstractSocket::ConnectedState)
    {
        qDebug() << "No hay conexion con el servidor";
        return;
    }

    if(solicitudPendiente == 1)
    {
        return;
    }

    uint8_t frame[8] = {0};

    int slave = ui->slave_address->text().toInt();

    frame[1] = 3;

    int address = ui->start_data->text().toInt();

    if(address >= 40001 && address <= 49999)
    {
        address = address - 40001;
    }

    int cantidad = ui->register_2->text().toInt();

    if(slave < 1 || slave > 247 ||
        address < 0 || address > 65535 ||
        cantidad < 1 || cantidad > 125 ||
        address + cantidad > 65536)
    {
        qDebug() << "Configuracion Modbus incorrecta";
        return;
    }

    frame[0] = slave;

    frame[1] = 3;

    frame[2] = address >> 8;

    frame[3] = address & 0xFF;

    frame[4] = cantidad >> 8;

    frame[5] = cantidad & 0xFF;

    uint16_t crc_value = CRC(frame, 6);

    frame[6] = crc_value & 0xFF;

    frame[7] = crc_value >> 8;

    cantidadEsperada = cantidad;

    slaveEsperado = slave;

    direccionSolicitada = ui->start_data->text().toInt();

    QByteArray trama(
        reinterpret_cast<const char*>(frame), 8
        );

    qDebug() << "Trama Modbus:"
             << trama.toHex(' ').toUpper();

    bufferRx.clear();

    if(socket.write(trama) == -1)
    {
        qDebug() << "Error al enviar la solicitud";
        return;
    }

    solicitudPendiente = 1;

    timerRespuesta.start(3000);
}

void MainWindow::tiempoAgotado()
{
    if(solicitudPendiente == 1)
    {
        qDebug() << "El servidor no respondio a tiempo";

        solicitudPendiente = 0;

        bufferRx.clear();
    }
}

void MainWindow::receive_data()
{
    bufferRx.append(socket.readAll());

    while(bufferRx.size() >= 3)
    {
        if(solicitudPendiente == 0)
        {
            bufferRx.clear();
            return;
        }

        int slave = (uint8_t)bufferRx[0];

        int funcion = (uint8_t)bufferRx[1];

        if(slave != slaveEsperado)
        {
            bufferRx.remove(0, 1);
            continue;
        }

        if(funcion != 3 && funcion != 0x83)
        {
            bufferRx.remove(0, 1);
            continue;
        }

        int longitud = 0;

        if(funcion == 0x83)
        {
            longitud = 5;
        }

        else
        {
            int bytesDatos = (uint8_t)bufferRx[2];

            if(bytesDatos < 2 ||
                bytesDatos > 250 ||
                bytesDatos % 2 != 0)
            {
                bufferRx.remove(0, 1);
                continue;
            }

            longitud = 3 + bytesDatos + 2;
        }

        if(bufferRx.size() < longitud)
        {
            return;
        }

        QByteArray respuesta = bufferRx.left(longitud);

        uint16_t crcCalculado = CRC(
            reinterpret_cast<uint8_t*>(respuesta.data()),
            longitud - 2
            );

        uint16_t crcRecibido =
            (uint8_t)respuesta[longitud - 2] |
            ((uint16_t)(uint8_t)respuesta[longitud - 1] << 8);

        if(crcCalculado != crcRecibido)
        {
            qDebug() << "Error de CRC";

            bufferRx.remove(0, 1);

            continue;
        }

        bufferRx.remove(0, longitud);

        timerRespuesta.stop();

        solicitudPendiente = 0;

        qDebug() << "Respuesta Modbus:"
                 << respuesta.toHex(' ').toUpper();

        if(funcion == 0x83)
        {
            qDebug() << "Excepcion Modbus:"
                     << (uint8_t)respuesta[2];

            continue;
        }

        int bytesDatos = (uint8_t)respuesta[2];

        int cantidad = bytesDatos / 2;

        if(cantidad != cantidadEsperada)
        {
            qDebug() << "Cantidad de registros inesperada";
            continue;
        }

        if(ui->plot->graphCount() != cantidad ||
            valores.size() != cantidad ||
            direccionGrafico != direccionSolicitada)
        {
            ui->plot->clearGraphs();

            ejeX.clear();

            valores.clear();

            valores.resize(cantidad);

            contadorMuestras = 0;

            direccionGrafico = direccionSolicitada;

            for(int i = 0; i < cantidad; i++)
            {
                ui->plot->addGraph();

                QColor color = QColor::fromHsv(
                    (i * 90) % 360, 220, 220
                    );

                ui->plot->graph(i)->setPen(
                    QPen(color)
                    );

                int direccion = direccionGrafico + i;

                ui->plot->graph(i)->setName(
                    QString("Registro %1").arg(direccion)
                    );
            }
        }

        ejeX.append(contadorMuestras);

        contadorMuestras++;

        for(int i = 0; i < cantidad; i++)
        {
            int alto = (uint8_t)respuesta[3 + i * 2];

            int bajo = (uint8_t)respuesta[4 + i * 2];

            int valor = (alto << 8) | bajo;

            valores[i].append(valor);

            qDebug() << "Registro" << direccionGrafico + i
                     << "Valor:" << valor;
        }

        while(ejeX.size() > limitePuntos)
        {
            ejeX.remove(0);

            for(int i = 0; i < cantidad; i++)
            {
                valores[i].remove(0);
            }
        }

        for(int i = 0; i < cantidad; i++)
        {
            ui->plot->graph(i)->setData(
                ejeX, valores[i]
                );
        }

        ui->plot->rescaleAxes();

        ui->plot->replot();
    }
}

void MainWindow::configurarIntervalo(const QString &modo)
{
    timer.stop();

    if(socket.state() != QAbstractSocket::ConnectedState)
    {
        ui->request->setEnabled(false);
        return;
    }

    if(modo == "Manual request")
    {
        ui->request->setEnabled(true);

        qDebug() << "Modo manual activado";
    }

    else if(modo == "500 ms")
    {
        ui->request->setEnabled(false);

        timer.start(500);

        qDebug() << "Solicitud automatica cada 500 ms";
    }

    else if(modo == "1000 ms")
    {
        ui->request->setEnabled(false);

        timer.start(1000);

        qDebug() << "Solicitud automatica cada 1000 ms";
    }

    else if(modo == "2000 ms")
    {
        ui->request->setEnabled(false);

        timer.start(2000);

        qDebug() << "Solicitud automatica cada 2000 ms";
    }
}

void MainWindow::actualizarPuntos()
{
    int cantidad = ui->plot_p->text().toInt();

    if(cantidad <= 0)
    {
        printf("Ingrese una cantidad de puntos valida.\n");
        return;
    }

    limitePuntos = cantidad;

    while(ejeX.size() > limitePuntos)
    {
        ejeX.remove(0);

        for(int i = 0; i < valores.size(); i++)
        {
            valores[i].remove(0);
        }
    }

    for(int i = 0; i < valores.size(); i++)
    {
        ui->plot->graph(i)->setData(
            ejeX, valores[i]
            );
    }

    if(ui->plot->graphCount() > 0 &&
        ejeX.size() > 0)
    {
        ui->plot->rescaleAxes();
    }

    ui->plot->replot();


    printf("Nuevo limite de puntos: %d\n", limitePuntos);
}

void MainWindow::limpiarGrafico()
{
    ejeX.clear();

    valores.clear();

    contadorMuestras = 0;

    direccionGrafico = -1;

    ui->plot->clearGraphs();

    ui->plot->replot();


    printf("Graficos limpiados correctamente.\n");
}
