#include "neuron.h"

Neuron::Neuron(int t) : threshold(t){}

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

Associative::Associative(int edges, int weight) : Neuron(qrand() % 3 - 1)
{
    // задание случайного порога
//    this->threshold = qrand() % 3 - 1;
    for (int i = 0; i < edges; ++i)
    // инициализация весов значениями weight
        this->links << weight;
}

void Associative::setLinkWeight(int idx, int weight)
{
    this->links[idx] = weight;
}

Resulting::Resulting(int t) : Neuron(t){}

Layer::Layer(LayerType type, int cnt, int out)
{
    this->type = type;
    switch (type) {
    case SensorType:
        for(int i = 0; i < cnt; ++i)
            this->neurons << new Sensor(out);
        break;
    case AssociativeType:
        for(int i = 0; i < cnt; ++i)
            this->neurons << new Associative(out,/*qrand() % 3 - */1);
        break;
    case ResultingType:
        for(int i = 0; i < cnt; ++i)
            this->neurons << new Resulting(0);
        break;
    default:
        break;
    }
}

void Layer::setValue(int val)
{
    foreach (Neuron* n, this->neurons) {
        n->setValue(val);
    }
}
