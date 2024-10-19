#include "dataserialcom.h"

#include "Base/Communication/serialCom/serialportmanager.h"
#include "Base/Communication/serialCom/modbusprotocolparser.h"
DataSerialCom::DataSerialCom(QObject *parent) : QObject(parent)
{

    SerialPortManager &serialManager = SerialPortManager::getInstance();
    //绑定onDataReceived槽,即当串口接收到数据后台处理
    connect(&serialManager, &SerialPortManager::dataReceived, this, &DataSerialCom::onDataReceived);

    steadyTimer = new QTimer(this);
    connect(steadyTimer,&QTimer::timeout,this,[this](){

        steadyRequest();

    });
    suddLoadTimer = new QTimer(this);
    connect(suddLoadTimer,&QTimer::timeout,this,[this](){

        suddLoadRequest();
    });


}


void DataSerialCom::setFengJiSlot(bool isOpen)
{
    if(isOpen == true){

        qDebug()<<"打开风机！";

        QByteArray dataToSend = QByteArray::fromHex("010600010001");
        controlLoadQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlLoadHexBusy;
        // 如果串口不忙，立即发送
        if (!isControlLoadHexBusy) {
            sendControlLoadHex();
        }


    }else{

        qDebug()<<"关闭风机！";
        QByteArray dataToSend = QByteArray::fromHex("010600010000");
        controlLoadQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlLoadHexBusy;
        // 如果串口不忙，立即发送
        if (!isControlLoadHexBusy) {
            sendControlLoadHex();
        }
    }

}

void DataSerialCom::StartSteadySlot()
{

    qDebug()<<"先切换至逻辑0页";
    pageCodeRequest(0);
    steadyTimer->start(1000);
}


void DataSerialCom::StopSteadySlot()
{
    steadyTimer->stop();
}

void DataSerialCom::pageCodeRequest(int index)
{

    // 发送指定的16进制数据
     qDebug()<<"锁定页面0";
    QByteArray dataToSend = QByteArray::fromHex("011000000001020000");
    SerialPortManager::getInstance().writeData("COM7", dataToSend);
//    controlPanelQueue.enqueue(dataToSend); // 将数据加入队列
//    qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlPanelHexBusy;
//    // 如果串口不忙，立即发送
//    if (!isControlPanelHexBusy) {
//        sendControlPanelHex();
//    }
}

//稳态数据更新页面
void DataSerialCom::onTabChanged(int index)
{
    qDebug()<<"切换到了"<<index<<"页";

    type = index;

    if(!SerialPortManager::getInstance().openPort("COM4", QSerialPort::ReadWrite)){
        return ;
    }

    QStringList requestFramList;
    if(type == TEST_TYPE::STEADY){
        qDebug()<<"稳态数据：读取...0";

    }else if(type == TEST_TYPE::TUNING){
        qDebug()<<"进入整定测试页面,读取...1";

        requestFramList = QStringList({"011000000001020080","011000000001020045",
                                       "010313da0002","010313dc0002","010313de0002","010313e00002",
                                       "010313e20002","010313e40002","010313e60002","010313e80002",
                                       "010313ca0002","010313cc0002","010313ce0002","010313d00002",
                                       "010313d20002","010313d40002","010313d60002","010313d80002"});



    }else{

    }

    for(int i = 0;i<requestFramList.length();i++){

        QString hexString = requestFramList.at(i);  // 获取 QString
        QByteArray byteArray = hexString.toUtf8();  // 将 QString 转换为 QByteArray
        QByteArray dataToSend = QByteArray::fromHex(byteArray);  // 使用 QByteArray::fromHex
        controlPanelQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlPanelHexBusy;
    }

    // 如果串口不忙，立即发送
    if (!isControlPanelHexBusy) {
        qDebug()<<"如果串口不忙，立即发送";
        sendControlPanelHex();
    }else{

         // 弹出一个提示框，告知用户串口正在发送数据请等待...
        qDebug()<<"弹出一个提示框，告知用户串口正在发送数据请等待...";

    }

    requestFramList.clear();
}


void DataSerialCom::steadyRequest()
{


    if (isProcessing) {
        // 如果当前正在处理一轮请求，跳过新的请求
        qDebug() << "上一轮数据尚未处理完毕，跳过本次请求。";

        return ;
    } else {

        isProcessing = true;

        /*
         * 注意：对于只读取仪表示数前，要切换到页面0注意不要锁定页面，再执行请求帧
         * 这里的requestFramList.at(1):读取线路1瞬时电压偏差-
         * 这里的requestFramList.at(2):读取线路1电压恢复时间
        */

        QStringList requestFramList = QStringList( {"0103001e0002","010300200002","010300220002",
                                       "0103002e0002","010300300002","010300320002",
                                       "010300380002","0103003a0002","0103003c0002",
                                       "010300520002","010300540002","010300560002",
                                       "0103002a0002","010300340002","0103003e0002","010300580002",
                                       "010300240002","010300260002","010300280002","0103002c0002"});

        for(int i = 0;i<requestFramList.length();i++){

            QString hexString = requestFramList.at(i);  // 获取 QString
            QByteArray byteArray = hexString.toUtf8();  // 将 QString 转换为 QByteArray
            QByteArray dataToSend = QByteArray::fromHex(byteArray);  // 使用 QByteArray::fromHex
            controlPanelQueue.enqueue(dataToSend); // 将数据加入队列
            qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlPanelHexBusy;
        }

        // 如果串口不忙，立即发送
        if (!isControlPanelHexBusy) {
            qDebug()<<"如果串口不忙，立即发送";
            sendControlPanelHex();
        }else{

             // 弹出一个提示框，告知用户串口正在发送数据请等待...
            qDebug()<<"弹出一个提示框，告知用户串口正在发送数据请等待...";
        }

        requestFramList.clear();
    }



}

void DataSerialCom::suddLoadRequest()
{
    qDebug()<<"突加请求测试";

    //获取曲线的Y坐标值，034c之后每隔2个寄存器可以获取一个浮点数一直到098c
    //获取平均突加电压、平均突加电流和突加频率的Y轴数据
    QStringList requestFramList ={"011000000001020081",
                                  "0103160c0002","0103354c0002","01031c4c0002"//从左向右依次读取：电压、电流和频率
                                  };
    for(int i = 0;i<requestFramList.length();i++){

        QString hexString = requestFramList.at(i);  // 获取 QString
        QByteArray byteArray = hexString.toUtf8();  // 将 QString 转换为 QByteArray
        QByteArray dataToSend = QByteArray::fromHex(byteArray);  // 使用 QByteArray::fromHex
        controlPanelQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlPanelHexBusy;
    }

    // 如果串口不忙，立即发送
    if (!isControlPanelHexBusy) {
        qDebug()<<"如果串口不忙，立即发送";
        sendControlPanelHex();
    }else{

        // 弹出一个提示框，告知用户串口正在发送数据请等待...
        qDebug()<<"弹出一个提示框，告知用户串口正在发送数据请等待...";

    }

    requestFramList.clear();
}

void DataSerialCom::startSuddIncreaseSlot()
{
    qDebug()<<"开始突加测试";

    suddLoadTimer->start(500);


}

void DataSerialCom::stopSuddLoadSlot()
{
    suddLoadTimer->stop();
}


void DataSerialCom::startRecordWaveSlot()
{

    qDebug()<<"启动录波";
    QStringList requestFramList ={"011000000001020031"
                                 };
    for(int i = 0;i<requestFramList.length();i++){

        QString hexString = requestFramList.at(i);  // 获取 QString
        QByteArray byteArray = hexString.toUtf8();  // 将 QString 转换为 QByteArray
        QByteArray dataToSend = QByteArray::fromHex(byteArray);  // 使用 QByteArray::fromHex
        controlPanelQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlPanelHexBusy;
    }

    // 如果串口不忙，立即发送
    if (!isControlPanelHexBusy) {
        qDebug()<<"如果串口不忙，立即发送";
        sendControlPanelHex();
    }else{

        // 弹出一个提示框，告知用户串口正在发送数据请等待...
        qDebug()<<"弹出一个提示框，告知用户串口正在发送数据请等待...";

    }

    requestFramList.clear();
}

void DataSerialCom::readRecordWaveSlot()
{

    qDebug()<<"读取录波数据";


    QStringList requestFramList ={"011000000001020082",
                                "0103000a0002","0103000b0002","0103000c0002"//从左向右依次读取
                                 };
    for(int i = 0;i<requestFramList.length();i++){

        QString hexString = requestFramList.at(i);  // 获取 QString
        QByteArray byteArray = hexString.toUtf8();  // 将 QString 转换为 QByteArray
        QByteArray dataToSend = QByteArray::fromHex(byteArray);  // 使用 QByteArray::fromHex
        controlPanelQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isControlPanelHexBusy;
    }

    // 如果串口不忙，立即发送
    if (!isControlPanelHexBusy) {
        qDebug()<<"如果串口不忙，立即发送";
        sendControlPanelHex();
    }else{

        // 弹出一个提示框，告知用户串口正在发送数据请等待...
        qDebug()<<"弹出一个提示框，告知用户串口正在发送数据请等待...";

    }

    requestFramList.clear();
}

void DataSerialCom::sendControlPanelHex()
{
    qDebug()<<"********************队列中数据个数:"<<controlPanelQueue.length()<<"*******************";
    if (!controlPanelQueue.isEmpty()) {
        QByteArray nextData = controlPanelQueue.dequeue(); // 从队列中取出数据
        isControlPanelHexBusy = true; // 标记为忙碌
        if(SerialPortManager::getInstance().writeData("COM7", nextData)){



            qDebug()<<"写数据执行成功Yes";
        }else{

            qDebug()<<"长时间未写入数据,执行失败No..........";
        }
    }else{
        // 队列中的所有数据已经发送完毕，设置为处理完成状态
        qDebug() << "队列中的数据全部发送完毕！";
        isProcessing = false;  // 标记处理完成，允许下一轮数据处理

        if(type == TEST_TYPE::STEADY){
            qDebug()<<"是稳态页面...................................................................................................................";
            emit updateSteadyPageSignal(dataStrQueue);
        }else if(type == TEST_TYPE::SUDD_LOAD){
            qDebug()<<"发送突加更新界面信号！！！";
            emit updateSuddLoadPageSignal(dataStrQueue);

        }

        dataStrQueue.clear();

    }
}


void DataSerialCom::sendControlLoadHex()
{
    qDebug()<<"********************队列中数据个数:"<<controlPanelQueue.length()<<"*******************";
    if (!controlLoadQueue.isEmpty()) {
        QByteArray nextData = controlLoadQueue.dequeue(); // 从队列中取出数据
        isControlLoadHexBusy = true; // 标记为忙碌
        if(SerialPortManager::getInstance().writeData("COM5", nextData)){
            qDebug()<<"写数据执行成功Yes";
        }else{
            qDebug()<<"长时间未写入数据,执行失败No..........";
        }
    }else{
        // 队列中的所有数据已经发送完毕，设置为处理完成状态
        qDebug() << "队列中的数据全部发送完毕！";
        isProcessing = false;  // 标记处理完成，允许下一轮数据处理
        dataStrQueue.clear();

    }
}


void DataSerialCom::onDataReceived(const QSerialPort &port, const QByteArray &data)
{

    // 使用 QSerialPortInfo 来获取串口描述
    QSerialPortInfo portInfo(port.portName());

    if(portInfo.isValid()){

        QString portDescription = portInfo.description();
         // 仪表返回到主机缓存的数据
        qDebug() << "Data received on port" << port.portName() <<"串口描述:"<<portInfo.description()<< "串口中数据:" << data.toHex();
        //解析缓存的响应帧数据
        analyzingData(data);


        portName = portInfo.portName();

        qDebug()<<"portName"<<portName;

        if(portInfo.portName() == "COM5"){

            // 标记为负载控制不忙碌
            isControlLoadHexBusy = false;
            // 继续发送队列中的下一个数据
            sendControlLoadHex();

        }else if(portInfo.portName() == "COM7"){

            // 标记仪表控制为不忙碌
            isControlPanelHexBusy = false;
            // 继续发送队列中的下一个数据
            sendControlPanelHex();
        }
    }



}


void DataSerialCom::analyzingData(const QByteArray &data)
{

    //注意：显示界面数据时，用于切换页面的响应帧不需要解析,而只需要解析查询数据的响应帧
    // 提取功能码(值得注意的是：异常响应中"功能码"的最高位会设置为 1，表示错误响应,)
    uint8_t functionCode = static_cast<uint8_t>(data.at(1));
    if (functionCode >= 0x01 && functionCode <= 0x04){

        ModbusProtocolParser parser;
        QByteArray tempData = data;
        if(parser.parseReponse(tempData)){

            QByteArray dataField = parser.getDataField();
            qDebug()<<"成功analyzingData返回响应帧的数据域:"<<dataField.toHex();
            QString dataStr = QString::number(parser.toFloatData(dataField),'f',3);
            qDebug()<<"dataStr"<<dataStr;


            dataStrQueue.enqueue(dataStr);


        }else{
            qDebug()<<"数据域解析失败！！！！";

            stopSuddLoadSlot();

        }

    }else{
        qDebug()<<"写的响应帧不需要解析";
    }


}
