#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include "neuron.h"
#include <QImage>

struct Pattern
{
    QImage img;
    int type;
};


class Perceptron
{
public:
    // in - кол-во сенсоров, out - кол-во выходов
    Perceptron(int in, int out);
    ~Perceptron();
    //
    void train(const QList<Pattern> trSet);
    //
    void overestimateAR(int rIdx, int value);
    //
    int recognize(const QImage& image);
private:
    // слой сенсоров
    QList<Sensor> sensors;
    // промежуточный слой
    QList<Associative*> associatives;
    // результирующий слой
    QList<Resulting> resultings;
};

#endif // PERCEPTRON_H
