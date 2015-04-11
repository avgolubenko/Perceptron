#ifndef NEURON_H
#define NEURON_H

#include <QList>

class Neuron
{
public:    
    virtual ~Neuron();
    //  Пороговая функция
    virtual int thresholdFunc() const;
    //
    virtual void setValue(int val);
    // увеличение value на 1
    virtual void incValue();
    // увеличение value на n
    virtual void addValue(int n);
    // геттер для value
    virtual int getValue() const;
    // геттер для links
    virtual QList<int> getLinks() const;
protected:
    Neuron(int t = 0);
    // значение нейрона
    int value;
    // порог для передаточной ф-и
    int threshold;
    // веса синапсов
    QList<int> links;
};

class Sensor : public Neuron
{
public:
    Sensor(int edges);
};

class Associative : public Neuron
{
public:
    Associative(int edges, int weight);
    void setLinkWeight(int idx, int weight);
};

class Resulting : public Neuron
{
public:
    Resulting(int t);
};

enum LayerType{
    SensorType,
    AssociativeType,
    ResultingType
};

class Layer
{
public:
    Layer(LayerType type, int cnt, int out);
    // установить значение val для всех нейронов слоя
    void setValue(int val);
    QList<Neuron*> neurons;
private:
    LayerType type;
};

#endif // NEURON_H
