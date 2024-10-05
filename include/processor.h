#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // DONE: See src/processor.cpp

  // DONE: Declare any necessary private members
 private:
  float prevIdle{0.0};
  float prevTotal{0.0};
};

#endif