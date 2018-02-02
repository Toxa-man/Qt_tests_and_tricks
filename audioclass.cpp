#include "audioclass.h"

AudioClass::AudioClass(QWidget *parent)
    : QWidget(parent)
{
    inputBuffer.open(QBuffer::ReadWrite);
    outputBuffer.open(QBuffer::ReadWrite);
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
     if (!info.isFormatSupported(format)) {
         qWarning() << "Default format not supported, trying to use the nearest.";
         format = info.nearestFormat(format);
     }
    input = new QAudioInput(format, this);
    format.setSampleRate(8000);
    output = new QAudioOutput(format, this);

    connect(output, &QAudioOutput::stateChanged, [=](QAudio::State state){
        qDebug() << "error output: " << output->error();
        qDebug() <<"output: "  << state;

    });

    connect(input, &QAudioInput::stateChanged, [=](QAudio::State state){
        qDebug() << "error input: " << input->error();
        qDebug() <<"input: "  << state;
    });
    output->start(&outputBuffer);
    qDebug() << "period size: " << output->periodSize();
    input->start(&inputBuffer);
    qDebug() << "buffer size: " << output->bufferSize();
    QTimer::singleShot(500, Qt::PreciseTimer, [=](){
    });
    connect(&inputBuffer, &QBuffer::readyRead, [=](){

        outputBuffer.buffer().resize(inputBuffer.buffer().size());

        memcpy(outputBuffer.buffer().data(), inputBuffer.buffer().constData(), outputBuffer.buffer().size());
        outputBuffer.seek(0);
        if (output->state() == QAudio::IdleState){
            output->stop();
            output->start(&outputBuffer);
        }
        inputBuffer.buffer().clear();
        inputBuffer.seek(0);


//        qDebug() << "------------------";
//        qDebug() << "input size: " << inputBuffer.size();
//        qDebug() << "output size: " << outputBuffer.size();
    });

}

AudioClass::~AudioClass()
{
    input->stop();
    output->stop();
    outputBuffer.close();
    inputBuffer.close();
}
