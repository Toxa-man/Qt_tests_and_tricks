#include "audioclass.h"

AudioClass::AudioClass(QWidget *parent)
    : QWidget(parent)
{
    inputBuffer.open(QBuffer::ReadWrite);
    outputBuffer.open(QBuffer::ReadOnly);
    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    qDebug() << info.deviceName();
         if (!info.isFormatSupported(format)) {
             qWarning() << "Default format not supported, trying to use the nearest.";
             format = info.nearestFormat(format);
         }
    input = new QAudioInput(format, this);
    output = new QAudioOutput(format, this);
    input->reset();
    input->start(&inputBuffer);
    input->resume();
    output->start(&outputBuffer);
    connect(&inputBuffer, &QBuffer::readyRead, [=](){
        outputBuffer.buffer().resize(inputBuffer.buffer().size());
        memcpy(outputBuffer.buffer().data(), inputBuffer.buffer().constData(), outputBuffer.buffer().size());
//        inputBuffer.buffer().remove(0, inputBuffer.buffer().size());
//        qDebug() << "output size: " << outputBuffer.buffer().size();
//        qDebug() << "input size: " << inputBuffer.buffer().size();

    });

    connect(output, &QAudioOutput::stateChanged, [=](QAudio::State state){
        qDebug() << input->error();
        qDebug() <<"output: "  << state;
    });

    connect(input, &QAudioInput::stateChanged, [=](QAudio::State state){
        qDebug() <<"input: "  << state;
    });
}

AudioClass::~AudioClass()
{
    input->stop();
    output->stop();
    outputBuffer.close();
    inputBuffer.close();
}
