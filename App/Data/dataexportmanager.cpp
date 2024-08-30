#include "dataexportmanager.h"

DataExportManager::DataExportManager(QObject *parent) : QObject(parent)
{

}

void DataExportManager::createStaticTestExcel()
{
    QXlsx::Document xlsx;

    // 第一部分：生成第一个表格
    xlsx.mergeCells("A1:N1");
    QXlsx::Format titleFormat;
    titleFormat.setFontSize(16);
    titleFormat.setFontBold(true);
    titleFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    xlsx.write("A1", "电流、电压和频率数据", titleFormat);

    // 设置列宽
    for (int col = 1; col <= 14; ++col) {
        xlsx.setColumnWidth(col, 12);
    }

    // 第一行表头
    QXlsx::Format headerFormat;
    headerFormat.setFontBold(true);
    headerFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    headerFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    headerFormat.setTextWarp(true);
    headerFormat.setBorderStyle(QXlsx::Format::BorderThin);

    xlsx.mergeCells("A2:C2", headerFormat);
    xlsx.write("A2", "负载状况 (%)", headerFormat);

    xlsx.mergeCells("D2:F2", headerFormat);
    xlsx.write("D2", "电流 (A)", headerFormat);

    xlsx.mergeCells("G2:L2", headerFormat);
    xlsx.write("G2", "电压 (V)", headerFormat);

    xlsx.mergeCells("M2:N2", headerFormat);
    xlsx.write("M2", "频率 (Hz)", headerFormat);

    // 第二行表头
    xlsx.write("A3", "负载状况 (%)", headerFormat);
    xlsx.write("B3", "有功功率 (KW)", headerFormat);
    xlsx.write("C3", "功率因数 PF", headerFormat);
    xlsx.write("D3", "Ia", headerFormat);
    xlsx.write("E3", "Ib", headerFormat);
    xlsx.write("F3", "Ic", headerFormat);
    xlsx.write("G3", "Uan", headerFormat);
    xlsx.write("H3", "Ubn", headerFormat);
    xlsx.write("I3", "Ucn", headerFormat);
    xlsx.write("J3", "Uab", headerFormat);
    xlsx.write("K3", "Ubc", headerFormat);
    xlsx.write("L3", "Uca", headerFormat);
    xlsx.write("M3", "farb", headerFormat);
    xlsx.write("N3", "fmax", headerFormat);

    // 设置数据行
    QXlsx::Format dataFormat;
    dataFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    dataFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    dataFormat.setBorderStyle(QXlsx::Format::BorderThin);

    QStringList loadConditions = {"0", "25", "50", "75", "100", "75", "50", "25", "0"};
    for (int row = 4; row <= 12; ++row) {
        //第4行至第12行，的第一列填上loadConditions的每一个元素
        xlsx.write(row, 1, loadConditions[row - 4], dataFormat);
        for (int col = 2; col <= 14; ++col) {
            //第4行至第12行，的其他列填上空字符串
            xlsx.write(row, col, "", dataFormat);
        }
    }

    // 第二部分：生成第二个表格
    int startRow = 14; // 第二个表格从第14行开始

    xlsx.mergeCells(QString("A%1:J%1").arg(startRow)); // 标题合并单元格
    xlsx.write(QString("A%1").arg(startRow), "稳态电压和频率调整数据", titleFormat);

    startRow += 2;

    // 第一行表头
    xlsx.mergeCells(QString("A%1:A%2").arg(startRow).arg(startRow + 1), headerFormat);
    xlsx.write(QString("A%1").arg(startRow), "负载状况 (%)", headerFormat);

    QStringList conditions = {"0", "25", "50", "75", "100", "75", "50", "25", "0"};
    for (int col = 2; col <= 10; ++col) {
        xlsx.write(startRow, col, conditions[col - 2], headerFormat);
    }

    // 第二行数据标签
    QStringList labels = {"稳态电压调整系数δU (%)", "电压波动率δUb (%)", "稳态频率调整系数δf (%)", "频率波动率δf (%)"};
    for (int row = startRow + 1; row <= startRow + 4; ++row) {
        xlsx.write(row, 1, labels[row - startRow - 1], headerFormat);
        for (int col = 2; col <= 10; ++col) {
            xlsx.write(row, col, "", dataFormat);
        }
    }

    // 保存文件到桌面
    QString filePath = QDir::homePath() + "/Desktop/StaticTestReport.xlsx";
    if (xlsx.saveAs(filePath)) {
        qDebug() << "Excel report with two tables created and saved at:" << filePath;
    } else {
        qDebug() << "Failed to save the Excel report.";
    }
}
