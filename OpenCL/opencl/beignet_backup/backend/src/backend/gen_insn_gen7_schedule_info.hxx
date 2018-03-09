//                 Family     Latency     SIMD16     SIMD8
DECL_GEN7_SCHEDULE(Label,           0,         0,        0)
DECL_GEN7_SCHEDULE(Unary,           20,        4,        2)
DECL_GEN7_SCHEDULE(UnaryWithTemp,   20,        40,      20)
DECL_GEN7_SCHEDULE(Binary,          20,        4,        2)
DECL_GEN7_SCHEDULE(SimdShuffle,     20,        4,        2)
DECL_GEN7_SCHEDULE(BinaryWithTemp,  20,        40,      20)
DECL_GEN7_SCHEDULE(Ternary,         20,        4,        2)
DECL_GEN7_SCHEDULE(I64Shift,        20,        40,      20)
DECL_GEN7_SCHEDULE(I64HADD,         20,        40,      20)
DECL_GEN7_SCHEDULE(I64RHADD,        20,        40,      20)
DECL_GEN7_SCHEDULE(I64ToFloat,      20,        40,      20)
DECL_GEN7_SCHEDULE(FloatToI64,      20,        40,      20)
DECL_GEN7_SCHEDULE(I64MULHI,        20,        40,      20)
DECL_GEN7_SCHEDULE(I64MADSAT,       20,        40,      20)
DECL_GEN7_SCHEDULE(Compare,         20,        4,        2)
DECL_GEN7_SCHEDULE(I64Compare,      20,        80,      20)
DECL_GEN7_SCHEDULE(I64DIVREM,       20,        80,      20)
DECL_GEN7_SCHEDULE(Jump,            14,        1,        1)
DECL_GEN7_SCHEDULE(IndirectMove,    20,        2,        2)
DECL_GEN7_SCHEDULE(Eot,             20,        1,        1)
DECL_GEN7_SCHEDULE(NoOp,            20,        2,        2)
DECL_GEN7_SCHEDULE(Wait,            20,        2,        2)
DECL_GEN7_SCHEDULE(Math,            20,        4,        2)
DECL_GEN7_SCHEDULE(Barrier,         80,        1,        1)
DECL_GEN7_SCHEDULE(Fence,           80,        1,        1)
DECL_GEN7_SCHEDULE(Read64,          80,        1,        1)
DECL_GEN7_SCHEDULE(Write64,         80,        1,        1)
DECL_GEN7_SCHEDULE(Read64A64,       80,        1,        1)
DECL_GEN7_SCHEDULE(Write64A64,      80,        1,        1)
DECL_GEN7_SCHEDULE(UntypedRead,     160,       1,        1)
DECL_GEN7_SCHEDULE(UntypedWrite,    160,       1,        1)
DECL_GEN7_SCHEDULE(UntypedReadA64,  160,       1,        1)
DECL_GEN7_SCHEDULE(UntypedWriteA64, 160,       1,        1)
DECL_GEN7_SCHEDULE(ByteGatherA64,   160,       1,        1)
DECL_GEN7_SCHEDULE(ByteScatterA64,  160,       1,        1)
DECL_GEN7_SCHEDULE(ByteGather,      160,       1,        1)
DECL_GEN7_SCHEDULE(ByteScatter,     160,       1,        1)
DECL_GEN7_SCHEDULE(DWordGather,     160,       1,        1)
DECL_GEN7_SCHEDULE(PackByte,        40,        1,        1)
DECL_GEN7_SCHEDULE(UnpackByte,      40,        1,        1)
DECL_GEN7_SCHEDULE(PackLong,        40,        1,        1)
DECL_GEN7_SCHEDULE(UnpackLong,      40,        1,        1)
DECL_GEN7_SCHEDULE(Sample,          160,       1,        1)
DECL_GEN7_SCHEDULE(Vme,             320,       1,        1)
DECL_GEN7_SCHEDULE(Ime,             320,       1,        1)
DECL_GEN7_SCHEDULE(TypedWrite,      80,        1,        1)
DECL_GEN7_SCHEDULE(SpillReg,        20,        1,        1)
DECL_GEN7_SCHEDULE(UnSpillReg,      160,       1,        1)
DECL_GEN7_SCHEDULE(Atomic,          80,        1,        1)
DECL_GEN7_SCHEDULE(AtomicA64,       80,        1,        1)
DECL_GEN7_SCHEDULE(I64MUL,          20,        40,      20)
DECL_GEN7_SCHEDULE(I64SATADD,       20,        40,      20)
DECL_GEN7_SCHEDULE(I64SATSUB,       20,        40,      20)
DECL_GEN7_SCHEDULE(F64DIV,          20,        40,      20)
DECL_GEN7_SCHEDULE(CalcTimestamp,   80,        1,        1)
DECL_GEN7_SCHEDULE(StoreProfiling,  80,        1,        1)
DECL_GEN7_SCHEDULE(WorkGroupOp,     80,        1,        1)
DECL_GEN7_SCHEDULE(SubGroupOp,      80,        1,        1)
DECL_GEN7_SCHEDULE(Printf,          80,        1,        1)
DECL_GEN7_SCHEDULE(OBRead,          80,        1,        1)
DECL_GEN7_SCHEDULE(OBWrite,         80,        1,        1)
DECL_GEN7_SCHEDULE(MBRead,          80,        1,        1)
DECL_GEN7_SCHEDULE(MBWrite,         80,        1,        1)