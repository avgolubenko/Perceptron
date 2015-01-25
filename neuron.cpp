#include "neuron.h"

Neuron::Neuron() : threshold(0){}

Neuron::~Neuron(){}

int Neuron::thresholdFunc() const
{
    return this->value >= this->threshold ? 1 : 0;
}

void Neuron::setValue(int val)
{
    this->value = val;
}

void Neuron::incValue()
{
    this->addValue(1);
}

void Neuron::addValue(int n)
{
    this->value += n;
}

int Neuron::getValue() const
{
    return this->value;
}

QList<int> Neuron::getLinks() const
{
    return this->links;
}


Sensor::Sensor(int edges) : Neuron()
{
    for(int i = 0; i < edges; ++i)
    // инициализация весов S-A в интервале [-1,1]
        this->links << qrand() % 3 - 1;
}


Associative::Associative(int edges) : Neuron()
{
    // задание случайного порога
    this->threshold = qrand() % 3 - 1;
    for (int i = 0; i < edges; ++i)
    // инициализация весов A-R 0-значениями
        this->links << 0;
}

void Associative::setLinkWeight(int idx, int weight)
{
    this->links[idx] = weight;
}
