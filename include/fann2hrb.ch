
#define	FANN_LINEAR                      0
#define	FANN_THRESHOLD                   1
#define	FANN_THRESHOLD_SYMMETRIC         2
#define	FANN_SIGMOID                     3
#define	FANN_SIGMOID_STEPWISE            4
#define	FANN_SIGMOID_SYMMETRIC           5
#define	FANN_SIGMOID_SYMMETRIC_STEPWISE  6
#define	FANN_GAUSSIAN                    7
#define	FANN_GAUSSIAN_SYMMETRIC          8
	/* Stepwise linear approximation to gaussian.
	 * Faster than gaussian but a bit less precise.
	 * NOT implemented yet.
	 */
#define	FANN_GAUSSIAN_STEPWISE           9
#define	FANN_ELLIOT                     10
#define	FANN_ELLIOT_SYMMETRIC           11
#define	FANN_LINEAR_PIECE               12
#define	FANN_LINEAR_PIECE_SYMMETRIC     13
#define	FANN_SIN_SYMMETRIC              14
#define	FANN_COS_SYMMETRIC              15
#define	FANN_SIN                        16
#define	FANN_COS                        17

#define FANN_STOPFUNC_MSE                0
#define FANN_STOPFUNC_BIT                1

#define FANN_TRAIN_INCREMENTAL           0
#define FANN_TRAIN_BATCH                 1
#define FANN_TRAIN_RPROP                 2
#define FANN_TRAIN_QUICKPROP             3
#define FANN_TRAIN_SARPROP               4
