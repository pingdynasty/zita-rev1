#ifndef __ScaledValue_h__
#define __ScaledValue_h__

/**
 * Linearly scaled value. Scales an input on the range [0, 1] linearly to the range [min, max].
 * Minimum may be less than maximum for inverted scale.
 */
template<typename T>
class LinearValue {
protected:
  T scale;
  T offset;
  T value;
public:
  LinearValue(): scale(1), offset(0), value(0) {}
  LinearValue(const LinearValue<T>& other): scale(other.scale), offset(other.offset), value(other.value) {}
  LinearValue(T minimal, T maximal, T init): value(init) {
    setRange(minimal, maximal);
  }
  void setRange(T minimal, T maximal){
    scale = maximal - minimal;
    offset = minimal;
  }
  void reset(T x){
    value = x;
  }
  T getValue(){
    return value;
  }
  void update(T x){
    value = x * scale + offset;
  }
  T getControl(){
    return (value - offset) / scale;
  }
  /* value cast operator */
  operator T(){
    return getValue();
  }  
  /* assignment operator */
  LinearValue<T>& operator=(const T&& x){
    update(x);
    return *this;
  }
  /* assignment operator */
  LinearValue<T>& operator=(const LinearValue<T>& other){
    scale = other.scale;
    offset = other.offset;
    value = other.value;
    return *this;
  }
};

template class LinearValue<float>;
typedef LinearValue<float> LinearFloat;

/**
 * Exponentially scaled value. Scales an input on the range [0, 1] exponentially to the range [min, max].
 * Minimum may be less than maximum for inverted scale.
 * Note that configured minimum and maximum values must be greater than 0.
 */
template<typename T>
class ExponentialValue {
protected:
  T c;
  T k;
  T y;
public:
  T range = 2;
  ExponentialValue(): c(1), k(2*M_LN2), y(0) {}
  ExponentialValue(const ExponentialValue<T>& other): c(other.c), k(other.k), y(other.y) {}
  ExponentialValue(T minimal, T maximal, T init): y(init) {
    setRange(minimal, maximal);
  }
  void setRange(T minimal, T maximal){
    ASSERT(minimal > 0, "Exponential minimum must be greater than 0");
    c = minimal;
    k = logf(maximal/minimal);
  }
  void reset(T x){
    y = x;
  }
  T getValue(){
    return y;
  }
  void update(T x){
    y = c * expf(k*x);
  }
  T getControl(){
    return logf(y/c)/k;
  }
  /* value cast operator */
  operator T(){
    return getValue();
  }  
  /* assignment operator */
  ExponentialValue<T>& operator=(const T&& x){
    update(x);
    return *this;
  }
  /* assignment operator */
  ExponentialValue<T>& operator=(const ExponentialValue<T>& other){
    c = other.c;
    k = other.k;
    y = other.y;
    return *this;
  }
};

template class ExponentialValue<float>;
typedef ExponentialValue<float> ExponentialFloat;

#endif /* __ScaledValue_h__ */
