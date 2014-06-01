#ifndef OPERATION_CODE_H
#define OPERATION_CODE_H

namespace OperationCode {
    enum e{
        OK = 0,
        TUPLE_FULL,
        TUPLE_PATTERN_FULL,
        INDEX_OUT_OF_BOUNDS,
        WRONG_TYPE,
    };
}

#endif // OPERATION_CODE_H
