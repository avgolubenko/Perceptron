#include "perceptron.h"

#include <QColor>

Perceptron::Perceptron(int in, int out)
{
    // кол-во нейронов в A слое
    int assoc = in * 2;
    //
    for (int j = 0; j < in; ++j)
        this->sensors << Sensor(assoc);
    for (int j = 0; j < assoc; ++j)
        this->associatives << new Associative(out);
    for (int j = 0; j < out; ++j)
        this->resultings << Resulting();
}

Perceptron::~Perceptron(){}

void Perceptron::train(const QList<Pattern> trSet)
{
    foreach (Pattern pat, trSet) {
        this->recognize(pat.img);
        //
        this->calcReactionA();
        //
        if(pat.type)
            this->overestimateAR(0,-1);
        else
            this->overestimateAR(0,1);
    }
}

void Perceptron::overestimateAR(int rIdx, int value)
{
    foreach (Associative* aNeuro, this->associatives) {
        if(aNeuro->thresholdFunc() == 1)
        {
            int wht = aNeuro->getLinks()[rIdx];
            aNeuro->setLinkWeight(rIdx, wht + value);
        }
    }
}

void Perceptron::calcReactionA()
{
    // обнуление значений нейронов A
    for(int i = 0; i < this->associatives.size(); i++)
        this->associatives[i]->setValue(0);
    //
    for(int i = 0; i < this->sensors.size(); i++) // проход по всем сенсорам
        // проход по всем соответствующим нейронам A
        for(int j = 0; j < this->sensors[i].getLinks().size(); j++)
            this->associatives[j]->addValue(this->sensors[i].getValue() * this->sensors[i].getLinks()[j]);
}

int Perceptron::classify(const QImage &image)
{
    this->recognize(image);
    //
    this->calcReactionA();
    // обнуление значений нейронов R слоя
    for(int i = 0; i < this->resultings.size(); i++)
        this->resultings[i].setValue(0);
    // пересчёт значений  результирующего слоя
    for(int i = 0; i < this->associatives.size(); i++)
        for(int j = 0; j < this->associatives[i]->getLinks().size(); j++)
            this->resultings[j].addValue(
                    this->associatives[i]->thresholdFunc() * this->associatives[i]->getLinks()[j]);
    //
    return this->resultings[0].thresholdFunc();
}

void Perceptron::recognize(const QImage &image)
{
    const int cm = image.width();
    const int cn = image.height();
    // Инициализация сенсоров S
    for(int i = 0; i < cn; i++)
    {
        // вычисление значения строкового сенсора
        this->sensors[i].setValue(0);
        for(int j = 0; j < cm; j++)
            if(QColor(image.pixel(j, i)) == QColor(Qt::black))
                this->sensors[i].incValue();
    }
    for(int i = cn; i < cn + cm; i++)
    {
        // вычисление значения столбцового сенсора
        this->sensors[i].setValue(0);
        for (int j = 0; j < cn; j++)
            if(QColor(image.pixel(i - cn, j)) == QColor(Qt::black))
                this->sensors[i].incValue();
    }
}


Pattern::Pattern(const QImage &image, int type) : img(image), type(type){}
