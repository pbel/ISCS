
//#ifndef _CONSTANTS_H
//#define _CONSTANTS_H

// Global constants

double		STEP_SIZE			= 0.05;
// Controls the speed vs. accuracy of the simulator:
// A larger step size makes the simulator to run faster,
// but less stably;
// A smaller step size causes the simulator to run slower,
// but more stably.


// Environment-related constants

int		MAX_ENVIRONMENTS		= 10;

int		MAX_OTHER_OBJECTS		= 10;

int		SELECTION_LEVEL_OBJECT		= 0;
int		SELECTION_LEVEL_ENVIRONMENT	= 1;
int		SELECTION_LEVEL_ENVS		= 2;

// Optimizer-related constants

int		STARTING_EVALUATION_TIME	= 500;
double		MUTATION_PROBABILITY		= 1.0;

int		EVALS_UNTIL_EVAL_TIME_EXTENDED	= 30;
int		EVAL_TIME_EXTENSION_PERIOD	= 10;

double		WORST_FITNESS			= -100.0;

int		MAX_EVALS_BEFORE_SAVING		= 100;

// Constants related to viewing the simulation

int		MODE_VIEW_SIDE			= 0;
int		MODE_VIEW_TOP			= 1;
int		MODE_VIEW_BACK			= 2;

int		MODE_SIMULATE_DESIGN		= 0;
int		MODE_SIMULATE_EVOLVE		= 1;
int		MODE_SIMULATE_CHAMP		= 2;

double		MOVE_INCREMENT			= 0.1;

// Object constants

int		SHAPE_RECTANGLE			= 0;
int		SHAPE_CYLINDER			= 1;
int		SHAPE_SPHERE			= 2;

int		OBJECT_STATE_INCORPOREAL	= 0;
int		OBJECT_STATE_SOLID		= 1;
int		OBJECT_STATE_PHYSICAL		= 2;

int		OBJECT_FACE_FRONT		= 0;
int		OBJECT_FACE_BACK		= 1;
int		OBJECT_FACE_LEFT		= 2;
int		OBJECT_FACE_RIGHT		= 3;
int		OBJECT_FACE_TOP			= 4;
int		OBJECT_FACE_BOTTOM		= 5;


// Light source constants

double		LIGHT_SOURCE_LENGTH		= 0.5;
double		LIGHT_SOURCE_DISTANCE		= 5.0;

double		MAX_LIGHT_SENSOR_DISTANCE	= 10;

// Robot-specific constants

int		MAX_ROBOTS			= 2;

int		ROBOT_STARFISH			= 0;
int		ROBOT_SNAKE			= 1;
int		ROBOT_QUADRUPED			= 2;

double		ROBOT_STARFISH_BODY_LENGTH	= 1.0;
double		ROBOT_STARFISH_BODY_WIDTH	= 1.0;
double		ROBOT_STARFISH_BODY_HEIGHT	= 0.1;
double		ROBOT_STARFISH_LEG_RADIUS	= 0.1;
double		ROBOT_STARFISH_LEG_LENGTH	= 1.0;

double		ROBOT_STARFISH_JOINT_RANGE	= 60.0;
double		ROBOT_STARFISH_MOTOR_STRENGTH	= 3.0;
double		ROBOT_STARFISH_MOTOR_SPEED	= 5.0;

// Controller-specific constants

double		NN_TAUS_MIN			= 0.0001;
double		NN_TAUS_MAX			= 1.0;

double		NN_WEIGHTS_MIN			= -16.0;
double		NN_WEIGHTS_MAX			= +16.0;

double		NN_OMEGAS_MIN			= -4.0;
double		NN_OMEGAS_MAX			= +4.0;

double		NN_SENSORWEIGHTS_MIN		= NN_WEIGHTS_MIN;
double		NN_SENSORWEIGHTS_MAX		= NN_WEIGHTS_MAX;

double		SENSOR_CHANGE_THRESHOLD_FOR_STOPPING_EVALUATION	= 0.0001;

// Movie-specific constants

//int		TIME_STEPS_PER_FRAME		= 4;
int		TIME_STEPS_PER_FRAME		= 0;

//#endif
