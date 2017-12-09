#ifndef AUDIOCLASS_H
#define AUDIOCLASS_H

#include <QWidget>
#include <QtCore>
#include <QAudioBuffer>
#include <QAudioInput>
#include <QAudioOutput>


class AudioClass : public QWidget
{
    Q_OBJECT

public:
    AudioClass(QWidget *parent = 0);
    ~AudioClass();
private:
    QAudioInput* input;
    QAudioOutput* output;
    QBuffer inputBuffer;
    QBuffer outputBuffer;
};

#endif // AUDIOCLASS_H
