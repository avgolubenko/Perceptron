#include "perceptron.h"

#include <QColor>

Perceptron::Perceptron(int in, int nHidden, int nH_Neurons, int out)
{
    // создание S слоя со связями S-A
    for (int j = 0; j < in; ++j)
        this->sensors << Sensor(nH_Neurons);        
    // жуть 0...
//    this->layers << Layer(SensorType,in,nH_Neurons);
    for(int i = 0; i < nHidden - 1; ++i)
        this->layers << Layer(AssociativeType,nH_Neurons,nH_Neurons);
    this->layers << Layer(AssociativeType,nH_Neurons,out);
//    this->layers << Layer(ResultingType,out,0);
    // создание R слоя
    for (int j = 0; j < out; ++j)
        this->resultings << Resulting(0);
}

Perceptron::~Perceptron(){}

void Perceptron::train(const QList<Pattern> trSet)
{
    foreach (Pattern pat, trSet) {
        // предъявление образа
        this->recognize(pat.img);
        // вычисление реакций нейронов A слоя
        this->calcReactionA();
        //
        int actual;
        do {
            // вычисление реакций нейронов R слоя
            this->calcReactionR();
            //
            actual = this->argMaxR();
            //  правила Хебба
            if(pat.type != actual){
                this->overestimateAR(pat.type,1); // pat.type - actual?
                this->overestimateAR(actual,-1);
            }
        }while(pat.type != actual);
    }
}

void Perceptron::overestimateAR(int rIdx, int value)
{
    foreach (Neuron* n, this->layers.back().neurons) {
        if(n->thresholdFunc() == 1)
        {
            int wht = n->getLinks()[rIdx];
            dynamic_cast<Associative*>(n)->setLinkWeight(rIdx, wht + value);
        }
    }
}
// жуть 1...
void Perceptron::calcReactionA()
{
    // обнуление значений нейронов A слоя
    foreach (Neuron *aNeuro, this->layers.first().neurons) {
        aNeuro->setValue(0);
    }
    // проход по всем сенсорам
    foreach (Sensor sNeuro, this->sensors) {
        // проход по всем соответствующим нейрону выходам в A слой
        for(int j = 0; j < sNeuro.getLinks().size(); j++)
            // вычисление значения на входе нейрона A
            this->layers.first().neurons[j]->addValue(sNeuro.getValue() * sNeuro.getLinks()[j]);
    }
    //
    for(int i = 1; i < this->layers.size(); ++i)
    {
        // обнуление значений нейронов текущего слоя
        layers[i].setValue(0);
        // проход по всем нейронам предыдущего слоя
        foreach (Neuron* n, this->layers.at(i-1).neurons) {
            // проход по всем соответствующим нейрону выходам в A слой
            for(int j = 0; j < n->getLinks().size(); j++)
                // вычисление значения на входе нейрона A
                layers[i].neurons[j]->addValue(n->thresholdFunc() * n->getLinks()[j]);
        }
    }
}

void Perceptron::calcReactionR()
{
    // обнуление значений нейронов R слоя
    for(int i = 0; i < this->resultings.size(); i++)
        this->resultings[i].setValue(0);    
    // проход по всем нейронам A слоя
    foreach(Neuron *aNeuro, this->layers.back().neurons) {
        // проход по всем соответствующим нейрону выходам в R слой
        for(int j = 0; j < aNeuro->getLinks().size(); j++)
            // вычисление значения на входе нейрона R
            this->resultings[j].addValue(aNeuro->thresholdFunc() * aNeuro->getLinks()[j]);
    }
}

int Perceptron::argMaxR() const
{
    int max_idx = 0; // индекс нейрона-"победителя"
    // поиск max отклика
    for(int i = 1; i < this->resultings.size(); ++i)
        if(resultings[i].getValue() > resultings[max_idx].getValue())
            max_idx = i;
    return max_idx;
}

int Perceptron::classify(const QImage &image)
{
    this->recognize(image);
    //
    this->calcReactionA();    
    //
    this->calcReactionR();
    //
    return this->argMaxR();
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
