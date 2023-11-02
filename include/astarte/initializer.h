#ifndef _INITIALIZER_H_
#define _INITIALIZER_H_

#include "legion.h"
#include "parallel_tensor.h"

namespace astarte {

class CAModel;

class Initializer {
public:
    Initializer(void);
    virtual ~Initializer(void);
    virtual void init(CAModel const *ff, const ParallelTensor p) = 0;
};

class GlorotUniform : public Initializer {
public:
    GlorotUniform(int _seed);
    ~GlorotUniform(void);
    void init(CAModel const *ff, const ParallelTensor p);
    static void init_task(Legion::Task const *task,
                          std::vector<Legion::PhysicalRegion> const &regions,
                          Legion::Context ctx,
                          Legion::Runtime *runtime);
    int seed;
    float scale;
    DataType data_type;
};

class Op;
struct ZeroInitMeta {
    static int const MAX_NUM_REGIONS = 64;
    int num_regions;
    Op *op_ptr;
    DataType data_types[MAX_NUM_REGIONS];
};

class ZeroInitializer : public Initializer {
public:
    ZeroInitializer(void);
    ~ZeroInitializer(void);
    void init(CAConfig const *ff, const ParallelTensor p);
    static void init_task(Legion::Task const *task,
                          std::vector<Legion::PhysicalRegion> const &regions,
                          Legion::Context ctx,
                          Legion::Runtime *runtime);
};

class UniformInitializer : public Initializer {
public:
    UniformInitializer(int _seed, float _min, float _max);
    ~UniformInitializer(void);
    void init(CAModel const *ff, const ParallelTensor p);
    static void init_mask(Legion::Task const *task,
                          std::vector<Legion::PhysicalRegion> const &regions,
                          Legion::Context ctx,
                          Legion::Runtime *runtime);
    int seed;
    float min_val, max_val,
    DataType data_type;
};

class NormInitializer : public Initializer {
public:
    NormInitializer(int _seed, float _mean, float _stddev);
    ~NormInitializer(void);
    void init(CAModel const *ff, const ParallelTensor p);
    static void init_task(Legion::Task const *task,
                          std::vector<Legion::PhysicalRegion> const &regions,
                          Legion::Context ctx,
                          Legion::Runtime *runtime);
    int seed;
    float mean, stddev;
    DataType data_type;
};

class ConstantInitializer : public Initializer {
public:
    ConstantInitializer(float _value);
    ConstantInitializer(int64_t _value);
    ConstantInitializer(int _value);
    ~ConstantInitializer(void);
    void init(CAModel const *ff, const ParallelTensor p);
    static void init_task(Legion::Task const *task,
                          std::vector<Legion::PhysicalRegion> const &regions,
                          Legion::Context ctx,
                          Legion::Runtime *runtime);
    static void init_task_cpu(Legion::Task const *task,
                              std::vector<Legion::PhysicalRegion> const &regions,
                              Legion::Context ctx,
                              Legion::Runtime *runtime);
public:
    DataType data_type;
    float float_value;
    int64_t int64_value;
    int int32_value;
};

}; // namespace astarte
#endif // _INITIALIZER_H_